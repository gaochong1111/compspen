#include "pred_rule.h"

/*
void pred_rule::get_x_h(z3::expr exp, std::set<z3::expr, exprcomp>& x_h) {
        if (exp.is_app()) {
                for (int i=0; i<exp.num_args(); i++) {
                        get_x_h(exp.arg(i), x_h);
                }
        } else {
                if (exp.is_var()) {
                        x_h.insert(exp);
                }
        }
}
*/
/**
 * get exists args (x, h)
 * @param x_h_vec : the result vector
 */
void pred_rule::get_x_h(z3::expr_vector& x_h_vec) {
        std::set<z3::expr, exprcomp> x_h;
        expr_tool::get_vars(m_data, x_h);
        expr_tool::get_vars(m_pto, x_h);
        for (int i=0; i<m_rec_apps.size(); i++) {
                expr_tool::get_vars(m_rec_apps[i], x_h);
        }
        for (auto exp : x_h) {
                x_h_vec.push_back(exp);
        }
}



bool pred_rule::is_tree_rule() {
        return m_rec_apps.size() == 2;
}

bool pred_rule::is_list_rule() {
        return m_rec_apps.size() == 1;
}

std::ostream& operator<<(std::ostream& out, pred_rule& pr) {
        out << "data: " << pr.m_data << std::endl;
        out << "pto: " << pr.m_pto << std::endl;
        out << "m_rec_apps: " << pr.m_rec_apps << std::endl;
        return out;
}
