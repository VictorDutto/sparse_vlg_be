#ifndef STRATEGY
#define STRATEGY

#include <vector>
#include <igraph.h>
#include <optional>

// in strategy.cpp
long int starting_ite_point(std::vector<int> got_ecc, std::optional<size_t> avg_pos = std::nullopt);

long int starting_ite_point_degree(std::vector<int> got_ecc,
				   igraph_t *g_c_component);

#endif