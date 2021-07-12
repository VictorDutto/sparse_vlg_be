#include <igraph.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <limits>

#include "utils.hh"


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
   auto gcc = init_gcc(&graph);
   size_t nb_vertices = igraph_vcount(&graph);
   
   // Generates the vector of graph indexes
   // then fill it
   std::vector<unsigned int> index_vect(nb_vertices);
   std::iota(index_vect.begin(), index_vect.end(), 0);
   //filled index referes to the vertice
   std::vector<int> ecc_vect(nb_vertices, 0);
   std::vector<int> upper_bound(nb_vertices, std::numeric_limits<int>::max());
   std::vector<int> lower_bound(nb_vertices, std::numeric_limits<int>::min());

   //launch eccentricity computation routine
   std::fclose(f);


      return 0;
}