#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "friends.h"
#include "users.h"

#include "queue.h"

#define DIE(assertion, call_description)                       \
	do {                                                       \
		if (assertion) {                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);                          \
			exit(errno);                                       \
		}                                                      \
	} while (0)

/* ADD FRIENDS */
void add_friends(list_graph_t *graph, char *name1, char *name2)
{
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	lg_add_edge(graph, id1, id2);
	lg_add_edge(graph, id2, id1);
	printf("Added connection %s - %s\n", name1, name2);
}

/* REMOVE FRIENDS */
void remove_friends(list_graph_t *graph, char *name1, char *name2)
{
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);
	lg_remove_edge(graph, id1, id2);
	lg_remove_edge(graph, id2, id1);
	printf("Removed connection %s - %s\n", name1, name2);
}

/* GET MINIMUM DISTANCE */

typedef struct node_t
{
	int nr;
	int color;
	int dist;
	struct node_t *parent;
} node_t;

void initiate_nodes(node_t *nodes, int nr_nodes)
{
	for (int i = 0; i < nr_nodes; i++) {
		nodes[i].nr = i;
		nodes[i].color = 0;
		nodes[i].dist = 0;
		nodes[i].parent = NULL;
	}
}

int get_min_path(list_graph_t *graph, int src, int dest)
{
	queue_t *q = q_create(sizeof(int), graph->nodes);
	node_t *nodes = malloc(graph->nodes * sizeof(node_t));
	DIE(!nodes, "malloc failed\n");

	/* set all nodes as unexplored (color = 0) and the rest of the data */
	initiate_nodes(nodes, graph->nodes);

	/* mark the src node as visited */
	nodes[src].color = 1;
	nodes[src].dist = 0;

	/* introduce the first node in the queue */
	q_enqueue(q, &src);

	/* bfs for finding the distances between each node and src */
	/* as long as the queue is not empty (explore all nodes) */
	while (q_is_empty(q) == 0) {
		/* extract the node from front of the queue */
		int front_node = *((int *)q_front(q));
		q_dequeue(q);

		/* go through the neighbor list of the extracted node */
		dll_node_t *neighb = graph->neighbors[front_node]->head;
		while (neighb) {
			/* if the neighbor has not been visited */
			if (nodes[*((int *)neighb->data)].color == 0) {
				nodes[*((int *)neighb->data)].color = 1;
				nodes[*((int *)neighb->data)].parent = &nodes[front_node];
				/* update neighbor's distance from src */
				nodes[*((int *)neighb->data)].dist = nodes[front_node].dist + 1;
				/* add the neighbor to the queue to continue exploring */
				q_enqueue(q, (int *)neighb->data);
			}

			neighb = neighb->next;
		}
		/* when it has finished processing all neighbors of the current node,
		it moves on to exploring the neighbors of the next node in the queue */
	}

	/* create the route between src and dest */
	doubly_linked_list_t *route = dll_create(sizeof(int));
	int min_dist = nodes[dest].dist;
	int curr_dist = 0;
	node_t *curr_node = &nodes[dest];
	while (curr_dist < min_dist) {
		dll_add_nth_node(route, 0, &curr_node->nr);
		curr_node = curr_node->parent;
		curr_dist++;
	}

	/* if there is no path between src and dest */
	if (route->size == 0) {
		printf("There is no way to get from %s to %s\n", get_user_name(src),
			   get_user_name(dest));

		free(route);
		free(nodes);
		q_free(q);

		return -1;
	}

	/* if there is a path, I also add the node from which the route starts */
	dll_add_nth_node(route, 0, &src);

	/* the distance will be the number of edges in the route */
	int min_path = route->size - 1;

	dll_free(&route);
	free(nodes);
	q_free(q);

	return min_path;
}

/* function adds nodes to a list in ascending order of id */
void add_in_order(doubly_linked_list_t *list, int id)
{
	/* if it's not already in the list, add it */
	if (check_if_node_exist(list, id) == -1) {
		unsigned int i = 0;
		dll_node_t *current_node = list->head;
		while (i < list->size) {
			if (id < *((int *)current_node->data))
				break;
			i++;
			current_node = current_node->next;
		}
		dll_add_nth_node(list, i, &id);
	}
}

