#include "listchecker.h"


/**
 *################## list checker TODO.. #############
 */

void listchecker::check_args() {
//        std::cout << "list::check_args\n";
        // 1. fetch static parameters
        z3::expr_vector args = m_pred.get_pars();
        int size = args.size();
        int i=size-1;
        for (; i>=0; i--) {
                if (args[i].to_string().find("sta_")!=0) break;
                m_xi.push_back(args[i]);
        }
        size = i+1; // size of source and destination parameters
        // 2. source parameters and destination parameters
        if (size > 0 && size % 2 != 0){
                throw smt2exception("the number of source and destination parameters of predicate must be even number. \n");
        }
        // 3. first parameter
        z3::expr first_arg = args[0];
        if (first_arg.get_sort().sort_kind() != Z3_UNINTERPRETED_SORT) {
                throw smt2exception("the first parameter of predicate must be record type. \n");
        }

        // 4. source and dest paramters type match
        for (int i=0; i<size/2; i++) {
                if (args[i].get_sort().sort_kind() != args[i+size/2].get_sort().sort_kind()) {
                        throw smt2exception("the source parameters and destination parameters of predicate must be matched in types. \n");
                }
        }

        // 5. no repeat
        if (is_repeat(args)) {
                throw smt2exception("the parameters of predicate must be different from each other. \n");
        }
}

