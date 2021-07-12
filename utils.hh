#ifndef UTILS
#define UTILS
#include <igraph.h>
#include <vector>

igraph_t *init_gcc(igraph_t *graph);

std::vector<int> calculate_eccentricity(igraph_t *g_c_component);

#endif