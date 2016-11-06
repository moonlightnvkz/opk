#include <stdio.h>
#include "test_slist.h"

int main()
{
    test_slist_append();
    test_slist_prepend();
    test_slist_insert();
    test_slist_remove();
    test_slist_remove_all();
    test_slist_remove_next();
    test_slist_length();
    test_slist_copy();
    test_slist_reverse();
    test_slist_concat();
    test_slist_foreach();
    test_slist_last();
    test_slist_nth();
    test_slist_find();
    test_slist_find_custom();
    test_slist_position();
    return 0;
}
