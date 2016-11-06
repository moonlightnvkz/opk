#include <stdio.h>
#include "test_mydeque.h"

int main()
{
    test_deque_create();
    test_deque_destroy();
    test_deque_add_first();
    test_deque_add_last();
    test_deque_remove_first();
    test_deque_remove_last();
    test_deque_peek_first();
    test_deque_peek_last();
    test_deque_size();
    test_deque_tune();
    return 0;
}
