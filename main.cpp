#include <igraph.h>
#include <stdio.h>

int main()
{
   igraph_real_t diameter;
   FILE* f = fopen("amazon0302.txt", "r");
   if (!f)
   {
      return 1;
   }
   int size = 1024;
   igraph_t graph;
   igraph_bool_t b = false;
   int error = igraph_read_graph_edgelist(&graph, f, 0, b);
   printf("There is %d edges in the graph\n", igraph_ecount(&graph));
   fclose(f);
   return 0;
}