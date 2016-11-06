#include <stdio.h>
#include <assert.h>
#include "myqueue.h"
#include "test_myqueue.h"

int main(void)
{
    test_deque_create();
    test_deque_destroy();
    test_deque_add_last();
    test_deque_size();
    test_deque_remove_first();
    test_deque_peek_first();
    test_deque_tune();
    test_deque_full();
    return 0;
}
