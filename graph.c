
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

#define DIE(assertion, call_description)                       \
	do {                                                       \
		if (assertion) {                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);                          \
			exit(errno);                                       \
		}                                                      \
	} while (0)

list_graph_t *lg_create(int nodes)
{
	list_graph_t *graph = malloc(1 * sizeof(list_graph_t));
	DIE(!graph, "malloc failed\n");

	graph->nodes = nodes;
	graph->neighbors = malloc(nodes * sizeof(doubly_linked_list_t *));
	DIE(!graph->neighbors, "malloc failed\n");

	/* create the adjacency lists of the graph */
	for (int i = 0; i < nodes; i++)
		graph->neighbors[i] = dll_create(sizeof(int));

	return graph;
}

/* add an edge between the nodes received as parameters */
void lg_add_edge(list_graph_t *graph, int src, int dest)
{
	/* check if there is a connection so that I don't add more if there is */
	if (check_if_node_exist(graph->neighbors[src], dest) != -1)
		return;
	dll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dest);
}

/* returns 1 if there is an edge between the two nodes, 0 otherwise*/
int lg_has_edge(list_graph_t *graph, int src, int dest)
{
	if (check_if_node_exist(graph->neighbors[src], dest) != -1 ||
		check_if_node_exist(graph->neighbors[dest], src) != -1) {
		return 1;
	}
	return 0;
}

/* removes the edge between the nodes received as parameters */
void lg_remove_edge(list_graph_t *graph, int src, int dest)
{
	if (check_if_node_exist(graph->neighbors[src], dest) != -1) {
		int index = check_if_node_exist(graph->neighbors[src], dest);
		dll_node_t *removed = dll_remove_nth_node(graph->neighbors[src], index);
		free(removed->data);
		free(removed);
	}
}

/* free memory for the entire graph */
void lg_free(list_graph_t *graph)
{
	for (int i = 0; i < graph->nodes; i++)
		dll_free(&graph->neighbors[i]);
	free(graph->neighbors);
	free(graph);
}
