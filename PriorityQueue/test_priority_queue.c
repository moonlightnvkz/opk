#include <assert.h>
#include <string.h>
#include "test_priority_queue.h"
#include "priorityqueue.h"

static int cmp(Pointer data1, Pointer data2)
{
    // If data1 == data2 == NULL or it is the same piece of memory
    if (((size_t)data1 ^ (size_t)data2) == 0) {
        return 0;
    }
    return strcmp(data1, data2);   // To remove compiler warning
}

void test_pq_create()
{
    PriorityQueue q;
    PriorityQueue *pq = &q;
    pq_create(pq, cmp);
    assert(pq->cmp_func != NULL);
    assert(pq->heap.allocated == 50);
    assert(pq->heap.data != NULL);
    assert(pq->heap.size == 0);
}

void test_pq_insert()
{

    PriorityQueue q;
    PriorityQueue *pq = &q;
    pq_create(pq, cmp);
    pq_insert(pq, "1");
    assert(pq->heap.size == 1);
    assert(cmp(pq->heap.data[pq->heap.size - 1], "1") == 0);
    pq_insert(pq, "2");
    assert(pq->heap.size == 2);
    assert(cmp(pq->heap.data[0], "2") == 0);
    assert(cmp(pq->heap.data[pq->heap.size - 1], "1") == 0);

}

void test_pq_delete_maximum()
{
    PriorityQueue q;
    PriorityQueue *pq = &q;
    pq_create(pq, cmp);
    pq_insert(pq, "1");
    assert(cmp(pq_delete_maximum(pq), "1") == 0);
    pq_insert(pq, "2");
    assert(cmp(pq_delete_maximum(pq), "2") == 0);
    pq_insert(pq, "3");
    pq_insert(pq, "4");
    pq_insert(pq, "5");
    assert(cmp(pq_delete_maximum(pq), "5") == 0);
    assert(cmp(pq_delete_maximum(pq), "4") == 0);
    assert(cmp(pq_delete_maximum(pq), "3") == 0);
    assert(pq_delete_maximum(pq) == NULL);
}

void test_pq_is_empty()
{
    PriorityQueue q;
    PriorityQueue *pq = &q;
    pq_create(pq, cmp);
    assert(pq_is_empty(pq) == 0);
    pq_insert(pq, "1");
    assert(pq_is_empty(pq) == 1);
    pq_insert(pq, "2");
    assert(pq_is_empty(pq) == 1);
    pq_delete_maximum(pq);
    pq_delete_maximum(pq);
    assert(pq_is_empty(pq) == 0);
}
