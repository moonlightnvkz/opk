#include <stdio.h>
#include "test_avl_tree.h"

int main()
{
    test_avl_create();
    test_avl_insert();
    test_avl_clear();
    test_avl_destroy();
    test_avl_find();
    test_avl_size();
    test_avl_delete();
    test_avl_foreach();
    // test_avl_stress_test();  // too looong
    return 0;
}
