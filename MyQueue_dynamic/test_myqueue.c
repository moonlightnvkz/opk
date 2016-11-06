#include "test_myqueue.h"
#include "myqueue.h"
#include <assert.h>
#include <string.h>

void test_deque_create()
{
    Queue queue;
    queue_create(&queue);
    assert(queue.allocated == 50
        && queue.size == 0
        && queue.buffer != NULL
        && queue.buffer_end == queue.buffer + queue.allocated
        && queue.data_end == queue.data_start
        && queue.data_end == queue.buffer
        && queue.initial_size == 50
        && queue.increment == 1);
    queue_destroy(&queue);
    queue_tune(&queue, 25, 40);
    queue_create(&queue);
    assert(queue.allocated == 25
        && queue.size == 0
        && queue.buffer != NULL
        && queue.buffer_end == queue.buffer + queue.allocated
        && queue.data_end == queue.data_start
        && queue.data_end == queue.buffer
        && queue.initial_size == 25
        && queue.increment == 40);
    queue_destroy(&queue);
}

void test_deque_destroy()
{
    Queue queue;
    queue_create(&queue);
    queue_destroy(&queue);
    assert(queue.allocated == 0
        && queue.size == 0
        && queue.buffer == NULL
        && queue.buffer_end == queue.buffer
        && queue.data_end == queue.data_start
        && queue.data_end == queue.buffer);
}

void test_deque_add_last()
{
    Queue queue;
    queue_create(&queue);
    queue_enqueue(&queue, "1");
    assert(strcmp(((char**)queue.data_start)[0], "1") == 0);
    queue_enqueue(&queue, "2");
    assert(strcmp(((char**)queue.data_start)[1], "2") == 0);
    queue_enqueue(&queue, "3");
    assert(strcmp(((char**)queue.data_start)[2], "3") == 0);
    queue_destroy(&queue);
}

void test_deque_size()
{
    Queue queue;
    queue_create(&queue);
    assert(queue_size(&queue) == 0);
    queue_enqueue(&queue, "1");
    assert(queue_size(&queue) == 1);
    queue_enqueue(&queue, "2");
    assert(queue_size(&queue) == 2);
    queue_dequeue(&queue);
    assert(queue_size(&queue) == 1);
    queue_enqueue(&queue, "3");
    assert(queue_size(&queue) == 2);
    queue_destroy(&queue);
}

void test_deque_remove_first()
{
    Queue queue;
    queue_create(&queue);
    assert(queue_dequeue(&queue) == NULL);
    queue_enqueue(&queue, "1");
    assert(strcmp((char*)queue_dequeue(&queue), "1") == 0);
    queue_enqueue(&queue, "2");
    queue_enqueue(&queue, "3");
    assert(strcmp((char*)queue_dequeue(&queue), "2") == 0);
    assert(strcmp((char*)queue_dequeue(&queue), "3") == 0);
    assert(queue_dequeue(&queue) == NULL);
    queue_destroy(&queue);
}

void test_deque_peek_first()
{
    Queue queue;
    queue_create(&queue);
    assert(queue_peek(&queue) == NULL);
    queue_enqueue(&queue, "1");
    assert(strcmp((char*)queue_peek(&queue), "1") == 0);
    queue_enqueue(&queue, "2");
    queue_enqueue(&queue, "3");
    assert(strcmp((char*)queue_peek(&queue), "1") == 0);
    queue_dequeue(&queue);
    assert(strcmp((char*)queue_peek(&queue), "2") == 0);
    queue_dequeue(&queue);
    assert(strcmp((char*)queue_peek(&queue), "3") == 0);
    queue_dequeue(&queue);
    assert(queue_peek(&queue) == NULL);
    queue_destroy(&queue);
}

void test_deque_tune()
{
    Queue queue;
    queue_tune(&queue, 10, 0);
    assert(queue.initial_size == 10 && queue.increment == 0);
    queue_tune(&queue, 20, 5);
    assert(queue.initial_size == 20 && queue.increment == 5);
    queue_tune(&queue, 0, 10);
    assert(queue.initial_size == 0 && queue.increment == 10);
}

void test_deque_full()
{
    Queue queue;
    assert(queue_full(&queue) == 1);
    queue_tune(&queue, 1, 2);
    assert(queue_full(&queue) == 1);
    queue_create(&queue);
    assert(queue_full(&queue) == 0);
    queue_enqueue(&queue, "1");
    assert(queue_full(&queue) == 1);
    queue_enqueue(&queue, "2");
    assert(queue_full(&queue) == 0);
    queue_destroy(&queue);
}
