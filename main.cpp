#include <igraph.h>
#include <cstdio>
#include <cstdlib>

int main()
{
   igraph_real_t diameter;
   FILE* f = std::fopen("../amazon0302.txt", "r");
   if (!f)
   {
      std::perror("Open failed\n");
      return EXIT_FAILURE;
   }
   int size = 1024;
   igraph_t graph;
   igraph_bool_t b = false;

   // Get graph from file
   int error = igraph_read_graph_edgelist(&graph, f, 0, b);
   printf("There is %d edges in the graph\n", igraph_ecount(&graph));

   // Get greatest connected components.
   
   igraph_vector_ptr_t graph_list;
   //igraph_vector_t clust_size;
   igraph_vector_ptr_init(&graph_list, 50);
   //igraph_vector_init(&clust_size, 50);

   //igraph_integer_t nb_clusters;

   //igraph_clusters(&graph, &clusters, &clust_size, &nb_clusters, IGRAPH_STRONG);
   //printf("There are %d nb of clusters in the graph\n", nb_clusters);

   igraph_decompose(&graph, &graph_list, IGRAPH_STRONG, -1, 2);
   printf("There are %ld graph in list\n", igraph_vector_ptr_size(&graph_list));

   // Sort connexes subgraphs (ascending order ?)
   igraph_vector_ptr_sort(&graph_list, igraph_vector_colex_cmp);

   // Get largest connexe composant
   igraph_t *lcc_graph = (igraph_t *) igraph_vector_ptr_e(&graph_list, 0);

   // Free all others graph
   igraph_vector_ptr_resize(&graph_list, igraph_vector_ptr_size(&graph_list) - 1);
   igraph_decompose_destroy(&graph_list);

   printf("lcc_graph size = %d\n", igraph_ecount(lcc_graph));

   std::fclose(f);
   return 0;
}