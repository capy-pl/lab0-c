#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    /* Free queue structure */

    while (q->head) {
        list_ele_t *temp;
        temp = q->head;
        q->head = q->head->next;
        free(temp->value);
        free(temp);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return NULL;
    }
    newh->value = malloc((strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s));
    newh->value[strlen(s)] = '\0';
    if (q->size == 0) {
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newt = malloc(sizeof(list_ele_t));

    if (!newt) {
        return false;
    }

    newt->value = malloc((strlen(s) + 1));

    if (!newt->value) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s));
    newt->value[strlen(s)] = '\0';
    newt->next = NULL;
    if (q->size == 0) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *temp;
    if (!q || !q->head) {
        return false;
    }
    if (sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    temp = q->head;
    q->head = q->head->next;
    free(temp->value);
    free(temp);
    q->size--;
    if (q->size == 0) {
        q->head = NULL;
    }
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }
    list_ele_t *current = q->head;
    list_ele_t *prev, *temp = NULL;

    while (current) {
        temp = current->next;
        current->next = prev;
        prev = current;
        current = temp;
    }
    q->tail = q->head;
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2) {
        return;
    }

    queue_t left;
    queue_t right;
    q_split(q, &left, &right);

    q_sort(&left);
    q_sort(&right);
    q_merge(&left, &right, q);
}

void q_split(queue_t *q, queue_t *left, queue_t *right)
{
    list_ele_t *temp;
    left->size = (q->size >> 1) + (q->size & 1);
    right->size = q->size >> 1;

    left->head = q->head;
    right->tail = q->tail;

    temp = left->head;
    for (int i = 0; i < left->size - 1; i++) {
        temp = temp->next;
    }
    left->tail = temp;
    right->head = temp->next;
    left->tail->next = right->tail->next = NULL;
    q->head = q->tail = NULL;
}

void q_push(queue_t *target, list_ele_t *element)
{
    if (!target->head) {
        target->head = target->tail = element;
    } else {
        target->tail->next = element;
        target->tail = element;
    }
}

void q_merge(queue_t *left, queue_t *right, queue_t *q)
{
    q->size = left->size + right->size;

    while (left->head || right->head) {
        if (!right->head ||
            (left->head &&
             strcasecmp(left->head->value, right->head->value) < 0)) {
            q_push(q, left->head);
            left->head = left->head->next;
        } else {
            q_push(q, right->head);
            right->head = right->head->next;
        }
    }
    q->tail->next = NULL;
}
