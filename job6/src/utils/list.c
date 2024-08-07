#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "list.h"

node_t *node_new(void *data)
{
    node_t *this = malloc(sizeof(node_t));
    this->next = NULL;
    this->prev = NULL;
    this->data = data;
    return this;
}

void node_delete(node_t *this)
{
    assert(this->next == NULL && this->prev == NULL);
    free(this);
}

int node_is_attached(node_t *this)
{
    return this->next != NULL;
}

int node_is_detached(node_t *this)
{
    return this->next == NULL;
}

void node_link(node_t *left, node_t *right)
{
    left->next = right;
    right->prev = left;
}

void node_link3(node_t *left, node_t *node, node_t *right)
{
    node_link(left, node);
    node_link(node, right);
}

void node_unlink(node_t *this)
{
    node_link(this->prev, this->next);
    this->next = NULL;
    this->prev = NULL;
}

void node_prepend(node_t *this, node_t *that)
{
    node_link3(this->prev, that, this);
}

void node_append(node_t *this, node_t *that)
{
    node_link3(this, that, this->next);
}

void list_init(list_t *this)
{
    this->next = this->prev = this;
}

void list_destroy(list_t *this)
{
}

int list_is_empty(list_t *this)
{
    return this->next == this;
}

void *list_get_head(list_t *this)
{
    assert(!list_is_empty(this));
    node_t *head = this->next;
    return head->data;
}

void *list_get_tail(list_t *this)
{
    assert(!list_is_empty(this));
    node_t *tail = this->prev;
    return tail->data;
}

node_t *list_push_head(list_t *this, void *data)
{
    node_t *node = node_new(data);
    node_link3(this, node, this->next);
    return node;
}

void *list_pop_head(list_t *this)
{
    assert(!list_is_empty(this));
    node_t *head = this->next;
    node_unlink(head);

    void *data = head->data;
    node_delete(head);
    return data;
}

node_t *list_push_tail(list_t *this, void *data)
{
    node_t *node = node_new(data);
    node_link3(this->prev, node, this);
    return node;
}

void *list_pop_tail(list_t *this)
{
    assert(!list_is_empty(this));
    node_t *tail = this->prev;
    node_unlink(tail);

    void *data = tail->data;
    node_delete(tail);
    return tail;
}

int list_count(list_t *this)
{
    int sum = 0;
    node_t *node;
    void *data;
    list_each (this, node, data)
        sum++;
    return sum;
}

void *list_get(list_t *this, int index)
{
    int i = 0;
    node_t *node;
    void *data;
    list_each (this, node, data) {
        if (i++ == index)
            return data;
    }

    assert(0);
    return NULL;
}
