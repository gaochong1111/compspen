#ifndef SOLVER_H
#define SOLVER_H
#include "checker.h"

/**
 * Solver
 */
class solver {
protected:
     smt2context& m_ctx;
     // z3::expr_vector new_bools;
     z3::expr_vector delta_ge1_predicates; // phi^{>=1}_P(\alpha; \beta), corresponding to preds_array
     z3::solver s;


protected:
     void get_data_space(z3::expr& formula, z3::expr& data, z3::expr& space);
     int index_of_pred(std::string& pred_name);
     z3::expr abs_space(z3::expr& space, z3::expr_vector& new_bools);
     z3::expr abs_phi_star(z3::expr_vector& new_bools);

     smt2context& get_ctx() {return m_ctx;}
     z3::context& z3_ctx() {return m_ctx.z3_context();}
     Log& logger() {return m_ctx.logger();}


     virtual void check_preds()=0;
     virtual z3::expr pred2abs(z3::expr& atom, int i, z3::expr_vector& new_bools)=0;
     // virtual std::string get_model_of_atom(z3::model& m, z3::expr& atom, int i, int n)=0;

public:
solver(smt2context& ctx): m_ctx(ctx), delta_ge1_predicates(ctx.z3_context()), s(ctx.z3_context()) {}
     virtual z3::check_result check_sat()=0;
     virtual z3::check_result check_entl()=0;
     void solve();
     virtual z3::model get_model()=0;

};




#endif /* SOLVER_H */
