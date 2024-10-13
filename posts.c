#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "users.h"
#include "posts.h"

#define START_NR_POSTS 10
#define START_NR_REPOSTS 10

#define DIE(assertion, call_description)  \
	do                                    \
	{                                     \
		if (assertion)                    \
		{                                 \
			fprintf(stderr, "(%s, %d): ", \
					__FILE__, __LINE__);  \
			perror(call_description);     \
			exit(errno);                  \
		}                                 \
	} while (0)

t_node_t *t_node_create(void *data, int data_size)
{
	t_node_t *node = malloc(sizeof(t_node_t));
	DIE(!node, "t_node malloc");

	node->allocated_reposts = START_NR_REPOSTS;
	node->children = malloc(node->allocated_reposts * sizeof(t_node_t *));
	DIE(!node->children, "t_node malloc");
	node->nr_children = 0;
	node->data = malloc(data_size);
	DIE(!node->data, "t_node->data malloc");
	memcpy(node->data, data, data_size);

	return node;
}

tree_t *tree_create(int data_size)
{
	tree_t *tree = malloc(1 * sizeof(tree_t));
	DIE(!tree, "malloc failed\n");
	tree->data_size = data_size;
	tree->root = NULL;
	tree->nr_reposts = 0;
	return tree;
}

tree_array_t *tree_array_create(void)
{
	tree_array_t *tree_array = malloc(1 * sizeof(tree_array_t));
	DIE(!tree_array, "malloc failed\n");

	tree_array->size = 0;
	tree_array->curr_id = 1;
	tree_array->allocated_size = START_NR_POSTS;

	/* allocate memory for the array of trees (a tree = a post) */
	tree_array->posts = malloc(tree_array->allocated_size * sizeof(tree_t *));
	DIE(!tree_array->posts, "malloc failed\n");

	return tree_array;
}

/* returns the index of a main post from the tree_array */
int get_index_post_id(tree_array_t *tree_array, int post_id)
{
	int index = 0;
	for (unsigned int i = 0; i < tree_array->size; i++) {
		if (((data_in_node *)tree_array->posts[i]->root->data)->post_id ==
			post_id)
			break;
		index++;
	}
	return index;
}

/* search recursively and return the node of a subtree with the specified
repost_id */
t_node_t *search_children(t_node_t *root, int repost_id)
{
	if (repost_id == ((data_in_node *)root->data)->post_id)
		return root;

	t_node_t *node = NULL;
	for (unsigned int i = 0; i < root->nr_children; i++) {
		node = search_children(root->children[i], repost_id);
		if (node)
			break;
	}
	return node;
}

/* returns the index of a child from the root's array of children */
int get_index_repost_id(t_node_t *root, int repost_id)
{
	int index = 0;
	for (unsigned int i = 0; i < root->nr_children; i++) {
		if (((data_in_node *)root->children[i]->data)->post_id == repost_id)
			break;
		index++;
	}
	return index;
}

/* CREATE A POST */
void create_post(tree_array_t *tree_array, char *name, char *title)
{
	tree_array->size++;

	/* check if there is enough memory allocated */
	if (tree_array->allocated_size < tree_array->size) {
		/* reallocate memory if there is not enough */
		tree_array->allocated_size = 2 * tree_array->allocated_size;
		tree_array->posts = realloc(tree_array->posts,
									tree_array->allocated_size *
									sizeof(tree_t *));
	}

	/* create the post in the tree_array on the last position */
	unsigned int index = tree_array->size - 1;
	tree_array->posts[index] = tree_create(sizeof(data_in_node));

	/* create the root of the post */
	data_in_node *data = malloc(sizeof(data_in_node));
	DIE(!data, "malloc failed\n");
	data->post_id = tree_array->curr_id;
	data->user_id = get_user_id(name);
	strcpy(data->title, title);
	data->likes = dll_create(sizeof(int));

	tree_array->posts[index]->root = t_node_create(data, sizeof(data_in_node));
	free(data);

	/* increment curr_id with every new post */
	tree_array->curr_id++;

	printf("Created \"%s\" for %s\n", title, name);
}

