//static libs
#include "utils.hh"

//c dyn libs
#include <getopt.h>
#include <unistd.h>

//c++ dyn libs
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>

int main(int argc, char *const argv[])
{
   printf_wrapper("Bounding Eccentricities algorithm\n");
   int opt, option_index;
   int strategy = -1;

   std::string graph_name = "p2p-Gnutella30.txt";

   static struct option long_options[] = 
   {
      {"basic",    0, NULL,  0},
      {"density",  0, NULL,  1},
      {"degreemin", 0, NULL, 2},
      {"degreemax", 0, NULL, 3},
      {"delta",     0, NULL, 4},
      {"file",      1, NULL, 5}
   };

   // Get parameter for strategies and assign file
   while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
   {
      if (option_index == 5)
         graph_name.assign(optarg);
      else
         strategy = option_index;
      //printf_wrapper("No argument given, will proceed to a multi approach resolution\n");
      //printf_wrapper("Can call a specific method with: %s [-t nsecs] [-n] name\n", argv[0]);
   }

   igraph_real_t diameter;
   //graph format must be separated with whitespaces
   //this project is not meant to be a wrapper around igraph_read_graph_edgelist
   std::string arg("../graph/");
   arg = arg  + graph_name;
   printf_wrapper("Using file: %s\n\n", arg.c_str());
   FILE* f = std::fopen(arg.c_str(), "r");
  
   if (!f)
   {
      std::perror("Open failed\n");
      return EXIT_FAILURE;
   }
   igraph_t graph;
   igraph_bool_t b = false;

   // Get graph from file
   int error = igraph_read_graph_edgelist(&graph, f, 0, b);

   // Get greatest connected components.
   auto gcc = init_gcc(&graph);
   auto ecc_vect = calculate_eccentricity(gcc, strategy);
  
   std::fclose(f);
   f = std::fopen("ecc_vect.data", "w");
   if (!f)
   {
      std::perror("Cannot write eccentricity vector in a file\n");
      return EXIT_FAILURE;
   }
   std::fwrite(ecc_vect.data(), sizeof ecc_vect[0], ecc_vect.size(), f);
   std::fclose(f);
   std::ofstream file;
   file.open("ecc_vect.txt");
   for (size_t i = 0; i < ecc_vect.size(); i++)
       file << ecc_vect[i] << std::endl;
   file.close();
   return 0;
}