#ifndef SMT2CONTEXT_H
#define SMT2CONTEXT_H

#include <iostream>
#include "log.h"
#include "predicate.h"


class smt2context {
private:
        bool m_exit_on_err;
        z3::context& m_ctx; // z3 context
        Log m_log; // logger

        // parse result
        std::vector<predicate> m_preds;
        z3::expr m_negf;
        z3::expr m_posf;

public:
        // some aux info used by solver
        int omega_eq_size;
        z3::expr phi_space;
        z3::expr psi_space;
        z3::expr phi_abs;
        z3::expr psi_abs;
        std::vector<z3::expr> phi_const_vec;
        std::vector<z3::expr> psi_const_vec;
        std::vector<int> psi_eq_to_eq_table;
        std::vector<int> phi_const_eq_class_vec;
        std::vector<int> psi_const_eq_class_vec;
public:



smt2context(z3::context& ctx, std::string log_file, bool exit_err=true) :m_ctx(ctx),
         m_exit_on_err(exit_err),
                m_negf(ctx),
                m_posf(ctx),
                omega_eq_size(0),
                phi_space(ctx),
                phi_abs(ctx),
                psi_abs(ctx),
                psi_space(ctx)
        {
                m_log.common_log_init(log_file);
        }

        Log& logger() {return m_log;}
        z3::context& z3_context() {return m_ctx;}
        bool exit_on_error() {return m_exit_on_err;}


        void add_predicate(predicate pred) {
                m_preds.push_back(pred);
        }

        void set_negf(z3::expr f) {
                m_negf = f;
        }

        void set_posf(z3::expr f) {
                m_posf = f;
        }

        int pred_size() {return m_preds.size();}

        predicate get_pred(int index);

        z3::expr get_negf() {
                return m_negf;
        }

        z3::expr get_posf() {
                return m_posf;
        }

        bool is_tree();

        bool is_list();

        bool is_listset();


        bool is_no_formula() ;

        bool is_sat() ;

        bool is_entl() ;


};

#endif /* SMT2CONTEXT_H */
