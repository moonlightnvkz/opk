#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "test_slist.h"
#include "slist.h"

#define CORRUPT -1
#define OK 0

static int comp_each(SList *list, char **expect, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (list == NULL)
            return CORRUPT;
        if (!(list->data == NULL && expect[i] == NULL))
            if (list->data == NULL || strcmp(list->data, expect[i]) != 0)
                return CORRUPT;
        list = list->next;
    }
    return OK;
}

void test_slist_append()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    char *expected[] = {"head", "list1", "list2", NULL};
    assert(comp_each(list, expected, 4) == OK);

    slist_free(list);
}

void test_slist_prepend()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_prepend(list, "list1");
    list = slist_prepend(list, "list2");
    list = slist_prepend(list, NULL);
    char *expected[] = {NULL, "list2", "list1", "head"};
    assert(comp_each(list, expected, 4) == OK);

    slist_free(list);
}

void test_slist_insert()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    slist_insert(list, "list1");
    slist_insert(list, "list2");
    slist_insert(list->next, NULL);
    char *expected[] = {"head", "list2", NULL, "list1"};
    assert(comp_each(list, expected, 4) == OK);

    slist_free(list);
}

void test_slist_remove()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list2");
    list = slist_append(list, "list4");
    slist_remove(list, NULL);
    {
        char *expected[] = {"head", "list1", "list2", "list2", "list4"};
        assert(comp_each(list, expected, 5) == OK);
    }
    slist_remove(list, "list2");
    {
        char *expected[] = {"head", "list1", "list2", "list4"};
        assert(comp_each(list, expected, 4) == OK);
    }
    slist_remove(list, "list2");
    {
        char *expected[] = {"head", "list1", "list4"};
        assert(comp_each(list, expected, 3) == OK);
    }
    slist_remove(list, "list3");
    {
        char *expected[] = {"head", "list1", "list4"};
        assert(comp_each(list, expected, 3) == OK);
    }
    slist_free(list);
}

void test_slist_remove_all()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list2");
    list = slist_append(list, "list4");
    slist_remove_all(list, NULL);
    {
        char *expected[] = {"head", "list1", "list2", "list2", "list4"};
        assert(comp_each(list, expected, 5) == OK);
    }
    slist_remove_all(list, "list2");
    {
        char *expected[] = {"head", "list1", "list4"};
        assert(comp_each(list, expected, 3) == OK);
    }
    slist_free(list);
}

void test_slist_remove_next()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list2");
    list = slist_append(list, "list4");
    assert(strcmp(slist_remove_next(list), "list2"));
    {
        char *expected[] = {"head", "list2", NULL, "list2", "list4"};
        assert(comp_each(list, expected, 5) == OK);
    }
    assert(slist_remove_next(list->next) == NULL);
    {
        char *expected[] = {"head", "list2", "list2", "list4"};
        assert(comp_each(list, expected, 4) == OK);
    }
    assert(strcmp(slist_remove_next(list->next->next), "list2"));
    {
        char *expected[] = {"head", "list2", "list2"};
        assert(comp_each(list, expected, 3) == OK);
    }
    assert(slist_remove_next(list->next->next) == NULL);
    {
        char *expected[] = {"head", "list2", "list2"};
        assert(comp_each(list, expected, 3) == OK);
    }
    slist_free(list);
}

void test_slist_length()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    assert(slist_length(list) == 1);
    list = slist_append(list, "list1");
    assert(slist_length(list) == 2);
    list = slist_append(list, "list2");
    assert(slist_length(list) == 3);
    list = slist_append(list, NULL);
    assert(slist_length(list) == 4);
    slist_remove_next(list);
    assert(slist_length(list) == 3);
    list = slist_append(list, "list4");
    assert(slist_length(list) == 4);

    slist_free(list);
}

