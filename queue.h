
/* QUEUE */

typedef struct queue_t queue_t;
struct queue_t
{
	/* the maximum size of the queue */
	unsigned int max_size;
	/* the size of the queue */
	unsigned int size;
	/* the size in bytes of the data type stored in the queue */
	unsigned int data_size;
	/* the index from which front and dequeue operations will be performed */
	unsigned int read_idx;
	/* the index from which the enqueue operations will be performed */
	unsigned int write_idx;
	/* the buffer that stores the elements of the queue */
	void **buff;
};

queue_t *q_create(unsigned int data_size, unsigned int max_size);
unsigned int q_get_size(queue_t *q);
unsigned int q_is_empty(queue_t *q);
void *q_front(queue_t *q);
int q_dequeue(queue_t *q);
int q_enqueue(queue_t *q, void *new_data);
void q_clear(queue_t *q);
void q_free(queue_t *q);
