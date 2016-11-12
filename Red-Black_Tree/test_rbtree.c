#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "myqueue.h"
#include "rbtree.h"
#include "test_rbtree.h"

static int cmp(Pointer data1, Pointer data2)
{
    // If data1 == data2 == NULL or it is the same piece of memory
    if (((size_t)data1 ^ (size_t)data2) == 0) {
        return 0;
    }
    return strcmp(data1, data2);
}

void f_each(Pointer data, Pointer res)
{
    char *expected[] = {"0", "1", "3", "5", "7", "6", "4", "2"};
    static size_t idx = 0;
    if (strcmp(data, expected[idx]) != 0) {
        ++(*(int*)res);
    }
    ++idx;
}

static bool check_struct(RBTreeNode *root,
                         char* expected[],
                         size_t size)
{
    size_t idx = 0;
    Queue q;
    Queue *pq = &q;
    queue_create(pq);
    queue_enqueue(pq, root);
    while (queue_size(pq) > 0 && idx < size) {
        RBTreeNode *node = queue_dequeue(pq);
        if (node->left != NULL) {
            queue_enqueue(pq, node->left);
        }
        if (node->right != NULL) {
            queue_enqueue(pq, node->right);
        }
        if ((expected[idx] == NULL && node->data != NULL) ||
            (expected[idx] != NULL && node->data == NULL)) {
            return 1;
        }
        if (expected[idx] == NULL && node->data == NULL) {
            idx++;
            continue;
        }
        if (strcmp(expected[idx], node->data) != 0) {
            return 1;
        }
        idx++;
    }
    if (idx != size) {
        return 1;
    }
    queue_destroy(pq);
    return 0;
}

void test_rb_create()
{
    RBTree *tree = rb_create(cmp);
    assert(tree->cmp_func == cmp);
    assert(tree->root == NULL);
    free(tree);
    // tree = rb_create(NULL); // will cause abort from rb_create()
}

void test_rb_clear()
{
    RBTree *t = rb_create(cmp);
    rb_insert(t, "1");
    rb_insert(t, "2");
    rb_insert(t, "3");
    rb_insert(t, "4");
    rb_insert(t, "5");
    rb_insert(t, "0");
    rb_insert(t, "7");
    rb_insert(t, "6");
    rb_clear(t);
    assert(t->root == NULL);
    rb_destroy(t);
}

void test_rb_destroy()
{
    RBTree *t = rb_create(cmp);
    rb_insert(t, "1");
    rb_insert(t, "2");
    rb_insert(t, "3");
    rb_insert(t, "4");
    rb_insert(t, "5");
    rb_insert(t, "0");
    rb_insert(t, "7");
    rb_insert(t, "6");
    rb_destroy(t);
}

void test_rb_size()
{
    RBTree *t = rb_create(cmp);
    assert(rb_size(t) == 0);
    rb_insert(t, "1");
    assert(rb_size(t) == 1);
    rb_insert(t, "2");
    rb_delete(t, "2");
    assert(rb_size(t) == 1);
    rb_insert(t, "3");
    assert(rb_size(t) == 2);
    rb_insert(t, "4");
    assert(rb_size(t) == 3);
    rb_insert(t, "5");
    assert(rb_size(t) == 4);
    rb_insert(t, "0");
    assert(rb_size(t) == 5);
    rb_insert(t, "7");
    assert(rb_size(t) == 6);
    rb_insert(t, "6");
    assert(rb_size(t) == 7);
    rb_destroy(t);
}

