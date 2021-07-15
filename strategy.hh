#ifndef STRATEGY
#define STRATEGY

#include <vector>
#include <igraph.h>
#include <optional>

// in strategy.cpp
long int starting_ite_point(std::vector<int> got_ecc, std::optional<size_t> avg_pos = std::nullopt);


long int starting_ite_point_degree(std::vector<int> got_ecc,
                                   igraph_vector_t prio_vect);

long int starting_ite_point_all(std::vector<int> got_ecc, size_t avg_pos, igraph_vector_t prio_vect);

#endif