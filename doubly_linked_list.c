
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doubly_linked_list.h"

#define DIE(assertion, call_description)                       \
	do {                                                       \
		if (assertion) {                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);                          \
			exit(errno);                                       \
		}                                                      \
	} while (0)

/* DOUBLY LINKED LIST */

doubly_linked_list_t *dll_create(unsigned int data_size)
{
	doubly_linked_list_t *list = malloc(sizeof(doubly_linked_list_t));
	DIE(!list, "malloc failed\n");
	list->data_size = data_size;
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
}

dll_node_t *dll_get_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	if (n >= list->size)
		n = n % list->size;

	if (n == 0)
		return list->head;

	unsigned int count = 0;
	dll_node_t *curr = list->head;
	while (curr) {
		if (count == n)
			return curr;
		count++;
		curr = curr->next;
	}
	return NULL;
}

void dll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
					  const void *data)
{
	/* add on first position */
	if (!list->head) {
		dll_node_t *new = malloc(sizeof(dll_node_t));
		DIE(!new, "malloc failed\n");

		new->data = malloc(list->data_size);
		DIE(!new->data, "malloc failed\n");
		memcpy(new->data, data, list->data_size);

		list->head = new;
		new->next = NULL;
		list->head->prev = NULL;
		list->tail = new;
		list->size++;
		return;
	}

	if (n == 0) {
		dll_node_t *new = malloc(1 * sizeof(dll_node_t));
		DIE(!new, "malloc failed\n");

		new->data = malloc(list->data_size);
		DIE(!new->data, "malloc failed\n");
		memcpy(new->data, data, list->data_size);

		new->next = list->head;
		new->prev = NULL;
		list->head->prev = new;
		list->head = new;
		list->size++;
		return;
	}

	/* add to end */
	if (n >= list->size) {
		dll_node_t *tail = list->tail;
		dll_node_t *new = malloc(1 * sizeof(dll_node_t));
		DIE(!new, "malloc failed\n");

		new->data = malloc(list->data_size);
		DIE(!new->data, "malloc failed\n");
		memcpy(new->data, data, list->data_size);

		tail->next = new;
		new->prev = tail;
		new->next = NULL;
		list->tail = new;
		list->size++;
		return;
	}

	/* add in the middle */
	unsigned int contor = 0;
	dll_node_t *curr_node = list->head;
	while (curr_node->next) {
		if (contor == n - 1) {
			dll_node_t *new = malloc(1 * sizeof(dll_node_t));
			DIE(!new, "malloc failed\n");

			new->data = malloc(list->data_size);
			DIE(!new->data, "malloc failed\n");
			memcpy(new->data, data, list->data_size);

			new->next = curr_node->next;
			new->prev = curr_node;
			curr_node->next = new;
			(new->next)->prev = new;
			list->size++;
			return;
		}
		/* go to next */
		contor++;
		curr_node = curr_node->next;
	}
}

dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	if (!list || list->size == 0 || n >= list->size)
		return NULL;

	/* remove first element when it's the only one */
	if (n == 0 && list->size == 1) {
		dll_node_t *aux = list->head;
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return aux;
	}

	/* remove first element */
	if (n == 0) {
		dll_node_t *aux = list->head;
		list->head = aux->next;
		list->head->prev = NULL;
		list->size--;
		return aux;
	}

	/* remove last element */
	if (n == list->size - 1) {
		dll_node_t *curr_node = list->tail->prev;
		dll_node_t *aux = list->tail;
		curr_node->next = NULL;
		list->tail = curr_node;
		list->size--;
		return aux;
	}

	dll_node_t *node = list->head;
	unsigned int contor = 0;
	while (node) {
		if (contor == list->size - 2)
			break;
		if (contor == n - 1) {
			/* modify connections */
			dll_node_t *aux = node->next;
			node->next = aux->next;
			aux->next->prev = node;
			list->size--;
			return aux;
		}

		contor++;
		node = node->next;
	}

	return NULL;
}

void dll_free(doubly_linked_list_t **pp_list)
{
	if (*pp_list) {
		dll_node_t *aux = (*pp_list)->head;
		while (aux) {
			(*pp_list)->head = aux->next;
			if (aux->data)
				free(aux->data);
			free(aux);
			aux = (*pp_list)->head;
		}

		free(*pp_list);
		*pp_list = NULL;
	}
}

/* returns -1 if the node does not exist and the index where it is located if
it exists */
int check_if_node_exist(doubly_linked_list_t *list, int value)
{
	int index = 0;
	dll_node_t *node = list->head;
	while (node) {
		if (*((int *)node->data) == value)
			return index;
		node = node->next;
		index++;
	}
	return -1;
}
