#ifndef FEED_H
#define FEED_H

#include "posts.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
 */

int friendship_verification(list_graph_t *graph, int user_id, int friend_id);
void feed(tree_array_t *tree_array, list_graph_t *graph, char *name,
		  int feed_size);
t_node_t *search_repost(t_node_t *root, char *name, int post_id,
						t_node_t *initial_root);
void view_profile(tree_array_t *tree_array, char *name);
void friends_repost(tree_array_t *tree_array, list_graph_t *graph, char *name,
					int post_id);
int all_are_friends(doubly_linked_list_t *list, list_graph_t *graph);
void print_clique(doubly_linked_list_t *clique);
void common_group(list_graph_t *graph, char *name);
void handle_input_feed(char *input, tree_array_t *tree_array,
					   list_graph_t *graph);

#endif // FEED_H
