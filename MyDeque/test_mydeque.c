#include "test_mydeque.h"
#include "mydeque.h"
#include <assert.h>
#include <string.h>

void test_deque_create()
{
    Deque queue;
    deque_create(&queue);
    assert(queue.size == 0
        && queue.buffer != NULL
        && queue.buffer_end - queue.buffer == 50
        && queue.data_end == queue.data_start
        && queue.data_end == queue.buffer
        && queue.initial_size == 50
        && queue.increment == 1);
    deque_destroy(&queue);
    deque_tune(&queue, 25, 40);
    deque_create(&queue);
    assert(queue.size == 0
        && queue.buffer != NULL
        && queue.buffer_end - queue.buffer == 25
        && queue.data_end == queue.data_start
        && queue.data_end == queue.buffer
        && queue.initial_size == 25
        && queue.increment == 40);
    deque_destroy(&queue);
}

void test_deque_destroy()
{
    Deque queue;
    deque_create(&queue);
    deque_destroy(&queue);
    assert(queue.size == 0
        && queue.buffer == NULL
        && queue.buffer_end == queue.buffer
        && queue.data_end == queue.data_start
        && queue.data_end == queue.buffer);
}


void test_deque_add_first()
{
    Deque queue;
    deque_create(&queue);
    deque_add_first(&queue, "1");
    assert(strcmp(deque_peek_first(&queue), "1") == 0);
    deque_add_first(&queue, "2");
    assert(strcmp(deque_peek_first(&queue), "2") == 0);
    deque_add_first(&queue, "3");
    assert(strcmp(deque_peek_first(&queue), "3") == 0);
    deque_destroy(&queue);
}

void test_deque_add_last()
{
    Deque queue;
    deque_create(&queue);
    deque_add_last(&queue, "1");
    assert(strcmp(deque_peek_last(&queue), "1") == 0);
    deque_add_last(&queue, "2");
    assert(strcmp(deque_peek_last(&queue), "2") == 0);
    deque_add_last(&queue, "3");
    assert(strcmp(deque_peek_last(&queue), "3") == 0);
    deque_destroy(&queue);
}

void test_deque_size()
{
    Deque queue;
    deque_create(&queue);
    assert(deque_size(&queue) == 0);
    deque_add_last(&queue, "1");
    assert(deque_size(&queue) == 1);
    deque_add_last(&queue, "2");
    assert(deque_size(&queue) == 2);
    deque_remove_first(&queue);
    assert(deque_size(&queue) == 1);
    deque_add_last(&queue, "3");
    assert(deque_size(&queue) == 2);
    deque_destroy(&queue);
}

void test_deque_remove_first()
{
    Deque queue;
    deque_create(&queue);
    assert(deque_remove_first(&queue) == NULL);
    deque_add_last(&queue, "1");
    assert(strcmp((char*)deque_remove_first(&queue), "1") == 0);
    deque_add_last(&queue, "2");
    deque_add_last(&queue, "3");
    assert(strcmp((char*)deque_remove_first(&queue), "2") == 0);
    assert(strcmp((char*)deque_remove_first(&queue), "3") == 0);
    assert(deque_remove_first(&queue) == NULL);
    deque_destroy(&queue);
}

void test_deque_remove_last()
{
    Deque queue;
    deque_create(&queue);
    assert(deque_remove_first(&queue) == NULL);
    deque_add_last(&queue, "1");
    assert(strcmp((char*)deque_remove_last(&queue), "1") == 0);
    deque_add_last(&queue, "2");
    deque_add_last(&queue, "3");
    assert(strcmp((char*)deque_remove_last(&queue), "3") == 0);
    assert(strcmp((char*)deque_remove_last(&queue), "2") == 0);
    assert(deque_remove_first(&queue) == NULL);
    deque_destroy(&queue);
}

void test_deque_peek_first()
{
    Deque queue;
    deque_create(&queue);
    assert(deque_peek_first(&queue) == NULL);
    deque_add_last(&queue, "1");
    assert(strcmp((char*)deque_peek_first(&queue), "1") == 0);
    deque_add_last(&queue, "2");
    deque_add_last(&queue, "3");
    assert(strcmp((char*)deque_peek_first(&queue), "1") == 0);
    deque_remove_first(&queue);
    assert(strcmp((char*)deque_peek_first(&queue), "2") == 0);
    deque_remove_first(&queue);
    assert(strcmp((char*)deque_peek_first(&queue), "3") == 0);
    deque_remove_first(&queue);
    assert(deque_peek_first(&queue) == NULL);
    deque_destroy(&queue);
}

void test_deque_peek_last()
{
    Deque queue;
    deque_create(&queue);
    assert(deque_peek_last(&queue) == NULL);
    deque_add_last(&queue, "1");
    assert(strcmp((char*)deque_peek_last(&queue), "1") == 0);
    deque_add_last(&queue, "2");
    deque_add_last(&queue, "3");
    assert(strcmp((char*)deque_peek_last(&queue), "3") == 0);
    deque_remove_last(&queue);
    assert(strcmp((char*)deque_peek_last(&queue), "2") == 0);
    deque_remove_last(&queue);
    assert(strcmp((char*)deque_peek_last(&queue), "1") == 0);
    deque_remove_last(&queue);
    assert(deque_peek_first(&queue) == NULL);
    deque_destroy(&queue);
}

void test_deque_tune()
{
    Deque queue;
    deque_tune(&queue, 10, 0);
    assert(queue.initial_size == 10 && queue.increment == 0);
    deque_tune(&queue, 20, 5);
    assert(queue.initial_size == 20 && queue.increment == 5);
    deque_tune(&queue, 0, 10);
    assert(queue.initial_size == 0 && queue.increment == 10);
}