void test_rb_find()
{
    RBTree *t = rb_create(cmp);
    assert(rb_find(t, "1") == NULL);
    rb_insert(t, "1");
    rb_insert(t, "2");
    rb_insert(t, "3");
    rb_insert(t, "4");
    rb_insert(t, "5");
    rb_insert(t, "0");
    rb_insert(t, "7");
    rb_insert(t, "6");
    assert(strcmp(rb_find(t, "2"), "2") == 0);
    assert(strcmp(rb_find(t, "1"), "1") == 0);
    assert(strcmp(rb_find(t, "3"), "3") == 0);
    assert(strcmp(rb_find(t, "0"), "0") == 0);
    assert(strcmp(rb_find(t, "6"), "6") == 0);
    assert(strcmp(rb_find(t, "5"), "5") == 0);
    assert(strcmp(rb_find(t, "7"), "7") == 0);
    assert(strcmp(rb_find(t, "4"), "4") == 0);
    assert(rb_find(t, "9") == NULL);
    rb_destroy(t);
}

void test_rb_insert()
{
    RBTree *t = rb_create(cmp);
    assert(rb_insert(t, "4") == NULL);
    {
        char* s[] = {"4"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "3") == NULL);
    {
        char* s[] = {"4", "3"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "2") == NULL);
    {
        char* s[] = {"3", "2", "4"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "1") == NULL);
    {
        char* s[] = {"3", "2", "4", "1"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "6") == NULL);
    {
        char* s[] = {"3", "2", "4", "1", "6"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "5") == NULL);
    {
        char* s[] = {"3", "2", "5", "1", "4", "6"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "8") == NULL);
    {
        char* s[] = {"3", "2", "5", "1", "4", "6", "8"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "9") == NULL);
    {
        char* s[] = {"3", "2", "5", "1", "4", "8", "6", "9"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(rb_insert(t, "7") == NULL);
    {
        char* s[] = {"5", "3", "8", "2", "4", "6", "9", "1", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(rb_insert(t, "7"), "7") == 0);
    {
        char* s[] = {"5", "3", "8", "2", "4", "6", "9", "1", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(rb_insert(t, "5"), "5") == 0);
    {
        char* s[] = {"5", "3", "8", "2", "4", "6", "9", "1", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(rb_insert(t, "8"), "2") != 0);
    {
        char* s[] = {"5", "3", "8", "2", "4", "6", "9", "1", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    rb_destroy(t);
}

void test_rb_delete()
{
    RBTree *t = rb_create(cmp);
    assert(rb_delete(t, "1") == NULL);
    rb_insert(t, "1");
    assert(rb_delete(t, "2") == NULL);
    {
        char* s[] = {"1"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(rb_delete(t, "1"), "1") == 0);
    assert(rb_delete(t, "1") == NULL);
    assert(t->root == NULL);
    rb_insert(t, "2");
    rb_insert(t, "3");
    rb_insert(t, "4");
    rb_insert(t, "5");
    rb_insert(t, "0");
    rb_insert(t, "7");
    rb_insert(t, "6");
    assert(strcmp(rb_delete(t, "2"), "2") == 0);
    {
        char* s[] = {"5", "3", "7", "0", "4", "6"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(rb_delete(t, "3"), "3") == 0);
    assert(strcmp(rb_delete(t, "0"), "0") == 0);
    assert(strcmp(rb_delete(t, "6"), "6") == 0);
    assert(strcmp(rb_delete(t, "5"), "5") == 0);
    assert(strcmp(rb_delete(t, "7"), "7") == 0);
    {
        char* s[] = {"4"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(rb_delete(t, "4"), "4") == 0);
    assert(t->root == NULL);
    assert(rb_delete(t, "4") == NULL);
    rb_destroy(t);
}

void test_rb_foreach()
{
    RBTree *t = rb_create(cmp);
    int res = 0;
    rb_foreach(t, f_each, &res);
    assert(res == 0);
    assert(t->root == NULL);
    rb_insert(t, "1");
    rb_insert(t, "2");
    rb_insert(t, "3");
    rb_insert(t, "4");
    rb_insert(t, "5");
    rb_insert(t, "0");
    rb_insert(t, "7");
    rb_insert(t, "6");
    rb_foreach(t, f_each, &res);
    assert(res == 0);
    rb_destroy(t);
}
