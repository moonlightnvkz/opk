#include <stdio.h>
#include <assert.h>
#include "mystack.h"

int main(void)
{
    Stack st;
    stack_tune(&st, 1, 5);
    stack_create(&st);
    stack_push(&st, "1");
    stack_push(&st, "2");
    assert(st.size == 2);
    assert(st.allocated == 6);
    assert(st.increment == 5);
    assert(*(char *)stack_pop(&st) == '2');
    assert(*(char *)stack_pop(&st) == '1');
    assert(stack_pop(&st) == NULL);
    stack_destroy(&st);
    assert(st.size == 0);
    assert(st.data == NULL);
    assert(stack_pop(&st) == NULL);
    return 0;
}