/* CREATE A REPOST */
void create_repost(tree_array_t *tree_array, char *name, int index,
				   int repost_id)
{
	/* no repost_id, so add the repost to the root's children */
	if (repost_id == 0) {
		tree_array->posts[index]->root->nr_children++;

		/* reallocate memory if necessary for children */
		if (tree_array->posts[index]->root->allocated_reposts <
			tree_array->posts[index]->root->nr_children) {
			tree_array->posts[index]->root->allocated_reposts = 2 *
			tree_array->posts[index]->root->allocated_reposts;

			tree_array->posts[index]->root->children =
			realloc(tree_array->posts[index]->root->children,
					tree_array->posts[index]->root->allocated_reposts
					* sizeof(t_node_t *));
		}

		/* create the node to add to the root's children */
		data_in_node *data = malloc(sizeof(data_in_node));
		DIE(!data, "malloc failed\n");
		data->post_id = tree_array->curr_id;
		strcpy(data->title, "NULL");
		data->user_id = get_user_id(name);
		data->likes = dll_create(sizeof(int));
		t_node_t *new_node = t_node_create(data, sizeof(data_in_node));
		free(data);

		/* add the node to the root's children */
		int index_children = tree_array->posts[index]->root->nr_children - 1;
		tree_array->posts[index]->root->children[index_children] = new_node;

		/* increase the number of reposts of the tree (total number of nodes) */
		tree_array->posts[index]->nr_reposts++;

		printf("Created repost #%d for %s\n", tree_array->curr_id, name);

	} else {
		/* there is a repost_id so I need to add to a subtree of a tree */

		/* find the node where the repost_id is located (the parent) */
		t_node_t *repost_node = search_children(tree_array->posts[index]->root,
												repost_id);

		/* calculate the index where to add the new node (the repost) */
		int index_children = repost_node->nr_children;
		/* increase the number of children of the current node */
		repost_node->nr_children++;

		/* reallocate memory if necessary for children */
		if (repost_node->allocated_reposts < repost_node->nr_children) {
			repost_node->allocated_reposts = 2 * repost_node->allocated_reposts;

			repost_node->children =
			realloc(repost_node->children,
					repost_node->allocated_reposts * sizeof(t_node_t *));
		}

		/* create the node to add to the children array of the parent */
		data_in_node *data = malloc(sizeof(data_in_node));
		DIE(!data, "malloc failed\n");
		data->post_id = tree_array->curr_id;
		strcpy(data->title, "NULL");
		data->user_id = get_user_id(name);
		data->likes = dll_create(sizeof(int));
		t_node_t *new_node = t_node_create(data, sizeof(data_in_node));
		free(data);
		/* add child */
		repost_node->children[index_children] = new_node;

		/* increase the number of reposts of the tree (total number of nodes) */
		tree_array->posts[index]->nr_reposts++;

		printf("Created repost #%d for %s\n", tree_array->curr_id, name);
	}

	/* increment the curr_id with every new post/repost created */
	tree_array->curr_id++;
}

void print_post(t_node_t *root, int post_id)
{
	/* print everything except the main post (printed in get_posts) */
	if (post_id != ((data_in_node *)root->data)->post_id) {
		int user_id = ((data_in_node *)root->data)->user_id;
		printf("Repost #%d by %s\n", ((data_in_node *)root->data)->post_id,
			   get_user_name(user_id));
	}

	/* print recursively */
	for (unsigned int i = 0; i < root->nr_children; i++)
		print_post(root->children[i], post_id);
}

/* GET-REPOSTS */
void get_reposts(tree_array_t *tree_array, int post_id, int repost_id,
				 int index)
{
	/* print for a main post */
	if (repost_id == 0) {
		int user_id =
		((data_in_node *)tree_array->posts[index]->root->data)->user_id;
		printf("\"%s\" - Post by %s\n",
			   ((data_in_node *)tree_array->posts[index]->root->data)->title,
				get_user_name(user_id));

		print_post(tree_array->posts[index]->root, post_id);
		return;
	}

	/* print for a repost */
	t_node_t *new_root = search_children(tree_array->posts[index]->root,
										 repost_id);
	print_post(new_root, post_id);
}