void listchecker::check_rec_rule(pred_rule &rule) {
//        std::cout << "list::check_rec_rule\n";
        //
        z3::expr data = rule.get_data();
        z3::expr pto = rule.get_pto();
        z3::expr_vector rec_apps = rule.get_rec_apps();

        z3::expr_vector args =  m_pred.get_pars();
        // std::cout << "parameters: " << args << std::endl;
        // std::cout << "data: " << data << std::endl;
        // std::cout << "pto: " << pto << std::endl;
        // std::cout << "rec_app: " << rec_apps << std::endl;
        int size = args.size() - m_xi.size(); //  size of source and destination parameters
        std::vector<z3::expr> alpha;
        std::vector<z3::expr> beta;
        z3::expr E = args[0];
        z3::expr F = args[size/2];
        for (int i=1; i<size/2; i++) {
                alpha.push_back(args[i]);
                beta.push_back(args[i+size/2]);
        }

        // 0. rec_apps
        if (rec_apps.size() != 1) {
                throw smt2exception("the number of recursive applications in recursive rules of  predicate must be one. \n");
        }
        z3::expr rec_app = rec_apps[0];
        std::vector<int> alpha_i_flags(alpha.size(),0); // alpha_i occurs in rho or data
        std::vector<int> xi_j_flags(m_xi.size(), 0); // xi_j occurs in rho or data


        // 1. data
        for (unsigned int i=0; i<data.num_args(); i++) {
                z3::expr data_item = data.arg(i);
                if (data_item.num_args() == 2) {
                        // alpha_i < c or alpha_i < xi_j + c or alpha_i < gamma_i + c
                        // std::cout << "data item: " << data_item << std::endl;
                        z3::expr alpha_i = data_item.arg(0);
                        int idx_i = index_of_pars(alpha_i, alpha);


                        if (idx_i == -1) {
                                throw smt2exception("the data item  of recursive rule of  predicate must be like alpha_i < c or alpha_i < xi_j + c or alpha_i < gamma_i + c. \n");
                        }
                        z3::expr other = data_item.arg(1);
                        if (!other.is_app()) {
                                throw smt2exception("the data item  of recursive rule of  predicate must be like alpha_i < c or alpha_i < xi_j + c or alpha_i < gamma_i + c. \n");
                        }

                        if (other.num_args() != 2) {
                                if (!is_numeral(other)) {
                                        throw smt2exception("the data item  of recursive rule of  predicate must be like alpha_i < c or alpha_i < xi_j + c or alpha_i < gamma_i + c. \n");
                                }
                        } else if (other.num_args() == 2) {
                                if (other.is_app() && other.decl().name().str() == "+") {
                                        z3::expr op1 = other.arg(0);
                                        z3::expr op2 = other.arg(1);
                                        if (is_numeral(op2)) {
                                                int idx_j = index_of_pars(op1, m_xi);
                                                if (idx_j != -1) {
                                                        xi_j_flags[idx_j]=1;
                                                }
                                                else if(op1.hash() == rec_app.arg(idx_i+1).hash()) {
                                                        if (data_item.decl().name().str() == "=") {
                                                                alpha_i_flags[idx_i] = 1;
                                                        }
                                                } else {
                                                        throw smt2exception("the data item  of recursive rule of  predicate must be like alpha_i < c or alpha_i < xi_j + c or alpha_i < gamma_i + c. \n");
                                                }
                                        } else {
                                                throw smt2exception("the data item  of recursive rule of  predicate must be like alpha_i < c or alpha_i < xi_j + c or alpha_i < gamma_i + c. \n");
                                        }
                                } else {
                                        throw smt2exception("the data item operator  of recursive rule of  predicate must be PLUS. \n");
                                }
                        }
                }
        }

        // 2. pto
        z3::expr pto_source = pto.arg(0);
        if (pto_source.hash() != E.hash()) {
                throw smt2exception("the source of pto atom in the recursive rule of  predicate must be the first parameter of predicate. \n");
        }

        z3::expr refs = pto.arg(1);
        if (refs.decl().name().str() == "sref") {
                for (int i=0; i<refs.num_args(); i++) {
                        z3::expr ref = refs.arg(i);
                        z3::expr f_dest = ref.arg(1);
                        int idx_i = index_of_pars(f_dest, alpha);
                        if (idx_i != -1) {
                                alpha_i_flags[idx_i] = 1;
                        } else {
                                int idx_j = index_of_pars(f_dest, m_xi);
                                if (idx_j != -1) {
                                        xi_j_flags[idx_j] = 1;
                                } else if (f_dest.to_string().find("(:var") == 0) {
                                       if (i==0 && f_dest.hash() != rec_app.arg(0).hash()) {
                                                throw smt2exception("the location field of pto atom in the recursive rule of  predicate from exists parameters  must be the first parameter of recursive application. \n");
                                        }
                                }
                                else {
                                        throw smt2exception("the field of pto atom in the recursive rule of predicate has error destination. \n");
                                }
                        }
                }
        } else {
                z3::expr f_dest = refs.arg(1);
                if (f_dest.to_string().find("(:var") == 0 && f_dest.hash() == rec_app.arg(0).hash()) {
                } else {
                        throw smt2exception("the location field of pto atom in the recursive rule of  predicate from exists parameters  must be the first parameter of recursive application. \n");
                }
        }

        // alpha_i is data
        for (int i=0; i<alpha.size(); i++) {
                z3::expr alpha_i = alpha[i];
                if ((alpha_i.get_sort().to_string() == "Int" || alpha_i.get_sort().to_string()=="Real") && alpha_i_flags[i]==0) {
                        throw smt2exception("when alpha_i is data parameter, either alpha_i occurs in rho or alpha_i = gamma_i + c occurs in delta.");
                }
                if (alpha_i.get_sort().sort_kind() == Z3_UNINTERPRETED_SORT && alpha_i_flags[i]==0) {
                        throw smt2exception("the location variable of alpha must occur in rho.");
                }
        }

        // all location of alpha and  X  and xi  occur in rho
        for (int i=0; i<m_xi.size(); i++) {
                if (m_xi[i].get_sort().sort_kind() == Z3_UNINTERPRETED_SORT && alpha_i_flags[i]==0) {
                        throw smt2exception("the location variable of xi must occur in rho.");
                }
        }

        // 3. rec_apps
        // gamma
        for (int i=1; i<size/2;i++) {
                z3::expr gamma_i = rec_app.arg(i);
                if (gamma_i.hash() == E.hash()) {
                }
                else if (gamma_i.to_string().find("(:var") != 0) {
                        throw smt2exception("gamma_i must be from exists parameters.");
                }
        }

        // rec_app args
        std::vector<z3::expr> rec_args;
        for (int i=0; i<rec_app.num_args(); i++) {
                rec_args.push_back(rec_app.arg(i));
        }
        if (is_repeat(rec_args)) {
                throw smt2exception("the parameters of recursive application must be different from each other. \n");

        }
}

void listchecker::check_rec_rules() {
        if (m_pred.rec_rule_size() != 1) {
                throw smt2exception("the number of recursive rules of  predicate must be one. \n");
        }
        pred_rule rule = m_pred.get_rule(0);
        check_rec_rule(rule);
}

int listchecker::index_of_pars(z3::expr x, std::vector<z3::expr> &vec) {
        for (int i=0; i<vec.size(); i++) {
                if (x.hash() == vec[i].hash()) return i;
        }
        return -1;
}