void test_slist_copy()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    {
        SList *copy = slist_copy(list);
        char *expected[] = {"head"};
        assert(comp_each(copy, expected, 1) == OK);
        slist_free(copy);
    }

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list2");
    list = slist_append(list, "list4");

    {
        SList *copy = slist_copy(list);
        char *expected[] = {"head", "list1", "list2", NULL, "list2", "list4"};
        assert(comp_each(copy, expected, 6) == OK);
        slist_free(copy);
    }
    {
        assert(slist_copy(NULL) == NULL);
    }

    slist_free(list);
}

void test_slist_reverse()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    SList *reversed;

    {
        reversed = slist_reverse(list);
        char *expected[] = {"head"};
        assert(comp_each(reversed, expected, 1) == OK);
    }

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list2");
    list = slist_append(list, "list4");

    {
        reversed = slist_reverse(list);
        char *expected[] = {"list4", "list2", NULL, "list2", "list1", "head"};
        assert(comp_each(reversed, expected, 6) == OK);
    }
    {
        assert(slist_copy(NULL) == NULL);
    }

    slist_free(reversed);
}

void test_slist_concat()
{
    SList *list1 = malloc(sizeof(SList));
    list1->data = "head1";
    list1->next = NULL;
    list1 = slist_append(list1, "list1");
    list1 = slist_append(list1, "list2");
    list1 = slist_append(list1, NULL);
    list1 = slist_append(list1, "list2");
    list1 = slist_append(list1, "list4");

    SList *list2 = malloc(sizeof(SList));
    list2->data = "head2";
    list2->next = NULL;
    list2 = slist_append(list2, "list1");
    list2 = slist_append(list2, "list2");
    list2 = slist_append(list2, NULL);
    list2 = slist_append(list2, "list2");
    list2 = slist_append(list2, "list4");
    {
        SList *concat = slist_concat(list1, NULL);
        char *expected[] = {"head1", "list1", "list2", NULL, "list2", "list4"};
        assert(comp_each(concat, expected, 6) == OK);
    }
    {
        SList *concat = slist_concat(NULL, list2);
        char *expected[] = {"head2", "list1", "list2", NULL, "list2", "list4"};
        assert(comp_each(concat, expected, 6) == OK);
    }
    {
        SList *concat = slist_concat(list1, list2);
        char *expected[] = {"head1", "list1", "list2", NULL, "list2", "list4",
                            "head2", "list1", "list2", NULL, "list2", "list4"};
        assert(comp_each(concat, expected, 12) == OK);
    }
    {
        assert(slist_concat(NULL, NULL) == NULL);
    }
    slist_free(list1);
}

void f_each(Pointer data, Pointer user_data)
{
    char *expected[] = {"head", "list1", "list2", NULL, "list2", "list4"};
    static int idx = 0;
    if (data == NULL)
    {
        if(expected[idx] != NULL)
        {
            assert(0);
        }
        else
        {
            ++idx;
            return;
        }
    }

    assert(strcmp(data, expected[idx]) == 0);
    ++idx;
}

void test_slist_foreach()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list2");
    list = slist_append(list, "list4");

    slist_foreach(list, f_each, "42");

    slist_foreach(NULL, f_each, NULL);
    slist_free(list);
}

void test_slist_last()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    SList *last;

    last = slist_last(list);
    assert(strcmp(last->data, "head") == 0);
    list = slist_append(list, "list1");
    last = slist_last(list);
    assert(strcmp(last->data, "list1") == 0);
    list = slist_append(list, "list2");
    last = slist_last(list);
    assert(strcmp(last->data, "list2") == 0);
    list = slist_append(list, NULL);
    last = slist_last(list);
    assert(last->data == NULL);
    list = slist_append(list, "list3");
    last = slist_last(list);
    assert(strcmp(last->data, "list3") == 0);
    list = slist_append(list, "list4");
    last = slist_last(list);
    assert(strcmp(last->data, "list4") == 0);

    slist_free(list);
}

