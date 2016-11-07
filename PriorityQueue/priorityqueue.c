#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "priorityqueue.h"

static void* allocate_and_exit_if_failed(size_t size, const int line, const char *func)
{
    void* temp = malloc(size);
    if (!temp) {
        fprintf(stderr, "Err:Failed to allocate memory:%d:%s", line, func);
        exit(1);
    }
    return temp;
}

#define INITIAL_HEAP_SIZE 50
static void create_heap(Heap *ph)
{
    ph->data = allocate_and_exit_if_failed(sizeof(Pointer) * INITIAL_HEAP_SIZE, __LINE__, __FUNCTION__);
    ph->allocated = INITIAL_HEAP_SIZE;
    ph->size = 0;
}

void pq_create(PriorityQueue *pq, CmpFunc cmp)
{
    assert(pq != NULL);
    assert(cmp != NULL);
    pq->cmp_func = cmp;
    create_heap(&pq->heap);
}

void hp_destroy(Heap *hp)
{
    free(hp->data);
    hp->data = NULL;
    hp->allocated = hp->size = 0;
}

void pq_destroy(PriorityQueue *pq)
{
    hp_destroy(&pq->heap);
}

void hp_resize(Heap *ph, size_t size)
{
    Pointer *temp = realloc(ph->data, size * sizeof(Pointer));
    if (!temp) {
        fprintf(stderr, "Err:Failed to allocate memory:%d:%s", __LINE__, __FUNCTION__);
        exit(1);
    }
    ph->data = temp;
    ph->allocated = size;
}

void swap(Pointer *a, Pointer *b)
{
    Pointer temp = *a;
    *a = *b;
    *b = temp;
}

void sink(Pointer *data, size_t size, size_t i, CmpFunc cmp)
{
    assert(data != NULL);
    while (2 * (i + 1) - 1 < size)
    {
        int max_child_index;
        if (2 * (i + 1) - 1 == size - 1 || cmp(data[2 * (i + 1) - 1], data[2 * (i + 1)]) > 0)
            max_child_index = 2 * (i + 1) - 1;
        else
            max_child_index = 2 * (i + 1);
        if (cmp(data[i], data[max_child_index]) < 0)
        {
            swap(data + i, data + max_child_index);
            i = max_child_index;
        } else {
            break;
        }
    }
}

void bubble(Pointer *data, size_t idx, CmpFunc cmp)
{
    assert(data != NULL);
    while (cmp(data[idx], data[idx / 2]) > 0)
    {
        swap(data + idx, data + idx / 2);
        idx /= 2;
    }
}

void hp_insert(Heap *ph, Pointer value, CmpFunc cmp)
{
    if (ph->size == ph->allocated) {
        hp_resize(ph, ph->allocated * 2);
    }
    ph->data[ph->size] = value;
    ph->size++;
    bubble(ph->data, ph->size - 1, cmp);
}

void pq_insert(PriorityQueue *pq, Pointer value)
{
    hp_insert(&pq->heap, value, pq->cmp_func);
}

Pointer hp_delete_maximum(Heap *ph, CmpFunc cmp)
{
    assert(ph != NULL);
    if (ph->size == 0) {
        return NULL;
    }
    Pointer data = ph->data[0];
    ph->data[0] = ph->data[ph->size-1];
    ph->size--;
    if (ph->size <= ph->allocated / 4) {
        hp_resize(ph, ph->allocated / 2);
    }
    sink(ph->data, ph->size, 0, cmp);
    return data;
}

Pointer pq_delete_maximum(PriorityQueue *pq)
{
    assert(pq != NULL);
    Pointer data = hp_delete_maximum(&pq->heap, pq->cmp_func);
    return data;
}

int pq_is_empty(PriorityQueue *pq)
{
    return pq->heap.size != 0;
}
