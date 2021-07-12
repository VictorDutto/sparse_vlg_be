#include "utils.hh"

#include <numeric>
#include <limits>

igraph_t *init_gcc(igraph_t *graph)
{
    igraph_vector_ptr_t graph_list;
    igraph_vector_ptr_init(&graph_list, 50);

    // Decompose in connexe subgraph
    igraph_decompose(graph, &graph_list, IGRAPH_STRONG, -1, 2);
    printf("There are %ld graph in list\n", igraph_vector_ptr_size(&graph_list));

    // Sort connexes subgraphs (ascending order ?)
    igraph_vector_ptr_sort(&graph_list, igraph_vector_colex_cmp);

    // Get largest connexe composant
    igraph_t *lcc_graph = (igraph_t *) igraph_vector_ptr_e(&graph_list, 0);

    // Free all others graph
    igraph_vector_ptr_resize(&graph_list, igraph_vector_ptr_size(&graph_list) - 1);
    igraph_decompose_destroy(&graph_list);

    printf("lcc_graph size = %d\n", igraph_ecount(lcc_graph));

    return lcc_graph;
}


std::vector<int> calculate_eccentricity(igraph_t *g_c_component)
{
   size_t nb_vertices = igraph_vcount(g_c_component);
   
   // Generates the vector of graph indexes
   // then fill it
   std::vector<unsigned int> index_vect(nb_vertices);
   std::iota(index_vect.begin(), index_vect.end(), 0);
   //filled index referes to the vertice
   std::vector<int> ecc_vect(nb_vertices, 0);
   std::vector<int> upper_bound(nb_vertices, std::numeric_limits<int>::max());
   std::vector<int> lower_bound(nb_vertices, std::numeric_limits<int>::min());

   //launch eccentricity computation routine

   igraph_vector_t uninit_vect;
   igraph_matrix_t res;
   igraph_matrix_init(&res, 0, 0);
   //basic shortest_path
   igraph_shortest_paths(g_c_component, &res, igraph_vss_1(5), igraph_vss_1(5), IGRAPH_ALL);

   // select vertice
   // compute eccentricity
   //propagate information
   return ecc_vect;

}