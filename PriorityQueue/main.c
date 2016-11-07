#include <stdio.h>
#include "test_priority_queue.h"

int main()
{
    test_pq_create();
    test_pq_insert();
    test_pq_delete_maximum();
    test_pq_is_empty();
    return 0;
}
