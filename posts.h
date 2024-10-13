#ifndef POSTS_H
#define POSTS_H

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
 */

#include "friends.h"
#include "queue.h"

typedef struct data_in_node data_in_node;
struct data_in_node
{
	int post_id;
	char title[281];
	int user_id;
	// likes?
	doubly_linked_list_t *likes;
};

typedef struct t_node_t t_node_t;
struct t_node_t
{
	/* array of children */
	t_node_t **children;
	unsigned int nr_children;
	/* the number of reposts we allocated memory for */
	unsigned int allocated_reposts;
	/* data contained by the node */
	void *data;
};

typedef struct tree_t tree_t;
struct tree_t
{
	/* root of the tree */
	t_node_t *root;

	/* number of reposts of a tree */
	unsigned int nr_reposts;

	/* size of the data contained by the nodes */
	size_t data_size;
};

typedef struct tree_array_t tree_array_t;
struct tree_array_t
{
	tree_t **posts;
	unsigned int size;
	int curr_id;
	unsigned int allocated_size;
};

t_node_t *t_node_create(void *data, int data_size);
tree_t *tree_create(int data_size);
tree_array_t *tree_array_create(void);

int get_index_post_id(tree_array_t *tree_array, int post_id);
t_node_t *search_children(t_node_t *root, int repost_id);
int get_index_repost_id(t_node_t *root, int repost_id);
t_node_t *find_parent_of_repost(t_node_t *root, t_node_t *target,
								t_node_t *parent);

void create_post(tree_array_t *tree_array, char *name, char *title);
void create_repost(tree_array_t *tree_array, char *name, int index,
				   int repost_id);
void print_post(t_node_t *root, int post_id);
void get_reposts(tree_array_t *tree_array, int post_id, int repost_id,
				 int index);
void like_function(tree_array_t *tree_array, char *name, int post_id,
				   int repost_id);
void get_likes(tree_array_t *tree_array, int post_id, int repost_id);
int is_ancestor(t_node_t *root, t_node_t *node);
t_node_t *find_ancestor(t_node_t *root, t_node_t *r1, t_node_t *r2);
void first_common_repost(tree_array_t *tree_array, int post_id,
						 int repost_id1, int repost_id2);
t_node_t *get_max_likes_repost(t_node_t *root);
void ratio(tree_array_t *tree_array, int post_id);

void __tree_array_free(t_node_t *t_node);
void tree_array_free(tree_array_t *tree_array);

void delete(tree_array_t *tree_array, int post_id, int repost_id);
void extract_input(char *post_id_string, char *repost_id_string, int *post_id,
				   int *repost_id);

void handle_input_posts(char *input, tree_array_t *tree_array);

#endif // POSTS_H