void test_slist_nth()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    SList *nth;

    nth = slist_nth(list, 0);
    assert(strcmp(nth->data, "head") == 0);
    list = slist_append(list, "list1");
    nth = slist_nth(list, 1);
    assert(strcmp(nth->data, "list1") == 0);
    list = slist_append(list, "list2");
    nth = slist_nth(list, 2);
    assert(strcmp(nth->data, "list2") == 0);
    list = slist_append(list, NULL);
    nth = slist_nth(list, 3);
    assert(nth->data == NULL);
    list = slist_append(list, "list3");
    nth = slist_nth(list, 4);
    assert(strcmp(nth->data, "list3") == 0);
    list = slist_append(list, "list4");
    nth = slist_nth(list, 5);
    assert(strcmp(nth->data, "list4") == 0);

    nth = slist_nth(list, -1);
    assert(strcmp(nth->data, "list4") == 0);
    nth = slist_nth(list, -2);
    assert(strcmp(nth->data, "list3") == 0);
    nth = slist_nth(list, -3);
    assert(nth->data == NULL);
    nth = slist_nth(list, -4);
    assert(strcmp(nth->data, "list2") == 0);
    nth = slist_nth(list, -5);
    assert(strcmp(nth->data, "list1") == 0);
    nth = slist_nth(list, -6);
    assert(strcmp(nth->data, "head") == 0);

    slist_free(list);
}

void test_slist_find()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list3");
    list = slist_append(list, "list4");

    SList *found;
    found = slist_find(list, "head");
    assert(strcmp(found->data, "head") == 0);
    found = slist_find(list, "list1");
    assert(strcmp(found->data, "list1") == 0);
    found = slist_find(list, "list2");
    assert(strcmp(found->data, "list2") == 0);
    found = slist_find(list, NULL);
    assert(found->data == NULL);
    found = slist_find(list, "list3");
    assert(strcmp(found->data, "list3") == 0);
    found = slist_find(list, "list4");
    assert(strcmp(found->data, "list4") == 0);

    slist_free(list);
}

int comp(Pointer a, Pointer b)
{
    if (a == NULL && b == NULL)
        return 0;
    else if (a == NULL || b == NULL)
        return 1;
    else
        return strcmp(a, b);
}

void test_slist_find_custom()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    list = slist_append(list, "list1");
    list = slist_append(list, "list2");
    list = slist_append(list, NULL);
    list = slist_append(list, "list3");
    list = slist_append(list, "list4");

    SList *found;
    found = slist_find_custom(list, "head", comp);
    assert(strcmp(found->data, "head") == 0);
    found = slist_find_custom(list, "list1", comp);
    assert(strcmp(found->data, "list1") == 0);
    found = slist_find_custom(list, "list2", comp);
    assert(strcmp(found->data, "list2") == 0);
    found = slist_find_custom(list, NULL, comp);
    assert(found->data == NULL);
    found = slist_find_custom(list, "list3", comp);
    assert(strcmp(found->data, "list3") == 0);
    found = slist_find_custom(list, "list4", comp);
    assert(strcmp(found->data, "list4") == 0);
    found = slist_find_custom(list, "list5", comp);
    assert(found == NULL);

    slist_free(list);
}

void test_slist_position()
{
    SList *list = malloc(sizeof(SList));
    list->data = "head";
    list->next = NULL;

    SList *head = list;
    list = slist_append(list, "list1");
    SList *list1 = head->next;
    list = slist_append(list, "list2");
    SList *list2 = list1->next;
    list = slist_append(list, NULL);
    SList *null = list2->next;
    list = slist_append(list, "list3");
    SList *list3 = null->next;
    list = slist_append(list, "list4");
    SList *list4 = list3->next;

    int n;
    n = slist_position(list, head);
    assert(n == 0);
    n = slist_position(list, list1);
    assert(n == 1);
    n = slist_position(list, list2);
    assert(n == 2);
    n = slist_position(list, null);
    assert(n == 3);
    n = slist_position(list, list3);
    assert(n == 4);
    n = slist_position(list, list4);
    assert(n == 5);
    n = slist_position(list, list4 + 1);
    assert(n == -1);

    slist_free(list);
}
