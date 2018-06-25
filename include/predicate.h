#ifndef PREDICATE_H
#define PREDICATE_H


#include "pred_rule.h"


/**
 * the recursive definition
 * binding arguments (var env)
 * base_rule, rec_rules
 *
 */
class predicate {
private:
        z3::func_decl m_fun;
        z3::expr_vector m_pars;
        z3::expr m_base_rule;
        std::vector<pred_rule> m_rec_rules; // exists
public:
        z3::expr_vector& get_pars() {return m_pars;}
        void get_data_pars(z3::expr_vector& data_pars);
predicate(z3::func_decl fun, z3::expr_vector pars, z3::expr base_rule)
        :m_fun(fun), m_pars(pars), m_base_rule(base_rule) {
        }

        void add_base_rule(z3::expr base_rule) {
                m_base_rule = base_rule;
        }

        void add_rec_rule(pred_rule pr) {
                m_rec_rules.push_back(pr);
        }

        z3::expr get_base_rule(){return m_base_rule;}

        int rec_rule_size() {return m_rec_rules.size();}


        pred_rule get_rule(int i) {
                assert(i < m_rec_rules.size());
                return m_rec_rules[i];
        }

        z3::func_decl get_fun() {
                return m_fun;
        }

        std::string get_pred_name() {
                return m_fun.name().str();
        }



        bool is_tree();
        bool is_list() ;
        bool is_listset() ;

        int size_of_static_parameters();
        int idx_E_gamma();

        z3::expr get_plfld();

        // list with set phi_p
        z3::expr get_phi_p(z3::context& ctx);
        void get_subset_relation(std::vector<int>& sub_r);


        friend std::ostream& operator<<(std::ostream& out, predicate& p) ;
};


#endif /* PREDICATE_H */
