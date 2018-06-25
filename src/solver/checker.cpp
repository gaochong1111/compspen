#include "checker.h"


/**
 *###################### checker ####################################3
 */
/**
 * check the expr whether numberal
 * param: $x is the expr
 * return: true, if $x is numeral
 */
bool checker::is_numeral(z3::expr x) {
        if (x.is_numeral()) return true;
        if (x.is_app() && x.decl().name().str()=="-" && x.num_args()==1 && x.arg(0).is_numeral()) return true;
        if (x.is_app()
            && (x.decl().name().str() == "to_real" || x.decl().name().str() == "to_int")
            && is_numeral(x.arg(0))) return true;
        return false;
}


/**
 * union two expr set
 * param: $s1 is the expr set
 * param: $s2 is the expr set
 * return: result = $s1 union $s2
 */
std::set<z3::expr, exprcomp> checker::union_set(std::set<z3::expr, exprcomp> s1, std::set<z3::expr, exprcomp> s2) {
        for (auto item : s2) {
                s1.insert(item);
        }
        return s1;
}

bool checker::is_repeat(z3::expr_vector vec) {
        std::set<z3::expr, exprcomp> args_set;
        for (int i=0; i<vec.size(); i++) {
                args_set.insert(vec[i]);
        }
        if (args_set.size() != vec.size()) {
                return true;
        }
        return false;
}

bool checker::is_repeat(std::vector<z3::expr> vec) {
        std::set<z3::expr, exprcomp> args_set;
        for (int i=0; i<vec.size(); i++) {
                args_set.insert(vec[i]);
        }
        if (args_set.size() != vec.size()) {
                return true;
        }
        return false;
}
