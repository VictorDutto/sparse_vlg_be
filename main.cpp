#include "utils.hh"

#include <getopt.h>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <cstdlib>

int main(int argc, char *const argv[])
{

   int option_index = 0;
   static struct option long_options[] = {
      {"basic",    0, NULL,  0 },
      {"density",  0, NULL,  1 },
      {"degree",   0, NULL,  2 }
   };

   // Get parameter for strategies
   int opt = getopt_long(argc, argv, "", long_options, &option_index);
   
   if (opt == -1)
   {
      fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   igraph_real_t diameter;
   //graph format must be separated with whitespaces
   //this project is not meant to be a wrapper around igraph_read_graph_edgelist
   std::string graph_name("p2p-Gnutella04.txt");
   std::string arg("../graph/");
   arg = arg  + graph_name;
   printf_wrapper("%s\n", arg.c_str());
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
   printf_wrapper("There are %d edges in the graph\n", igraph_ecount(&graph));

   // Get greatest connected components.
   auto gcc = init_gcc(&graph);
   auto ecc_vect = calculate_eccentricity(gcc, option_index);
   size_t lgt = ecc_vect.size();
   for (size_t i = 0; i < lgt; i = 1 + i + 0.1 * lgt)
      printf_wrapper("This vector contains an eccentricity of value: %i\n", ecc_vect[i]);
   std::fclose(f);
   return 0;
}