/* LIKE */
void like_function(tree_array_t *tree_array, char *name, int post_id,
				   int repost_id)
{
	int index = get_index_post_id(tree_array, post_id);

	/* id of the user that needd to be added to the list of likes */
	int id = get_user_id(name);

	/* like at the main post */
	if (repost_id == 0) {
		/* if the like already exists in the list, remove the like */
		int like_index =
		check_if_node_exist(((data_in_node *)tree_array
							->posts[index]->root->data)->likes, id);
		if (like_index != -1) {
			dll_node_t *to_remove =
			dll_remove_nth_node(((data_in_node *)tree_array->posts[index]
								->root->data)->likes, like_index);
			free(to_remove->data);
			free(to_remove);
			to_remove = NULL;

			printf("User %s unliked post \"%s\"\n",
				   name, ((data_in_node *)tree_array->posts[index]->root->data)
				   ->title);
			return;
		}

		/* if there's no id of the person who gave like, add it at the end */
		int pos = ((data_in_node *)tree_array->posts[index]->root->data)
				   ->likes->size;
		/* add to the list of likes of the post */
		dll_add_nth_node(((data_in_node *)tree_array->posts[index]->root->data)
						 ->likes, pos, &id);

		printf("User %s liked post \"%s\"\n", name,
			   ((data_in_node *)tree_array->posts[index]->root->data)->title);
	} else {
		/* like to a repost */
		t_node_t *repost_node =
		search_children(tree_array->posts[index]->root,
						repost_id);

		/* check if the like already exists in the list */
		int like_index = check_if_node_exist(((data_in_node *)repost_node->data)
											 ->likes, id);
		/* if there is a like in the list, remove the like */
		if (like_index != -1) {
			dll_node_t *to_remove =
			dll_remove_nth_node(((data_in_node *)repost_node->data)->likes,
								like_index);
			free(to_remove->data);
			free(to_remove);
			to_remove = NULL;

			printf("User %s unliked repost \"%s\"\n", name,
				   ((data_in_node *)tree_array->posts[index]->root->data)
				   ->title);

			return;
		}

		/* if there is no id of the person who gave like, add it at the end */
		int pos = ((data_in_node *)repost_node->data)->likes->size;
		/* add to the list of likes of the repost */
		dll_add_nth_node(((data_in_node *)repost_node->data)->likes, pos, &id);

		printf("User %s liked repost \"%s\"\n", name,
			   ((data_in_node *)tree_array->posts[index]->root->data)->title);
	}
}

/* GET-LIKES */
void get_likes(tree_array_t *tree_array, int post_id, int repost_id)
{
	int index = get_index_post_id(tree_array, post_id);

	/* if it's main post */
	if (repost_id == 0) {
		int nr_likes =
		((data_in_node *)tree_array->posts[index]->root->data)->likes->size;

		printf("Post \"%s\" has %d likes\n",
			   ((data_in_node *)tree_array->posts[index]->root->data)->title,
			   nr_likes);
	} else {
		/* it's repost */
		t_node_t *repost_node = search_children(tree_array->posts[index]->root,
												repost_id);

		int nr_likes = ((data_in_node *)repost_node->data)->likes->size;

		printf("Repost #%d has %d likes\n", repost_id, nr_likes);
	}
}

/* returns 1 if root is an ancestor of node, and 0 otherwise */
int is_ancestor(t_node_t *root, t_node_t *node)
{
	if (!root)
		return 0;

	/* if it's the same node, the ancestor exists */
	if (((data_in_node *)root->data)->post_id ==
		((data_in_node *)node->data)->post_id) {
		return 1;
	}

	/* go recursively through all nodes */
	for (unsigned int i = 0; i < root->nr_children; i++) {
		if (is_ancestor(root->children[i], node) == 1)
			return 1;
	}

	/* if no ancestor of node was found */
	return 0;
}

