
/* DOUBLY LINKED LIST */

typedef struct dll_node_t dll_node_t;
struct dll_node_t
{
	void *data;
	dll_node_t *prev, *next;
};

typedef struct doubly_linked_list_t doubly_linked_list_t;
struct doubly_linked_list_t
{
	dll_node_t *head, *tail;
	unsigned int data_size;
	unsigned int size;
};

doubly_linked_list_t *dll_create(unsigned int data_size);
dll_node_t *dll_get_nth_node(doubly_linked_list_t *list, unsigned int n);
void dll_add_nth_node(doubly_linked_list_t *list, unsigned int n,
					  const void *data);
dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);
void dll_free(doubly_linked_list_t **pp_list);
int check_if_node_exist(doubly_linked_list_t *list, int value);
