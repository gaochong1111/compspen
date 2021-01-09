#include "alistsolver.h"

bool alistsolver::match_graph(graph& g_psi, graph& omega_g_i) {
	 // TODO (jackchong#1#06/18/18): compute allocated-table ...

	// m_ctx.phi_abs + omega_g_i -> allocated_table:
	// eq, neq table:

	// std::cout << "alist:: match_graph!!!\n";

	// logger() << "phi_abs: " << m_ctx.phi_abs << std::endl;

	logger() << "alist:: match graph\n";


	int eq_size = m_ctx.omega_eq_size;
	int const_size = m_ctx.phi_const_eq_class_vec.size();
	logger() << "equality class size: " << eq_size << std::endl;

	std::vector<std::vector<int> > eq_to_const_vec(eq_size);
	for (int i=0; i<const_size; i++) {
		eq_to_const_vec[m_ctx.phi_const_eq_class_vec[i]].push_back(i);
	}

	int nil_eq = m_ctx.phi_const_eq_class_vec[0];

	logger() << "eq class: \n";

	for (int i=0; i<eq_size; i++) {
		logger() << i <<": ";
		for (int j=0; j<eq_to_const_vec[i].size(); j++) {
			logger() << m_ctx.phi_const_vec[eq_to_const_vec[i][j]] << " ";
		}
		logger() << std::endl;
	}
	logger() << std::endl;

	z3::solver s(z3_ctx());
	bool allocated_table[eq_size]; // location variable allocated
	for (int i=0; i<eq_size; i++) {
		int vid = omega_g_i.get_vertex_id(eq_to_const_vec[i][0]);
		z3::expr_vector neg_vec(z3_ctx());
		std::vector<graph::edge_descriptor> rc_edges = omega_g_i.get_reachable_edges(vid);
		for (int j=0; j<rc_edges.size(); j++) {
			int idx = omega_g_i.get_edge_property(rc_edges[j]);
			std::string source_name = logger().string_format("[%s,%d]", m_ctx.phi_space.arg(idx).arg(0).to_string().c_str(), idx);
			neg_vec.push_back(!z3_ctx().bool_const(source_name.c_str()));
		}
		s.reset();
		z3::expr neg_cf = z3::mk_and(neg_vec);
		z3::expr check_f = m_ctx.phi_abs && neg_cf;
		s.add(check_f);
		if (s.check() == z3::unsat) {
			allocated_table[i] = true;
		} else {
			allocated_table[i] = false;
		}
	}

	logger() << "eq class allocated table: \n";

	for (int i=0; i<eq_size; i++) {
		logger() << i <<": " << allocated_table[i] << std::endl;
	}
	logger() << std::endl;


	bool neq_table[eq_size][eq_size]; // equality class neq relation


	for (int i=0; i<eq_size; i++) {
		neq_table[i][i] = false;
		for (int j=i+1; j<eq_size; j++) {
			int idx_x = eq_to_const_vec[i][0];
			z3::expr X = z3_ctx().int_const(m_ctx.phi_const_vec[idx_x].to_string().c_str());
			int idx_y = eq_to_const_vec[j][0];
			z3::expr Y = z3_ctx().int_const(m_ctx.phi_const_vec[idx_y].to_string().c_str());
			s.reset();
			s.add(m_ctx.phi_abs && (X == Y));
			if (s.check() == z3::unsat) {
				neq_table[i][j] = true;
				neq_table[j][i] = true;
			} else {
				neq_table[i][j] = false;
				neq_table[j][i] = false;
			}
		}
	}

	logger() << "neq table: \n";
	for (int i=0; i<eq_size; i++) {
		for (int j=0; j<eq_size; j++) {
			logger() << "\t" << neq_table[i][j];
		}
		logger() << std::endl;
	}
	logger() << std::endl;



	//  get psi_edges
	std::vector<std::pair<std::pair<int, int>, int> > psi_edge_vec;
	g_psi.get_edges(psi_edge_vec);
	 /*
	for (int i=0; i<psi_edge_vec.size(); i++) {
		logger() <<"psi edge: "<< m_ctx.psi_const_vec[psi_edge_vec[i].first.first]
			<< "--" << psi_edge_vec[i].second << "--"
			<< m_ctx.psi_const_vec[psi_edge_vec[i].first.second] << std::endl;
	}
	*/
	// get omega_edges
	std::vector<std::pair<std::pair<int, int>, int> > omega_edge_vec;
	omega_g_i.get_edges(omega_edge_vec);
	/*
	for (int i=0; i<omega_edge_vec.size(); i++) {
		logger() <<"omega edge: "<< m_ctx.phi_const_vec[omega_edge_vec[i].first.first]
			<< "--" << omega_edge_vec[i].second << "--"
			<< m_ctx.phi_const_vec[omega_edge_vec[i].first.second] << std::endl;
	}
	*/
	// std::vector<int> omega_edge_table(omega_edge_vec.size(), -1);

	std::vector<int> omega_edge_table(m_ctx.phi_space.num_args());
	for (int i=0; i<omega_edge_vec.size(); i++) {
			omega_edge_table[omega_edge_vec[i].second] = -1;
	}

	// get nodes set reach nil
	std::set<size_t> nil_set = omega_g_i.get_reachable_set(nil_eq);
	logger() << "reach nil set: \n";
	for (auto i : nil_set) {
		logger() << i << " ";
	}
	logger() << std::endl;

	//CC
	std::vector<int> cc_cycle_num = omega_g_i.get_cc_cycle_num();
	std::vector<std::set<int> > cc_cycle_intersection(cc_cycle_num.size());// intersection
	std::vector<std::set<int> > cc_cycle_set(cc_cycle_num.size());
	for (int i=0; i<cc_cycle_num.size(); i++) {
		if (cc_cycle_num[i] != 0) {
			std::pair<int, int> coord(i, 0);
			std::vector<int> cycle = omega_g_i.get_cycle(coord);
			logger() << "cycle: ";
			for (int j=0; j<cycle.size(); j++) {
				logger() << " " << cycle[j];
				// logger() << m_ctx.phi_const_vec[eq_to_const_vec[cycle[j]][0]] << " ";
				cc_cycle_set[i].insert(cycle[j]);
			}
			logger() << std::endl;
		}
	}

	// special case psi
    if (psi_edge_vec.size() == 0) {
        for (int i=1; i<m_ctx.psi_const_vec.size(); i++) {
            int idx = expr_tool::index_of_exp(m_ctx.psi_const_vec[i], m_ctx.phi_const_vec);
            int src = omega_g_i.get_vertex_id(idx);
            std::vector<graph::edge_descriptor> path = omega_g_i.get_path(src);
            for (int j=0; j<path.size(); j++) {
                 omega_edge_table[omega_g_i.get_edge_property(path[j])] ++;
            }
        }
    }


	// get all edges in psi graph, match
	std::pair<std::pair<int, int>, int> edge;
	for (int i=0; i<psi_edge_vec.size(); i++) {
		logger() <<"psi edge: "<< m_ctx.psi_const_vec[psi_edge_vec[i].first.first]
			<< "--" << psi_edge_vec[i].second << "--"
			<< m_ctx.psi_const_vec[psi_edge_vec[i].first.second] << std::endl;

		edge = psi_edge_vec[i];

		z3::expr psi_atom = m_ctx.psi_space.arg(edge.second);

		int src = expr_tool::index_of_exp(m_ctx.psi_const_vec[edge.first.first], m_ctx.phi_const_vec);
		int dst = expr_tool::index_of_exp(m_ctx.psi_const_vec[edge.first.second], m_ctx.phi_const_vec);

		src = omega_g_i.get_vertex_id(src);
		dst = omega_g_i.get_vertex_id(dst);
		// src and dst are in the same cc
		int cc_id = omega_g_i.which_cc(src);
		if (cc_id != omega_g_i.which_cc(dst)) return false; // path in different cc

		std::vector<graph::edge_descriptor> path = omega_g_i.get_path(src, dst);

		logger() << "path: \n";
		for (int j=0; j<path.size(); j++) {
				logger() << omega_g_i.source(path[j]);
				logger() << "---";
				logger() << omega_g_i.get_edge_property(path[j]);
				logger() << "---";
				logger() << omega_g_i.target(path[j]) << std::endl;
		}

		if (expr_tool::is_fun(psi_atom, "pto")) {
			// pto ->  pto
			logger() << "psi pto match path.\n";
			if (path.size() > 1) {
				logger() << "psi pto: mismatch path!\n";
				return false;
			} else {
				int idx = omega_g_i.get_edge_property(path[0]);
				if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
					// TODO math parameters
					return true;
				} else {
					logger() << "psi pto: mismatch predicate atom.\n";
					return false;
				}
			}

		} else {
			 // match psi-atom -> phi-path
			 if (src == dst) continue;

			  // pred_atom match path
			if (cc_cycle_num[cc_id] == 0) {
					logger() << "omega graph is dag. \n";
					// dag
					//  pred-atom match paths in dag ...

					if (path.size() == 1) {
						int src_eq = omega_g_i.source(path[0]);
						int dst_eq = omega_g_i.target(path[0]);
						if (expr_tool::is_fun(m_ctx.phi_space.arg(omega_g_i.get_edge_property(path[0])), "pto")) {

							if (!neq_table[src_eq][dst_eq]) {
								logger() << "psi pred: pto incomplete neq relation.\n";
								return false; // pto not neq relation
							}
						}

						if (m_ctx.division == "ACYCLIC-DLL") {
							int idx = omega_g_i.get_edge_property(path[0]);
							z3::expr atom = m_ctx.phi_space.arg(idx);
							z3::expr prev_arg = atom.arg(1).arg(1).arg(1);
							idx = expr_tool::index_of_exp(prev_arg, m_ctx.phi_const_vec);
							int prev_eq = m_ctx.phi_const_eq_class_vec[idx];
							if (!neq_table[src_eq][prev_eq]) {
								logger() << "psi pred: pto incomplete neq relation.\n";
								return false; // pto not neq relation
							}
						}
					} else {
						if (m_ctx.division == "ACYCLIC-SL") {
							if(!allocated_table[dst]) {
								if (nil_set.find(dst) == nil_set.end()) {
									int j=0;
									for (; j<path.size()-1; j++) {
										int idx = omega_g_i.get_edge_property(path[j]);
										int src_eq = omega_g_i.source(path[j]);
										// int dst_eq = omega_g_i.target(path[j]);
										if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
											if (!neq_table[src_eq][dst]) {
												logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
												return false; // pto + ... + (pto) -> incomplete neq relations -> match
											}
										} else {
											logger() << "psi pred: ... + ls + ... + \n";
											return false; // pred-edge: ... + ls + ... + -> mismatch
										}
									}
									// the last edge
									int idx = omega_g_i.get_edge_property(path[j]);
									int src_eq = omega_g_i.source(path[j]);
									if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
										if (!neq_table[src_eq][dst]) {
											logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
											return false; // pto + ... + (pto) -> incomplete neq relations -> match
										}
									}
								}
							}
						} else {
							// dll
							// path match
							z3::expr psi_P = psi_atom.arg(1);
							int tmp_i = expr_tool::index_of_exp(psi_P, m_ctx.phi_const_vec);
							int psi_P_eq = m_ctx.phi_const_eq_class_vec[tmp_i];



							z3::expr psi_L = psi_atom.arg(3);
							tmp_i = expr_tool::index_of_exp(psi_L, m_ctx.phi_const_vec);
							int psi_L_eq = m_ctx.phi_const_eq_class_vec[tmp_i];
							// head node
							int prev_L_eq = -1;
							int idx = omega_g_i.get_edge_property(path[0]);
							int idx1 = -1;
							z3::expr path_0 = m_ctx.phi_space.arg(idx);
							if (expr_tool::is_fun(path_0, "pto")) {
                                idx = expr_tool::index_of_exp(path_0.arg(0), m_ctx.phi_const_vec);
                                idx1 = expr_tool::index_of_exp(path_0.arg(1).arg(1).arg(1), m_ctx.phi_const_vec);
							} else {
                                idx = expr_tool::index_of_exp(path_0.arg(3), m_ctx.phi_const_vec);
                                idx1 = expr_tool::index_of_exp(path_0.arg(1), m_ctx.phi_const_vec);
							}

							if (psi_P_eq != m_ctx.phi_const_eq_class_vec[idx1]) {
                                logger() << "pred atom can not math the first node in path!\n";
                                return false;
							}

							prev_L_eq = m_ctx.phi_const_eq_class_vec[idx];
							int cur_P_eq = -1;

							for (int j=1; j<path.size(); j++) {
                                idx = omega_g_i.get_edge_property(path[j]);
                                z3::expr path_j = m_ctx.phi_space.arg(idx);
                                if (expr_tool::is_fun(path_j, "pto")) {
                                    idx = expr_tool::index_of_exp(path_j.arg(1).arg(1).arg(1), m_ctx.phi_const_vec);
                                    idx1 = expr_tool::index_of_exp(path_j.arg(0), m_ctx.phi_const_vec);
                                } else {
                                    idx = expr_tool::index_of_exp(path_j.arg(1), m_ctx.phi_const_vec);
                                    idx1 = expr_tool::index_of_exp(path_j.arg(3), m_ctx.phi_const_vec);
                                }
                                cur_P_eq = m_ctx.phi_const_eq_class_vec[idx];

                                if (cur_P_eq != prev_L_eq) {
                                    logger() << "pred atom can not math the path!\n";
                                    return false;
                                }
                                prev_L_eq = m_ctx.phi_const_eq_class_vec[idx1];
							}



							if (psi_L_eq != prev_L_eq) {
                                logger() << "pred atom can not math the last node in path!\n";
                                return false;
							}

							// 4 cases
							bool b_dst = allocated_table[dst] || (nil_set.find(dst) != nil_set.end());
							bool b_P = allocated_table[psi_P_eq] || (nil_set.find(psi_P_eq) != nil_set.end());


							if (!b_dst && b_P) {
                                int j=0;
                                for (; j<path.size()-1; j++) {
                                    int idx = omega_g_i.get_edge_property(path[j]);
                                    int src_eq = omega_g_i.source(path[j]);
                                    // int dst_eq = omega_g_i.target(path[j]);
                                    if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                        if (!neq_table[src_eq][dst]) {
                                            logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                            return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                        }
                                    } else {
                                        logger() << "psi pred: ... + ls + ... + \n";
                                        return false; // pred-edge: ... + ls + ... + -> mismatch
                                    }
                                }
                                // the last edge
                                int idx = omega_g_i.get_edge_property(path[j]);
                                int src_eq = omega_g_i.source(path[j]);
                                if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                    if (!neq_table[src_eq][dst]) {
                                        logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                        return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                    }
                                }

							} else if (b_dst && !b_P) {
                                int j=path.size()-1;
                                for (; j>0; j--) {
                                    int idx = omega_g_i.get_edge_property(path[j]);
                                    int src_eq = omega_g_i.source(path[j]);
                                    // int dst_eq = omega_g_i.target(path[j]);
                                    if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                        if (!neq_table[src_eq][psi_P]) {
                                            logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                            return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                        }
                                    } else {
                                        logger() << "psi pred: ... + ls + ... + \n";
                                        return false; // pred-edge: ... + ls + ... + -> mismatch
                                    }
                                }
                                // the last edge
                                int idx = omega_g_i.get_edge_property(path[j]);
                                int src_eq = omega_g_i.source(path[j]);
                                if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                    if (!neq_table[src_eq][psi_P]) {
                                        logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                        return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                    }
                                }

							} else if(!b_dst && !b_P){
                                int j=0;
                                for (; j<path.size()-1; j++) {
                                    int idx = omega_g_i.get_edge_property(path[j]);
                                    int src_eq = omega_g_i.source(path[j]);
                                    // int dst_eq = omega_g_i.target(path[j]);
                                    if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                        if (!neq_table[src_eq][dst] && !neq_table[src_eq][psi_P]) {
                                            logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                            return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                        }
                                    } else {
                                        logger() << "psi pred: ... + ls + ... + \n";
                                        return false; // pred-edge: ... + ls + ... + -> mismatch
                                    }
                                }
                                // the last edge
                                int idx = omega_g_i.get_edge_property(path[j]);
                                int src_eq = omega_g_i.source(path[j]);
                                if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                    if (!neq_table[src_eq][dst] && !neq_table[src_eq][psi_P]) {
                                        logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                        return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                    }
                                }
							}
						}
				}
			} else {
				logger() << "omega graph is dag-like. \n";
				// dag_like (each cc has at most one cycle)

				// pred-atom math path in dag-like
				// cycle-join-nodes: cycle -> nodes-set
				// paths + cycle -> nodes
                std::set<int>& cycle = cc_cycle_set[cc_id];
                if (cycle.find(dst) != cycle.end()) {
						bool flag = false;
						for (int j=0; j<path.size(); j++) {
							int source = omega_g_i.source(path[j]);
							if (cycle.find(source) != cycle.end()) {
								cc_cycle_intersection[cc_id].insert(source);
								flag = true;
								break;
							}
						}
						if (flag) {
							cc_cycle_intersection[cc_id].insert(dst);
						}
                }

				 //
                if (m_ctx.division == "ACYCLIC-DLL") {
                    z3::expr psi_P = psi_atom.arg(1);
                    int tmp_i = expr_tool::index_of_exp(psi_P, m_ctx.phi_const_vec);
                    int psi_P_eq = m_ctx.phi_const_eq_class_vec[tmp_i];
					// if p is not allocated

					if (!allocated_table[psi_P_eq]) {
                        if (nil_set.find(dst) == nil_set.end()) {
                            int j=path.size()-1;
                            for (; j>0; j--) {
                                int idx = omega_g_i.get_edge_property(path[j]);
                                int src_eq = omega_g_i.source(path[j]);
                                // int dst_eq = omega_g_i.target(path[j]);
                                if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                    if (!neq_table[src_eq][psi_P]) {
                                        logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                        return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                    }
                                } else {
                                    logger() << "psi pred: ... + ls + ... + \n";
                                    return false; // pred-edge: ... + ls + ... + -> mismatch
                                }
                            }
                            // the last edge
                            int idx = omega_g_i.get_edge_property(path[j]);
                            int src_eq = omega_g_i.source(path[j]);
                            if (expr_tool::is_fun(m_ctx.phi_space.arg(idx), "pto")) {
                                if (!neq_table[src_eq][psi_P]) {
                                    logger() << "psi pred: pto+pto+pto: incomplete neq relations.\n";
                                    return false; // pto + ... + (pto) -> incomplete neq relations -> match
                                }
                            }
                        }
					}
                }
			}
		}
		for (int j=0; j<path.size(); j++) {
			omega_edge_table[omega_g_i.get_edge_property(path[j])] ++;
		}
	}
	//
	if (psi_edge_vec.size() > 0) {
        logger() << "cycle intersection: \n";

        for (int i=0; i<cc_cycle_intersection.size(); i++) {
            for (auto j : cc_cycle_intersection[i]) {
                int idx = eq_to_const_vec[j][0];
                logger() << "  " << m_ctx.phi_const_vec[idx];
            }
            logger() << std::endl;
            // check
            if (cc_cycle_num[i] > 0) {
                std::set<int>::iterator it1 = cc_cycle_intersection[i].begin();
                std::set<int>::iterator it = cc_cycle_intersection[i].end();
                std::set<int>::iterator it2;
                bool pass = false;
                for (; !pass && it1 != it; it1++) {
                        it2 = it1;
                        it2++;
                    for (; !pass && it2 != it; it2++) {
                        logger() << "    " << *it1 << " " << *it2 << std::endl;
                        if (neq_table[*it1][*it2]) {
                            pass = true;
                        }
                    }
                }
                if (!pass) {
                    logger() << "intersection: no eq relations\n";
                    return false;
                }
            }
        }

	}

	logger() << "cover omega edge table: \n";
	for (int i=0; i<omega_edge_vec.size(); i++) {
			logger() << omega_edge_table[omega_edge_vec[i].second] << " ";

			if (omega_edge_table[omega_edge_vec[i].second] == -1) {
				return false;
			}
	}
	logger() << std::endl;

	return true;
}


z3::expr alistsolver::pred_unfold_ge1(z3::expr& atom, int i, z3::expr_vector& new_bools) {
	std::string pred_name = atom.decl().name().str();
	int index = index_of_pred(pred_name);
	predicate pred = m_ctx.get_pred(index); // get predicate definition
	int size = atom.num_args() - pred.size_of_static_parameters(); // size of source and destination paramaters

	z3::expr_vector neq_items(z3_ctx());
	for (int i=0; i<size/2; i++) {
		z3::expr src = z3_ctx().int_const(atom.arg(i).to_string().c_str());
		z3::expr dst = z3_ctx().int_const(atom.arg(size/2+i).to_string().c_str());
		neq_items.push_back(src != dst);
	}


	z3::expr or_ge1 = listsolver::pred_unfold_ge1(atom, i, new_bools);

	return z3::mk_and(neq_items) && or_ge1;
}