/* returns the node which is the common ancestor of repost1 and repost2 */
t_node_t *find_ancestor(t_node_t *root, t_node_t *r1, t_node_t *r2)
{
	if (!root)
		return NULL;

	/* if the common ancestor is one of r1 and r2 */
	if (is_ancestor(r1, r2) == 1)
		return r1;
	if (is_ancestor(r2, r1) == 1)
		return r2;

	/* if I reached one of the initial nodes */
	if (((data_in_node *)root->data)->post_id ==
		((data_in_node *)r1->data)->post_id ||
		((data_in_node *)root->data)->post_id ==
		((data_in_node *)r2->data)->post_id)
		return root;

	t_node_t *aux = NULL;

	/* nr increases if the current root is ancestor to one of r1 or r2 */
	int nr = 0;

	/* go recursively through all children  */
	for (unsigned int i = 0; i < root->nr_children; i++) {
		t_node_t *res = find_ancestor(root->children[i], r1, r2);
		/* if res != NULL, then root is ancestor for r1 or r2 */
		if (res) {
			nr++;
			aux = res;
		}
	}

	/* if the common ancestor was found (it is for both r1 and r2) */
	if (nr == 2)
		return root;

	/* if the ancestor was not found yet */
	return aux;
}

/* FIRST COMMON REPOST */
void first_common_repost(tree_array_t *tree_array, int post_id,
						 int repost_id1, int repost_id2)
{
	int index = get_index_post_id(tree_array, post_id);
	t_node_t *root = tree_array->posts[index]->root;
	t_node_t *r1 = search_children(root, repost_id1);
	t_node_t *r2 = search_children(root, repost_id2);

	t_node_t *common_repost = find_ancestor(root, r1, r2);
	int id = ((data_in_node *)common_repost->data)->post_id;

	printf("The first common repost of %d and %d is %d\n",
		   repost_id1, repost_id2, id);
}

/* find the node with most likes from a tree (from a post) */
t_node_t *get_max_likes_repost(t_node_t *root)
{
	if (!root)
		return NULL;

	/* assume the node with most likes is the current root */
	t_node_t *max_node = root;

	for (unsigned int i = 0; i < root->nr_children; i++) {
		/* go recursively through all nodes of the tree */
		t_node_t *child_max_node = get_max_likes_repost(root->children[i]);

		if (child_max_node) {
			data_in_node *child_data = (data_in_node *)child_max_node->data;
			data_in_node *max_data = (data_in_node *)max_node->data;

			/* if the child node has more likes than the max_node or
			if they have the same number of likes, the max node becomes the
			node with the smallest id */
			if (child_data->likes->size > max_data->likes->size ||
				(child_data->likes->size == max_data->likes->size &&
				 child_data->post_id < max_data->post_id)) {
				max_node = child_max_node;
			}
		}
	}

	return max_node;
}

/* RATIO */
void ratio(tree_array_t *tree_array, int post_id)
{
	int index = get_index_post_id(tree_array, post_id);

	/* initialize the maximum with the number of likes of the post */
	t_node_t *max_repost =
	get_max_likes_repost(tree_array->posts[index]->root);

	/* if the main post is the maximum */
	if (((data_in_node *)max_repost->data)->post_id == post_id)
		printf("The original post is the highest rated\n");
	else
		printf("Post %d got ratio'd by repost %d\n", post_id,
			   ((data_in_node *)max_repost->data)->post_id);
}

/* FREE */
void __tree_array_free(t_node_t *t_node)
{
	if (!t_node)
		return;

	/* recursively free the array of children of each node */
	for (unsigned int i = 0; i < t_node->nr_children; i++)
		__tree_array_free(t_node->children[i]);

	/* free the list of likes of each node and the node itself */
	dll_free(&(((data_in_node *)t_node->data)->likes));
	free(t_node->data);
	free(t_node->children);
	free(t_node);
	t_node = NULL;
}

void tree_array_free(tree_array_t *tree_array)
{
	/* free each tree with all his children */
	for (unsigned int i = 0; i < tree_array->size; i++) {
		__tree_array_free(tree_array->posts[i]->root);
		free(tree_array->posts[i]);
	}

	free(tree_array->posts);
	free(tree_array);
}

/* function returns the parent node of a node (target) */
t_node_t *find_parent_of_repost(t_node_t *root, t_node_t *target,
								t_node_t *parent)
{
	if (!root)
		return NULL;

	/* if the parent is found */
	if (root == target)
		return parent;

	/* go recursively though all children */
	for (unsigned int i = 0; i < root->nr_children; i++) {
		/* the parent becomes the current root (search in his subtree) */
		t_node_t *result = find_parent_of_repost(root->children[i], target,
												 root);
		if (result)
			return result;
	}

	/* if there is no parent */
	return NULL;
}

