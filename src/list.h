#ifndef LIST_H
#define LIST_H

typedef struct node_t {
	char str[1024];
	struct node_t *next;
} node_t;

node_t *node_init(void);
node_t *node_add(node_t * src, char *str);
node_t *node_insert(node_t * src, int index, char *str);
void node_delete_all(node_t * src);
node_t *node_delete(node_t * src);
void node_print(node_t * src);
int node_count(node_t * src);
int node_is_exist(node_t * src, char *str);

#endif
