#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

#include "graph.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/

void add_friends(list_graph_t *graph, char *name1, char *name2);
void remove_friends(list_graph_t *graph, char *name1, char *name2);
int get_min_path(list_graph_t *graph, int src, int dest);
void add_in_order(doubly_linked_list_t *list, int id);
void suggestions(list_graph_t *graph, char *name);
void common_friends(list_graph_t *graph, char *name1, char *name2);
void nr_friends(list_graph_t *graph, char *name);
void most_popular(list_graph_t *graph, char *name);
void handle_input_friends(char *input, list_graph_t *graph);

#endif // FRIENDS_H