/* DELETE */
void delete(tree_array_t *tree_array, int post_id, int repost_id)
{
	int index = get_index_post_id(tree_array, post_id);

	/* delete a main post (an entire tree) */
	if (repost_id == 0) {
		/* free memory for the entire tree */
		__tree_array_free(tree_array->posts[index]->root);
		free(tree_array->posts[index]);
		tree_array->posts[index] = NULL;

		/* move the elements of the tree array by an index further to left */
		for (unsigned int i = index; i < tree_array->size - 1; i++)
			tree_array->posts[i] = tree_array->posts[i + 1];
		tree_array->size--;

		printf("Deleted post \"%s\"\n",
			   ((data_in_node *)tree_array->posts[index]->root->data)->title);
	} else {
		/* delete a repost and its subtree */
		t_node_t *repost_node = search_children(tree_array->posts[index]->root,
												repost_id);

		/* need the parent node in order to move the position of its children
		after removing the repost_node */
		t_node_t *parent = NULL;
		parent = find_parent_of_repost(tree_array->posts[index]->root,
									   repost_node, parent);

		/* find the index of the node (which I delete) in the array of its
		parent's children */
		int index_repost_node = get_index_repost_id(parent, repost_id);

		/* delete the node and its entire subtree */
		__tree_array_free(repost_node);

		/* move the nodes with an index further to left */
		for (int i = index_repost_node; i < (int)parent->nr_children - 1; i++)
			parent->children[i] = parent->children[i + 1];

		parent->nr_children--;

		printf("Deleted repost #%d of post \"%s\"\n", repost_id,
			   ((data_in_node *)tree_array->posts[index]->root->data)->title);
	}
}

void extract_input(char *post_id_string, char *repost_id_string, int *post_id,
				   int *repost_id)
{
	*post_id = atoi(post_id_string);
	if (repost_id_string)
		*repost_id = atoi(repost_id_string);
	else
		*repost_id = 0;
}

void handle_input_posts(char *input, tree_array_t *tree_array)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *name = strtok(NULL, "\n ");
		char *title = strtok(NULL, "\"");
		create_post(tree_array, name, title);

	} else if (!strcmp(cmd, "repost")) {
		char *name = strtok(NULL, "\n ");
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id_string = strtok(NULL, "\n ");
		int post_id, repost_id;
		extract_input(post_id_string, repost_id_string, &post_id, &repost_id);
		int index = get_index_post_id(tree_array, post_id);
		create_repost(tree_array, name, index, repost_id);

	} else if (!strcmp(cmd, "common-repost")) {
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id1_string = strtok(NULL, "\n ");
		char *repost_id2_string = strtok(NULL, "\n ");
		int post_id = atoi(post_id_string);
		int repost_id1 = atoi(repost_id1_string);
		int repost_id2 = atoi(repost_id2_string);
		first_common_repost(tree_array, post_id, repost_id1, repost_id2);

	} else if (!strcmp(cmd, "like")) {
		char *name = strtok(NULL, "\n ");
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id_string = strtok(NULL, "\n ");
		int post_id, repost_id;
		extract_input(post_id_string, repost_id_string, &post_id, &repost_id);
		like_function(tree_array, name, post_id, repost_id);

	} else if (!strcmp(cmd, "ratio")) {
		char *post_id_string = strtok(NULL, "\n ");
		int post_id = atoi(post_id_string);
		ratio(tree_array, post_id);

	} else if (!strcmp(cmd, "delete")) {
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id_string = strtok(NULL, "\n ");
		int post_id, repost_id;
		extract_input(post_id_string, repost_id_string, &post_id, &repost_id);
		delete(tree_array, post_id, repost_id);

	} else if (!strcmp(cmd, "get-likes")) {
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id_string = strtok(NULL, "\n ");
		int post_id, repost_id;
		extract_input(post_id_string, repost_id_string, &post_id, &repost_id);
		get_likes(tree_array, post_id, repost_id);

	} else if (!strcmp(cmd, "get-reposts")) {
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id_string = strtok(NULL, "\n ");
		int post_id, repost_id;
		extract_input(post_id_string, repost_id_string, &post_id, &repost_id);
		int index = get_index_post_id(tree_array, post_id);
		get_reposts(tree_array, post_id, repost_id, index);
	}
	free(commands);
}

