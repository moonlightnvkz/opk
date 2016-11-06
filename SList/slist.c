#include "slist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void bad_alloc_assert(void* p, const char* func, const char* file, int line)
{
    if (!p)
    {
        fprintf(stderr, "Error(code:1) at : %s (%s:%d)\n", func, file, line);
        exit(1);
    }
}

// FIXED: without loops
SList *slist_append(SList *list, Pointer data)
{
    // FIXED: slist_last
    SList *last = slist_last(list);
    slist_insert(last, data);
    return list;
}

SList *slist_prepend(SList *list, Pointer data)
{
    SList *temp = malloc(sizeof(SList));
    bad_alloc_assert(temp, __FUNCTION__, __FILE__, __LINE__);
    temp->data = data;
    temp->next = list;
    return temp;
}

void slist_insert(SList *sibling, Pointer data)
{
    // FIXED: use prepend
    assert(sibling != NULL);
    sibling->next = slist_prepend(sibling->next, data);
}

SList *slist_remove(SList *list, Pointer data)
{
    if (list == NULL)
        return list;
    if (list->data == data)
    {
        SList *temp = list->next;
        free(list);
        return temp;
    } else {
        list->next = slist_remove(list->next, data);
    }
    return list;
}

// FIXED: without loops
SList *slist_remove_all(SList *list, Pointer data)
{
    if (list == NULL)
        return list;
    SList *new_head;
    if (list->data == data)
    {
        new_head = list->next;
        free(list);
        new_head = slist_remove_all(new_head, data);
    } else {
        new_head = list;
        new_head->next = slist_remove_all(new_head->next, data);
    }
    return new_head;
}

Pointer slist_remove_next(SList *sibling)
{
    if (sibling == NULL || sibling->next == NULL)
        return NULL;
    SList *tmp = sibling->next;
    Pointer data = tmp->data;
    sibling->next = sibling->next->next;
    free(tmp);
    return data;
}

void slist_free(SList *list)
{
    if (list == NULL)
        return;
    while (list->next != NULL)
    {
        SList *tmp = list;
        list = list->next;
        free(tmp);
    }
    free(list);
}

unsigned slist_length(SList *list)
{
    if (list == NULL)
        return 0;
    unsigned i = 1;
    while(list->next != NULL)
    {
        list = list->next;
        ++i;
    }
    return i;
}

SList *slist_copy(SList *list)
{
    if (list == NULL)
        return NULL;
    SList *head = slist_prepend(NULL, list->data);
    SList *current = head;
    while (list->next != NULL)
    {
        slist_insert(current, list->next->data);
        current = current->next;
        list = list->next;
    }
    return head;
}

// FIXED: without loops
SList *slist_reverse(SList *list)
{
    if (list == NULL || list->next == NULL)
        return list;
    SList *n = list->next;
    SList *new_head = slist_reverse(n);
    n->next = list;
    list->next = NULL;
    return new_head;
}

SList *slist_concat(SList *list1, SList *list2)
{
    if (list1 == NULL)
        return list2;
    SList *last = list1;
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = list2;
    return list1;
}

void slist_foreach(SList *list, void (*func)(Pointer, Pointer), Pointer user_data)
{
    if (list == NULL)
        return;
    while (list->next != NULL)
    {
        func(list->data, user_data);
        list = list->next;
    }
}

SList *slist_last(SList *list)
{
    if (list == NULL || list->next == NULL)
        return list;
    SList *last = list->next;
    while (last->next != NULL)
    {
        last = last->next;
    }
    return last;
}

// FIXED: without loops
SList *slist_nth(SList *list, int n)
{
    if (n < 0)
        n += slist_length(list);
    if (list == NULL || n == 0)
        return list;
    return slist_nth(list->next, n - 1);
}

SList *slist_find(SList *haystack, Pointer needle)
{
    if (haystack == NULL)
        return NULL;
    SList *curr = haystack;
    while (curr != NULL)
    {
        if (curr->data == needle)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

SList *slist_find_custom(SList *haystack, Pointer needle, int (*compare_func)(Pointer, Pointer))
{
    if (haystack == NULL)
        return NULL;
    SList *curr = haystack;
    while (curr != NULL)
    {
        if (compare_func(curr->data, needle) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

int slist_position(SList *list, SList *el)
{
    if (list == NULL && el != NULL)
        return -1;
    SList *curr = list;
    int curr_pos = 0;
    while (curr != NULL)
    {
        if (curr == el)
            return curr_pos;
        curr = curr->next;
        ++curr_pos;
    }
    return -1;
}
