#include "predicate.h"

bool predicate::is_tree() {
        for (int i=0; i<m_rec_rules.size(); i++) {
                if (!m_rec_rules[i].is_tree_rule()) return false;
        }
        return true;
}

bool predicate::is_list() {
        return m_rec_rules.size() == 1 && m_rec_rules[0].is_list_rule();
}

bool predicate::is_listset() {
        if (m_rec_rules.size() != 1) return false;
        for (int i=0; i<m_pars.size()-1; i++) {
                if (expr_tool::is_setint(m_pars[i])) return true;
        }
        return false;
}

std::ostream& operator<<(std::ostream& out, predicate& p) {
        out << "pars: \n" << p.m_pars << std::endl;
        out << "base rule: \n" << p.m_base_rule << std::endl;
        out << "m_rec_rules: \n";
        for (int i=0; i<p.m_rec_rules.size(); i++) {
                out << "index: " << i << " :\n" << p.m_rec_rules[i] << std::endl;
        }
        return out;
}

/**
 * the size of static parameters for list predicate
 * @return size
 */
int predicate::size_of_static_parameters() {
        if (is_list()) {
                int i = m_pars.size()-1;
                for (; i>=0; i--) {
                        if (m_pars[i].to_string().find("sta_")!=0) break;
                }
                return m_pars.size() - i - 1;
        }
        return 0;
}

void predicate::get_data_pars(z3::expr_vector& data_pars) {
    for (int i=0; i<m_pars.size(); i++) {
        if (!expr_tool::is_location(m_pars[i])) {
            data_pars.push_back(m_pars[i]);
        }
    }
}

/**
 * the index, when E occurs in gamma for list predicate
 * @return -1, E does not occur in gamma
 *         index
 */
int predicate::idx_E_gamma() {
        if (is_list()) {
                int size = m_pars.size() - size_of_static_parameters();
                for (int i=1; i<size/2; i++) {
                        if (m_rec_rules[0].get_rec_apps()[0].arg(i).hash() == m_pars[0].hash()) return i-1;
                }
        }
        return -1;
}

/**
 * get principal location filed
 */
z3::expr predicate::get_plfld() {
        if (is_list()) {
                z3::expr pto = m_rec_rules[0].get_pto();
                z3::expr rec_app_source = m_rec_rules[0].get_rec_apps()[0].arg(0);
                 z3::expr sref = pto.arg(1);
                if (sref.decl().name().str() == "sref") {
                        for (int j=0; j<sref.num_args(); j++) {
                                z3::expr ref = sref.arg(j);
                                z3::expr dest = ref.arg(1);
                                if (dest.hash() == rec_app_source.hash()) {
                                        return ref.arg(0);
                                }
                        }
                } else {
                        return sref.arg(0);

                }
        }
        return m_pars[0];
}

/**
 * get phi_p (delta) of list in inductive rule
 */
z3::expr predicate::get_phi_p(z3::context& ctx) {
        z3::expr delta = get_rule(0).get_data();
        if (is_listset() || is_list()) {
                int st_size = size_of_static_parameters();
                int size = m_pars.size() - st_size;
                pred_rule rule = m_rec_rules[0];
                z3::expr rec_app = rule.get_rec_apps()[0];

                z3::expr_vector src_pars(ctx);
                z3::expr_vector dst_pars(ctx);
                for (int i=0; i<size/2; i++) {
                        src_pars.push_back(rec_app.arg(i));
                        dst_pars.push_back(rec_app.arg(i+size/2));
                }
                delta = delta.substitute(src_pars, dst_pars); // delta = phi_r1 && phi_r2
        }
        return delta;
}
/**
 * list with set: get subset relation of alpha set with beta set
 * @param sub_r:  [alpha_i sub beta_i -> 1] alpha_i sub beta_i
 */
void predicate::get_subset_relation(std::vector<int> &sub_r) {
        if (is_listset()) {
                z3::expr delta = get_rule(0).get_data();
                int st_size = size_of_static_parameters();
                int size = m_pars.size() - st_size;

                std::vector<z3::expr> alpha;
                for (int i=1; i<size/2; i++) {
                        alpha.push_back(m_pars[i]);
                        sub_r.push_back(-1);
                }
                for (int i=0; i<delta.num_args(); i++) {
                        z3::expr item_i = delta.arg(i);
                        if (item_i.num_args() == 2) {
                                z3::expr s1 = item_i.arg(0);
                                int idx = -1;
                                int sub = 0;
                                if (expr_tool::is_setint(s1) && s1.is_var()) {
                                        z3::expr s2 = item_i.arg(1);
                                        if (s2.num_args() == 2) {
                                                s2 = s2.arg(0);
                                        }
                                        idx = expr_tool::index_of_exp(s2, alpha);
                                        sub = 1;
                                } else {
                                        idx = expr_tool::index_of_exp(s1, alpha);
                                }
                                if (idx != -1) {
                                        sub_r[idx] = sub;
                                }
                        }
                }
        }
}
