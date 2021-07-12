#include "utils.hh"

#include <string>
#include <cstdio>
#include <cstdlib>



int main()
{
   igraph_real_t diameter;
   //graph format must be separated with whitespaces
   //this project is not meant to be a wrapper around igraph_read_graph_edgelist
   std::string graph_name("p2p-Gnutella04.txt");
   std::string arg("../graph/");
   arg = arg  + graph_name;
   printf("%s\n", arg.c_str());
   FILE* f = std::fopen(arg.c_str(), "r");
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
   auto ecc_vect = calculate_eccentricity(gcc);
   size_t lgt = ecc_vect.size();
   for (size_t i = 0; i < lgt; i = 1 + i + 0.1 * lgt)
      printf("This vector contains an eccentricity of value: %i\n", ecc_vect[i]);
   std::fclose(f);
   return 0;
}