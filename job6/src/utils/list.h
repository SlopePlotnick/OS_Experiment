#ifndef _UTILS_LIST_H
#define _UTILS_LIST_H

typedef struct node {
    struct node *next, *prev;
    void *data;
} node_t;

typedef node_t list_t;

#define list_each(this, node, item)                                   \
    for (node = (this)->next;                                         \
         item = node->data, node != (this);                           \
         node = node->next)

#define list_each_reverse(this, node, item)                           \
    for (node = (this)->prev;                                         \
         item = node->data, node != (this);                           \
         node = node->prev)

#define list_each_safe(this, curr, next, item)                        \
    for (curr = (this)->next, next = curr->next;                      \
         item = curr->data, curr != (this);                           \
         curr = next, next = curr->next)

extern node_t *node_new(void *data);
extern void node_delete(node_t *this);

extern int node_is_attached(node_t *this);
extern int node_is_detached(node_t *this);
extern void node_unlink(node_t *this);
extern void node_prepend(node_t *this, node_t *that);
extern void node_append(node_t *this, node_t *that);

extern void list_init(list_t *this);
extern void list_destroy(list_t *this);
extern int list_is_empty(list_t *this);

extern void *list_get_head(list_t *this);
extern void *list_get_tail(list_t *this);

extern node_t *list_push_head(list_t *this, void *data);
extern void *list_pop_head(list_t *this);

extern node_t *list_push_tail(list_t *this, void *data);
extern void *list_pop_tail(list_t *this);

extern int list_count(list_t *this);
extern void *list_get(list_t *this, int index);

#endif
