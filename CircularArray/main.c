#include <stdio.h>
#include <assert.h>
#include <circularbuffer.h>

int main(void)
{
    {
        CircularBuffer cb;
        cb_tune(&cb, 1, 1);
        cb_create(&cb);
        assert(cb_pop(&cb) == NULL);
        cb_push(&cb, "0");
        assert(cb_full(&cb) == 1);
        cb_push(&cb, "1");
        assert(cb_full(&cb) == 1);
        assert(*(char*)cb_pop(&cb) == '0');
        assert(*(char*)cb_pop(&cb) == '1');
        assert(cb_pop(&cb) == NULL);
        cb_push(&cb, "2");
        assert(*(char*)cb_pop(&cb) == '2');
        cb_destroy(&cb);
    }
    {
        CircularBuffer cb;
        cb_tune(&cb, 3, 5);
        cb_create(&cb);
        cb_push(&cb, "1");
        cb_push(&cb, "2");
        cb_push(&cb, "3");
        cb_push(&cb, "4");
        cb_push(&cb, "5");
        cb_pop(&cb);
        cb_pop(&cb);
        assert(cb_size(&cb) == 3);
        assert(cb.allocated == 8);
        cb_pop(&cb);
        assert(cb_size(&cb) == 2);
        cb_pop(&cb);
        assert(cb.allocated == 3);
        cb_destroy(&cb);
    }
    return 0;
}
