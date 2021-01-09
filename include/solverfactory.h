#ifndef SOLVERFACTORY_H
#define SOLVERFACTORY_H
#include "solver.h"
#include "listsolver.h"
#include "smt2context.h"
#include "alistsolver.h"

class solverfactory {
public:
        static solver* get_solver(smt2context& ctx) {
                // solve the tree predicate case

                if (ctx.is_tree()) {
                        std::cout << "tree solver.\n";
                        //  treesolver sol(ctx);
                        // return new treesolver(ctx);
                } else if (ctx.is_listset()) {
                        std::cout << "list with set solver.\n";
                        // return new listsetsolver(ctx);
                } else if (ctx.is_list()) {
                        //TODO: solve the list predicate case
                        // std::cout << "list solver.\n";
                        // listsolver sol(ctx);
                        //sol.solve();
                        assert(ctx.pred_size() > 0);
                        if (ctx.division != "NORMAL") {
                            return new alistsolver(ctx);
                        } else {
                            return new listsolver(ctx);
                        }
                }  else {
                        std::cout << "unsupported. \n";
                        return NULL;
                }
        }

        static solver* get_solver(smt2context& ctx, std::string logic) {
                // solve the tree predicate case
                // std::cout << logic << std::endl;
                if (logic == "QF_SLID_LC") {
                    return new alistsolver(ctx);
                } else if (logic == "QF_SLID_LCD") {
                    return new listsolver(ctx);
                } else {
                    std::cout << "unsupported logic. \n";
                    exit(-1);
                    return NULL;
                }
        }
};



#endif /* SOLVERFACTORY_H */
