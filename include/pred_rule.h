#ifndef PRED_RULE_H
#define PRED_RULE_H

#include <z3++.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "expr_tool.h"


/**
 * pred_rule
 * data, pto, rec_app
 *
 */
class pred_rule {
private:
        z3::expr m_data;
        z3::expr m_pto;
        z3::expr_vector m_rec_apps;
        // void get_x_h(z3::expr exp, std::set<z3::expr, exprcomp>& x_h);

public:
pred_rule(z3::expr data, z3::expr pto, z3::expr_vector rec_apps)
        :m_data(data), m_pto(pto), m_rec_apps(rec_apps) {
        }

        z3::expr get_data() {return m_data;}
        z3::expr get_pto() {return m_pto;}
        z3::expr_vector get_rec_apps() {return m_rec_apps;}
        /**
         * get exists args (x, h)
         * @param x_h_vec : the result vector
         */
        void get_x_h(z3::expr_vector& x_h_vec);


        bool is_tree_rule();

        bool is_list_rule();
        friend std::ostream& operator<<(std::ostream& out, pred_rule& pr);
};


#endif /* PRED_RULE_H */
