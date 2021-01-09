#include "solver.h"


/**
 *###################### solver ####################################3
 */
void solver::solve() {
    // 1. check_preds
    check_preds();

    // 3. check sat or entl
    if (m_ctx.is_sat()) {
        // std::cout << "Checking satisfiability.\n";
        z3::check_result result = check_sat();
        std::cout << result << std::endl;
    } else {
        // std::cout << "Checking entailment.\n";
        z3::check_result result = check_entl();
        if (result == z3::unsat) {
            std::cout << "unsat"  << std::endl;
        } else {
            std::cout << "sat" << std::endl;
        }
    }
}





/**
 * get data and space part by formula
 * @param formula : the formula
 * @param data : the result data part (translate to abstraction)
 * @param space : the result space part
 */
void solver::get_data_space(z3::expr &formula, z3::expr &data, z3::expr &space) {

    if (formula.decl().name().str() == "tobool") {
        // only space part
        data = z3_ctx().bool_val(true);
        space = formula;
    } else {
        // data and space
        z3::expr_vector data_items(z3_ctx());
        data_items.push_back(z3_ctx().int_const("nil") == 0);
        std::vector<z3::expr> stack;
        stack.push_back(formula);
        while(!stack.empty()) {
            z3::expr curr = stack.back();
            if (expr_tool::is_fun(curr, "and")) {
                stack.pop_back();
                for (int i=0; i<curr.num_args(); i++) {
                    if (curr.arg(i).is_quantifier()) {
                        data_items.push_back(curr.arg(i));
                        continue;
                    }

                    if (expr_tool::is_fun(curr.arg(i), "tobool")) {
                        space = curr.arg(i);
                        break;
                    }
                    // (= Z1 Z2) or (distinct Z1 Z2) ==> abs
                    if (curr.arg(i).num_args()==2 && expr_tool::is_location(curr.arg(i).arg(0))) {
                        z3::expr item = curr.arg(i);
                        z3::expr z1_int = z3_ctx().int_const(item.arg(0).to_string().c_str());
                        z3::expr z2_int = z3_ctx().int_const(item.arg(1).to_string().c_str());
                        if (item.decl().name().str() == "distinct") {
                            data_items.push_back(z1_int != z2_int);
                        } else {
                            data_items.push_back(z1_int == z2_int);
                        }
                    } else if(expr_tool::is_fun(curr.arg(i), "and")){
                        stack.push_back(curr.arg(i));
                    } else {
                        data_items.push_back(curr.arg(i));
                    }
                }
            }
        }

        //
        if (data_items.size() > 0) {
            data = mk_and(data_items);
        }

    }

    if (Z3_ast(space) != 0 && space.decl().name().str() == "tobool") {
        if (space.arg(0).decl().name().str() == "ssep") {
            space = space.arg(0);
            if (space.arg(space.num_args()-1).to_string() == "emp") {
                z3::symbol ssep_sys = z3_ctx().str_symbol("ssep");
                z3::sort range = z3_ctx().uninterpreted_sort("Space");
                z3::sort_vector domains(z3_ctx());
                z3::expr_vector args(z3_ctx());
                for (unsigned i=0; i<space.num_args()-1; i++) {
                        domains.push_back(range);
                        args.push_back(space.arg(i));
                }
                z3::func_decl ssep_f = z3_ctx().function(ssep_sys, domains, range);
                space = ssep_f(args);
            }
        }

    }
}

/**
 * index of the predicate in preds
 * @param: pred_name : the predicate name
 * @return: the index, if exist
 *          -1       , otherwise
 */
int solver::index_of_pred(std::string &pred_name) {
    for (int i=0; i<m_ctx.pred_size(); i++) {
        if (pred_name == m_ctx.get_pred(i).get_pred_name()) {
            return i;
        }
    }
    return -1;
}

/**
 * compute abstraction of space part
 * @param space : the space part
 * @param new_bools : new bools var
 * @return the abstraction of space part
 */
z3::expr solver::abs_space(z3::expr &space, z3::expr_vector& new_bools) {
    // 1.space atoms -> abs (\phi_sigma)
    z3::expr f_abs(z3_ctx());
    z3::expr b_false = z3_ctx().bool_val(false);
    for (int i=0; i<space.num_args(); i++) {
        //1.1 fetch atom
        z3::expr atom = space.arg(i);

        z3::expr atom_f = pred2abs(atom, i, new_bools);
        if (atom_f.hash() == b_false.hash()) return b_false;

        // 1.5 add atom_f to f_abs
        if (Z3_ast(f_abs) == 0) {
            f_abs = atom_f;
        } else {
            f_abs = f_abs && atom_f;
        }
    }

    return f_abs;
}


/**
 * compute phi_star by new_bools
 * @param new_bools : new bool vars
 * @return the phi_star
 */
z3::expr solver::abs_phi_star(z3::expr_vector& new_bools) {
    z3::expr phi_star = z3_ctx().bool_val(true);
    // phi_star
    for (int i=0; i<new_bools.size(); i++) {
        for (int j=i+1; j<new_bools.size(); j++) {
            std::string b1_name = new_bools[i].to_string();
            std::string b2_name = new_bools[j].to_string();
            int i1 = b1_name.find(",");
            int i2 = b2_name.find(",");
            std::string z1_name = b1_name.substr(2, i1-2);
            std::string z1_i = b1_name.substr(i1+1, b1_name.length()-i1-3);
            std::string z2_name = b2_name.substr(2, i2-2);
            std::string z2_i = b2_name.substr(i2+1, b2_name.length()-i2-3);

            if (z1_i != z2_i) {
                // add imply atom
                // z3::expr imply = implies( (z3_ctx().int_const(z1_name.c_str()) == z3_ctx().int_const(z2_name.c_str()) && new_bools[i]), !new_bools[j]);
                z3::expr imply = ! ((z3_ctx().int_const(z1_name.c_str()) == z3_ctx().int_const(z2_name.c_str()) && new_bools[i]) && new_bools[j]);
                // if (Z3_ast(phi_star) == 0) {
                //    phi_star = imply;
                // } else {
                phi_star = phi_star && imply;
                //}
            }
        }
    }
    return phi_star;
}
