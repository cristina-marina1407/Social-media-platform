
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define DIE(assertion, call_description)                       \
	do {                                                       \
		if (assertion) {                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);                          \
			exit(errno);                                       \
		}                                                      \
	} while (0)

queue_t *q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *que = malloc(1 * sizeof(queue_t));
	DIE(!que, "malloc failed\n");

	que->max_size = max_size;
	que->size = 0;
	que->data_size = data_size;
	que->read_idx = 0;
	que->write_idx = 0;
	que->buff = malloc(max_size * sizeof(*que->buff));
	DIE(!que->buff, "malloc failed\n");

	for (unsigned int i = 0; i < que->max_size; i++)
		que->buff[i] = NULL;

	return que;
}

unsigned int q_get_size(queue_t *q)
{
	return q->size;
}

unsigned int q_is_empty(queue_t *q)
{
	if (q->size == 0 || !q)
		return 1;
	return 0;
}

/* function returns the first element in the queue, without removing it */
void *q_front(queue_t *q)
{
	if (q->size > 0)
		return q->buff[q->read_idx];
	return NULL;
}

int q_dequeue(queue_t *q)
{
	if (!q || q->size == 0)
		return 0;

	if (q->size > 0) {
		if (q->buff[q->read_idx]) {
			free(q->buff[q->read_idx]);
			q->buff[q->read_idx] = NULL;
		}
		q->read_idx = (q->read_idx + 1) % q->max_size;
		q->size--;
		return 1;
	} else {
		return 0;
	}
}

int q_enqueue(queue_t *q, void *new_data)
{
	if (q->size < q->max_size) {
		if (!q->buff[q->write_idx]) {
			q->buff[q->write_idx] = malloc(q->data_size);
			DIE(!(q->buff[q->write_idx]), "malloc failed\n");
		}
		memcpy(q->buff[q->write_idx], new_data, q->data_size);
		q->write_idx = (q->write_idx + 1) % q->max_size;

		q->size++;
		return 1;
	}

	return 0;
}

void q_clear(queue_t *q)
{
	unsigned int i;
	if (!q || !q->size)
		return;

	for (i = 0; i < q->max_size; i++) {
		if (q->buff[i]) {
			free(q->buff[i]);
			q->buff[i] = NULL;
		}
	}
}

void q_free(queue_t *q)
{
	if (!q)
		return;
	q_clear(q);
	free(q->buff);
	free(q);
}
