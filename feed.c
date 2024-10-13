#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"

int friendship_verification(list_graph_t *graph, int user_id, int friend_id)
{
	/* searching friend_id in the adjacency list of user_id */
	int res = check_if_node_exist(graph->neighbors[user_id], friend_id);

	/* if it is in the adjacency list of user_id */
	if (res != -1)
		return 1;
	/* if it is not in the list (they are not friends) */
	return 0;
}

/* FEED */
void feed(tree_array_t *tree_array, list_graph_t *graph, char *name,
		  int feed_size)
{
	int user_id = get_user_id(name);

	/* go through the tree array in descending order as the most recent ones
	are on the last positions of the tree array */
	for (int i = (int)tree_array->size - 1; i >= 0 && feed_size > 0; i--) {
		int friend_id =
		((data_in_node *)tree_array->posts[i]->root->data)->user_id;

		/* if they are friends, print it */
		if (friendship_verification(graph, user_id, friend_id) == 1 ||
			user_id == friend_id) {
			printf("%s: \"%s\"\n", get_user_name(friend_id),
				   ((data_in_node *)tree_array->posts[i]->root->data)->title);

			/* decrease until reach 0 to print as much as needed*/
			feed_size--;
		}
	}
}

t_node_t *search_repost(t_node_t *root, char *name, int post_id,
						t_node_t *initial_root)
{
	char *curr_name = get_user_name(((data_in_node *)root->data)->user_id);
	int curr_post_id = ((data_in_node *)root->data)->post_id;

	/* if found a repost made by the person given and it's not the main post */
	if (strcmp(curr_name, name) == 0 && curr_post_id != post_id) {
		printf("Reposted: \"%s\"\n",
			   ((data_in_node *)initial_root->data)->title);
		return root;
	}

	/* go through all children recursively to search for the person's reposts */
	t_node_t *node = NULL;
	for (unsigned int i = 0; i < root->nr_children; i++) {
		node = search_repost(root->children[i], name, post_id, initial_root);
		if (node)
			break;
	}
	return node;
}

/* VIEW PROFILE*/
void view_profile(tree_array_t *tree_array, char *name)
{
	int user_id = get_user_id(name);

	/* search in the main posts */
	for (unsigned int i = 0; i < tree_array->size; i++) {
		/* found the person in the main posts */
		if (((data_in_node *)tree_array->posts[i]->root->data)->user_id ==
			user_id)
			printf("Posted: \"%s\"\n",
				   ((data_in_node *)tree_array->posts[i]->root->data)->title);
	}

	/* recursively print the reposts */
	for (unsigned int i = 0; i < tree_array->size; i++) {
		int post_id =
		((data_in_node *)tree_array->posts[i]->root->data)->post_id;
		search_repost(tree_array->posts[i]->root, name, post_id,
					  tree_array->posts[i]->root);
	}
}

/* FRIENDS REPOST */
void friends_repost(tree_array_t *tree_array, list_graph_t *graph, char *name,
					int post_id)
{
	int user_id = get_user_id(name);

	/* search among the person's friends to see who reposted the post with
	post_id */
	dll_node_t *node = graph->neighbors[user_id]->head;
	while (node) {
		int friend_id = *((int *)node->data);

		/* get the index of post_id from the tree_array */
		int index = get_index_post_id(tree_array, post_id);

		/* iterate through the vector of post_id's children */
		unsigned int nr_children = tree_array->posts[index]->root->nr_children;
		for (unsigned int i = 0; i < nr_children; i++) {
			int child_id =
			((data_in_node *)tree_array->posts[index]->root->children[i]->data)
			->user_id;

			/* check if the current child is also a friend */
			if (child_id == friend_id)
				printf("%s\n", get_user_name(child_id));
		}

		/* go to the next friend */
		node = node->next;
	}
}

int all_are_friends(doubly_linked_list_t *list, list_graph_t *graph)
{
	/* go through the friend list of each friend of the name that has 'list' as
	the adjacent list */
	dll_node_t *friend_list_node = list->head;
	while (friend_list_node->next) {
		/* go through the rest of the list from the current friend */
		for (dll_node_t *curr = friend_list_node->next; curr;
			 curr = curr->next) {
			/* if they are not friends, return 0 */
			if (friendship_verification(graph,
										*((int *)friend_list_node->data),
										*((int *)curr->data)) == 0) {
				return 0;
			}
		}

		/* go to the next friend */
		friend_list_node = friend_list_node->next;
	}

	/* if reached here, all are friends with each other */
	return 1;
}

void print_clique(doubly_linked_list_t *clique)
{
	dll_node_t *node = clique->head;
	while (node) {
		char *name = get_user_name(*((int *)node->data));
		printf("%s\n", name);
		node = node->next;
	}
}

/* CLIQUE */
void common_group(list_graph_t *graph, char *name)
{
	int user_id = get_user_id(name);

	/* create the clique */
	doubly_linked_list_t *clique = dll_create(sizeof(int));

	/* go through the friends list of the requested person */
	dll_node_t *node = graph->neighbors[user_id]->head;
	while (node) {
		int curr_id = *((int *)node->data);

		/* check if they are all friends with each other from the current
		friend list of 'name' and update the clique only if it's bigger than
		the current clique */
		if (all_are_friends(graph->neighbors[curr_id], graph) == 1 &&
			clique->size < graph->neighbors[curr_id]->size + 1) {
			/* update the clique (put the entire list of the person which forms
			a clique in order) */
			dll_free(&clique);
			clique = dll_create(sizeof(int));

			/* add the one in whose list we are looking */
			add_in_order(clique, curr_id);

			/* add his friends */
			dll_node_t *aux = graph->neighbors[curr_id]->head;
			while (aux) {
				add_in_order(clique, *((int *)aux->data));
				aux = aux->next;
			}
		}

		node = node->next;
	}

	/* found the clique */
	printf("The closest friend group of %s is:\n", name);
	print_clique(clique);
	dll_free(&clique);
}

void handle_input_feed(char *input, tree_array_t *tree_array,
					   list_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name = strtok(NULL, "\n ");
		char *feed_size_string = strtok(NULL, "\n ");
		int feed_size = atoi(feed_size_string);
		feed(tree_array, graph, name, feed_size);

	} else if (!strcmp(cmd, "view-profile")) {
		char *name = strtok(NULL, "\n ");
		view_profile(tree_array, name);

	} else if (!strcmp(cmd, "friends-repost")) {
		char *name = strtok(NULL, "\n ");
		char *post_id_string = strtok(NULL, "\n ");
		int post_id = atoi(post_id_string);
		friends_repost(tree_array, graph, name, post_id);

	} else if (!strcmp(cmd, "common-group")) {
		char *name = strtok(NULL, "\n ");
		common_group(graph, name);
	}
	free(commands);
}
