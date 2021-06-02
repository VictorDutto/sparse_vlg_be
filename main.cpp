#include <igraph.h>

int main()
{
   igraph_real_t diameter;
   FILE* f = fopen("amazon0302.txt", "R");
   igraph_t *graph;
   igraph_bool_t b = false;
   igraph_read_graph_edgelist(graph, f, 262110, b);
   fclose(f);
   return 0;
}