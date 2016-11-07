#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include <stdlib.h>

typedef void *Pointer;
typedef int (*CmpFunc)(Pointer data1, Pointer data2);

typedef struct tHeap{
    Pointer *data;
    size_t size;
    size_t allocated;
} Heap;

typedef struct tPriorityQueue {
    Heap heap;
    CmpFunc cmp_func;
} PriorityQueue;

/* Create empty priority queue. */
void pq_create(PriorityQueue *pq, CmpFunc cmp);

/* Destroy priority queue and free its memory. */
void pq_destroy(PriorityQueue *pq);

/* Add new value into the priority queue. */
void pq_insert(PriorityQueue *pq, Pointer value);

/* Delete maximum value from the priority queue and return it. */
Pointer pq_delete_maximum(PriorityQueue *pq);

/* Return non-zero value if the priority queue is not empty. */
int pq_is_empty(PriorityQueue *pq);

#endif // PRIORITYQUEUE_H
