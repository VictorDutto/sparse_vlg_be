//c static lib
#include "utils.hh"
#include "strategy.hh"

//c dyn lib
#include <unistd.h>
#include <stdarg.h>

//c++ dyn lib
#include <numeric>
#include <limits>


void printf_wrapper(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    if (isatty(0))
        vprintf(format, args); 
    
    va_end(args);
}

igraph_t *init_gcc(igraph_t *graph)
{
    igraph_vector_ptr_t graph_list;
    igraph_vector_ptr_init(&graph_list, 50);

    // Decompose in connexe subgraph
    igraph_decompose(graph, &graph_list, IGRAPH_STRONG, -1, 2);
    printf_wrapper("There are %ld graph in list\n", igraph_vector_ptr_size(&graph_list));

    // Sort connexes subgraphs (ascending order ?)
    igraph_vector_ptr_sort(&graph_list, igraph_vector_colex_cmp);

    // Get largest connexe composant
    igraph_t *lcc_graph = (igraph_t *) igraph_vector_ptr_e(&graph_list, 0);

    // Free all others graph
    igraph_vector_ptr_resize(&graph_list, igraph_vector_ptr_size(&graph_list) - 1);
    igraph_decompose_destroy(&graph_list);

    printf_wrapper("lcc_graph size = %d\n", igraph_ecount(lcc_graph));

    return lcc_graph;
}

std::vector<int> calculate_eccentricity(igraph_t *g_c_component, int opt_index)
{
    opt_index = opt_index;
    size_t nb_vertices = igraph_vcount(g_c_component);
    // Generates the vector of graph indexes
    // then fill it
    std::vector<int> got_eccentricity(nb_vertices, 0);
    //filled index referes to the vertice
    std::vector<int> ecc_vect(nb_vertices, 0);
    std::vector<int> upper_bound(nb_vertices, std::numeric_limits<int>::max());
    std::vector<int> lower_bound(nb_vertices, std::numeric_limits<int>::min());

    size_t pos_sum = 0;
    for (size_t cmp_ecc = 0; cmp_ecc < nb_vertices;)
    {
       //launch eccentricity computation routine
       igraph_matrix_t res;
       igraph_matrix_init(&res, 0, 0);
       
       //new starting point, depends on strategy
       long int index;
       size_t avg_pos =  cmp_ecc == 0 ? 0 : pos_sum / cmp_ecc;

       switch (opt_index){
        case 0:
            index = starting_ite_point(got_eccentricity);
            break;
        case 1:
            index = starting_ite_point(got_eccentricity, avg_pos);
            break;
        case 2:
            index = starting_ite_point(got_eccentricity); //TODO
            break;
        default:
            fprintf(stderr, "No strategy with this index\n");
       }
       
       //searching for the greatest value in the row_index row
       igraph_vector_t row_vect;
       igraph_shortest_paths(g_c_component, &res, igraph_vss_1(index), igraph_vss_all(), IGRAPH_ALL);
       igraph_vector_init(&row_vect, nb_vertices);
       igraph_matrix_get_row(&res, &row_vect, 0);
       ecc_vect[index] =  igraph_vector_max(&row_vect);
       for (size_t w = 0; w < nb_vertices; w++)
       {
           //elem being d(v, w)
           int elem = igraph_vector_e(&row_vect, w);
           lower_bound[w] = std::max(lower_bound[w], std::max(ecc_vect[index] - elem, elem));
           upper_bound[w] = std::min(upper_bound[w], ecc_vect[index] + elem);
           if (lower_bound[w] == upper_bound[w] && got_eccentricity[w] == 0)
           {
               ecc_vect[w] = lower_bound[w];
               got_eccentricity[w] = 1;
               cmp_ecc += 1;
               pos_sum += w; 
           }
       }
    }
    return ecc_vect;

}