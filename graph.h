
#include "doubly_linked_list.h"

/* the graph is ORIENTED */

typedef struct
{
	/* adjacency lists of the graph */
	doubly_linked_list_t **neighbors;
	/* number of nodes in graph */
	int nodes;
} list_graph_t;

list_graph_t *lg_create(int nodes);
void lg_add_edge(list_graph_t *graph, int src, int dest);
int lg_has_edge(list_graph_t *graph, int src, int dest);
void lg_remove_edge(list_graph_t *graph, int src, int dest);
void lg_free(list_graph_t *graph);
