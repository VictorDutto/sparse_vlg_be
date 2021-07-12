#include "utils.hh"


#include <cstdio>
#include <cstdlib>
#include <vector>
#include <numeric>
#include <limits>



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
   calculate_eccentricity(gcc);
   std::fclose(f);


      return 0;
}