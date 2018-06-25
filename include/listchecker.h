#ifndef LISTCHECKER_H
#define LISTCHECKER_H

#include "checker.h"

class listchecker :public checker {
private:
        std::vector<z3::expr> m_xi;
        int index_of_pars(z3::expr x, std::vector<z3::expr>& vec);
public:
listchecker(predicate pred):checker(pred){}
        void check_args();
        void check_rec_rule(pred_rule& rule);
        void check_rec_rules();
};


#endif /* LISTCHECKER_H */