/* SUGGESTIONS */
void suggestions(list_graph_t *graph, char *name)
{
	doubly_linked_list_t *suggestion_list = dll_create(sizeof(int));
	int id = get_user_id(name);

	/* go through the list of id */
	dll_node_t *node = graph->neighbors[id]->head;
	while (node) {
		int id_friend = *((int *)node->data);

		/* go through the list of current friend & add it to suggestion_list */
		dll_node_t *friend_node = graph->neighbors[id_friend]->head;
		while (friend_node) {
			/* if it's not him and if he's not already friends with 'name' */
			if (*((int *)friend_node->data) != id &&
				check_if_node_exist(graph->neighbors[id],
									*((int *)friend_node->data)) == -1) {
				add_in_order(suggestion_list, *((int *)friend_node->data));
			}
			friend_node = friend_node->next;
		}

		node = node->next;
	}

	/* if there are no suggestions */
	if (suggestion_list->size == 0) {
		printf("There are no suggestions for %s\n", name);
		free(suggestion_list);
		return;
	}

	/* if there are suggestions, print the list */
	printf("Suggestions for %s:\n", name);
	dll_node_t *s_node = suggestion_list->head;
	while (s_node) {
		char *s_name = get_user_name(*((int *)s_node->data));
		printf("%s\n", s_name);
		s_node = s_node->next;
	}
	dll_free(&suggestion_list);
}

/* COMMON FRIENDS */
void common_friends(list_graph_t *graph, char *name1, char *name2)
{
	unsigned int i;
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);

	/* create the list of common friends */
	doubly_linked_list_t *common_friends = dll_create(sizeof(int));

	/* go through the friends list of the first one */
	dll_node_t *current_node1 = graph->neighbors[id1]->head;
	while (current_node1) {
		/* for each friend of the first, go through the list of the second */
		dll_node_t *current_node2 = graph->neighbors[id2]->head;

		while (current_node2) {
			/* if the current friend of the first is also a friend of the
			second, add him to the list sorted by id*/
			if (*((int *)current_node1->data) == *((int *)current_node2->data))
				add_in_order(common_friends, *((int *)current_node1->data));

			/* move on to the next friend of the second */
			current_node2 = current_node2->next;
		}

		/* move on to the next friend of the first */
		current_node1 = current_node1->next;
	}

	/* if no common friends were found (the list is empty) */
	if (common_friends->size == 0) {
		printf("No common friends for %s and %s\n", name1, name2);
		free(common_friends);
		return;
	}

	/* print common friends sorted by id */
	printf("The common friends between %s and %s are:\n", name1, name2);
	dll_node_t *current_node = common_friends->head;
	for (i = 0; i < common_friends->size; i++) {
		char *name = get_user_name(*((int *)current_node->data));
		printf("%s\n", name);
		current_node = current_node->next;
	}
	dll_free(&common_friends);
}

/* NUMBER OF FRIENDS */
void nr_friends(list_graph_t *graph, char *name)
{
	int id = get_user_id(name);
	printf("%s has %d friends\n", name, graph->neighbors[id]->size);
}

/* THE MOST POPULAR */
void most_popular(list_graph_t *graph, char *name)
{
	int id = get_user_id(name);
	/* number of friends of the person given in the input */
	int nr_friends_of_target = graph->neighbors[id]->size;

	/* initialize the results with those of the person in the input */
	int max_fr = graph->neighbors[id]->size;
	int max_id = id;

	/* go through the list of friends for the requested person */
	dll_node_t *node = graph->neighbors[id]->head;
	while (node) {
		int id_friend = *((int *)node->data);
		int nr_friends = graph->neighbors[id_friend]->size;

		if (nr_friends > max_fr) {
			max_fr = nr_friends;
			max_id = id_friend;
		} else if (nr_friends == max_fr) {
			/* if a friend has the same number of friends as the person given
			in the input, the person in the input is preferred */
			if (nr_friends == nr_friends_of_target) {
				max_fr = graph->neighbors[id]->size;
				max_id = id;
			} else {
				/* if there are 2 with the same number of friends, the one with
				the smaller id is taken */
				if (id_friend < max_id) {
					max_fr = nr_friends;
					max_id = id_friend;
				}
			}
		}

		node = node->next;
	}

	if (max_id == id) {
		printf("%s is the most popular\n", name);
	} else {
		printf("%s is the most popular friend of %s\n",
			   get_user_name(max_id), name);
	}
}

void handle_input_friends(char *input, list_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		add_friends(graph, name1, name2);

	} else if (!strcmp(cmd, "remove")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		remove_friends(graph, name1, name2);

	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n ");
		suggestions(graph, name);

	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		int src = get_user_id(name1);
		int dest = get_user_id(name2);
		int min_path = get_min_path(graph, src, dest);
		if (min_path != -1)
			printf("The distance between %s - %s is %d\n", name1, name2,
				   min_path);

	} else if (!strcmp(cmd, "common")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		common_friends(graph, name1, name2);

	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n ");
		nr_friends(graph, name);

	} else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n ");
		most_popular(graph, name);
	}

	free(commands);
}
