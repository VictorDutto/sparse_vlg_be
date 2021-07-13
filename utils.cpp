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


//sort the index according to the degrees
void custom_qs(igraph_vector_t *degree_map, std::vector<igraph_real_t> index_vect, size_t low, size_t high)
{
    if (low < high)
    {
        auto pi = low;
        auto i = low;
        auto j = high;
        while (i < j)
        {
            while (igraph_vector_e(degree_map, i) <= igraph_vector_e(degree_map, pi) && i < high)
                i++;
            while (igraph_vector_e(degree_map, j) > igraph_vector_e(degree_map, pi))
                j--;
            if (i < j)
            {
                igraph_vector_swap_elements(degree_map, i, j);
                auto tmp2 = index_vect[i];
                index_vect[i] = index_vect[j];
                index_vect[j] = tmp2;
            }
        }

        igraph_vector_swap_elements(degree_map, pi, j);
        auto tmp2 = index_vect[pi];
        index_vect[pi] = index_vect[j];
        index_vect[j] = tmp2;
        custom_qs(degree_map, index_vect, low, j - 1);
        custom_qs(degree_map, index_vect, j + 1, high);
    }
}

igraph_vector_t get_highest_degree(igraph_t *g_c_component, size_t nb_vertices)
{
    igraph_vector_t degree_map;
    igraph_vector_init(&degree_map, nb_vertices);
    //compute the degree for each vertex
    igraph_degree(g_c_component, &degree_map, igraph_vss_all(), IGRAPH_ALL, false);
    //match index to degrees
    std::vector<igraph_real_t> index_vect(nb_vertices, 0);
    std::iota(index_vect.begin(), index_vect.end(), 0);
    //quicksort index_vect using degree_map values
    custom_qs(&degree_map, index_vect, 0, nb_vertices - 1);
    const igraph_real_t *c_arr = &index_vect[0];
    igraph_vector_t sorted_by_degree;
    igraph_vector_init_copy(&sorted_by_degree, c_arr, nb_vertices);
    return sorted_by_degree;

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

    if (opt_index == 2)
        igraph_vector_t prio_vect = get_highest_degree(g_c_component, nb_vertices);
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
            index = starting_ite_point(got_eccentricity);
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
       igraph_vector_destroy(&row_vect);
       igraph_matrix_destroy(&res);
    }
    return ecc_vect;

}