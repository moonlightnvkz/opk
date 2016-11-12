#include <stdio.h>
#include "test_rbtree.h"
int main()
{
    test_rb_create();

    test_rb_clear();

    test_rb_destroy();

    test_rb_size();

    test_rb_find();

    test_rb_insert();

    // test_rb_delete();

    test_rb_foreach();

    return 0;
}
