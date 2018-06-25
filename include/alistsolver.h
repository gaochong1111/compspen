#ifndef ALISTSOLVER_H_INCLUDED
#define ALISTSOLVER_H_INCLUDED
#include "listsolver.h"

class alistsolver: public listsolver {
protected:
    // match
    bool match_graph(graph& psi_g, graph& omega_g_i);
    z3::expr pred_unfold_ge1(z3::expr& atom, int i, z3::expr_vector& new_bools);

public:
    alistsolver(smt2context& ctx) : listsolver(ctx) {}

};


#endif // ALISTSOLVER_H_INCLUDED
