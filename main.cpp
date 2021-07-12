#include "utils.hh"

#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>



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
   calculate_eccentricity(gcc);
   std::fclose(f);


      return 0;
}