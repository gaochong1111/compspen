#ifndef CHECKER_H
#define CHECKER_H

#include "smt2context.h"
#include "smt2exception.h"
#include "predicate.h"

/**
 * Checker
 */
class checker {
protected:
        predicate m_pred;
        bool is_numeral(z3::expr x);
        bool is_repeat(z3::expr_vector vec);
        bool is_repeat(std::vector<z3::expr> vec);
        std::set<z3::expr, exprcomp> union_set(std::set<z3::expr, exprcomp> s1, std::set<z3::expr, exprcomp> s2);
public:
checker(predicate pred):m_pred(pred){}
        virtual void check_args()=0;
        virtual void check_rec_rule(pred_rule& rule)=0;
        virtual void check_rec_rules()=0;
};

#endif /* CHECKER_H */
