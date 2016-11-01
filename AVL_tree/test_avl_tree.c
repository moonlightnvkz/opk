#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "test_avl_tree.h"
#include "avl_tree.h"
#include "myqueue.h"

// Non-recursive bfs
static bool check_struct(AVLTreeNode *root,
                         char* expected[],
                         size_t size);

static int cmp(Pointer data1, Pointer data2);

void f_each(Pointer data, Pointer res);

void test_avl_create()
{
    AVLTree *tree = avl_create(cmp);
    assert(tree->cmp_func == cmp);
    assert(tree->root == NULL);
    free(tree);
    // tree = avl_create(NULL); // will cause abort from avl_create()
}

void test_avl_clear()
{
    AVLTree *t = avl_create(cmp);
    avl_insert(t, "1");
    avl_insert(t, "2");
    avl_insert(t, "3");
    avl_insert(t, "4");
    avl_insert(t, "5");
    avl_insert(t, "0");
    avl_insert(t, "7");
    avl_insert(t, "6");
    avl_clear(t);
    assert(t->root == NULL);
    avl_destroy(t);
}

void test_avl_destroy()
{
    AVLTree *t = avl_create(cmp);
    avl_insert(t, "1");
    avl_insert(t, "2");
    avl_insert(t, "3");
    avl_insert(t, "4");
    avl_insert(t, "5");
    avl_insert(t, "0");
    avl_insert(t, "7");
    avl_insert(t, "6");
    avl_destroy(t);
}

void test_avl_size()
{
    AVLTree *t = avl_create(cmp);
    assert(avl_size(t) == 0);
    avl_insert(t, "1");
    assert(avl_size(t) == 1);
    avl_insert(t, "2");
    avl_delete(t, "2");
    assert(avl_size(t) == 1);
    avl_insert(t, "3");
    assert(avl_size(t) == 2);
    avl_insert(t, "4");
    assert(avl_size(t) == 3);
    avl_insert(t, "5");
    assert(avl_size(t) == 4);
    avl_insert(t, "0");
    assert(avl_size(t) == 5);
    avl_insert(t, "7");
    assert(avl_size(t) == 6);
    avl_insert(t, "6");
    assert(avl_size(t) == 7);
    avl_destroy(t);
}

void test_avl_find()
{
    AVLTree *t = avl_create(cmp);
    assert(avl_find(t, "1") == NULL);
    avl_insert(t, "1");
    avl_insert(t, "2");
    avl_insert(t, "3");
    avl_insert(t, "4");
    avl_insert(t, "5");
    avl_insert(t, "0");
    avl_insert(t, "7");
    avl_insert(t, "6");
    assert(strcmp(avl_find(t, "2"), "2") == 0);
    assert(strcmp(avl_find(t, "1"), "1") == 0);
    assert(strcmp(avl_find(t, "3"), "3") == 0);
    assert(strcmp(avl_find(t, "0"), "0") == 0);
    assert(strcmp(avl_find(t, "6"), "6") == 0);
    assert(strcmp(avl_find(t, "5"), "5") == 0);
    assert(strcmp(avl_find(t, "7"), "7") == 0);
    assert(strcmp(avl_find(t, "4"), "4") == 0);
    assert(avl_find(t, "9") == NULL);
    avl_destroy(t);
}

void test_avl_insert()
{
    AVLTree *t = avl_create(cmp);
    assert(avl_insert(t, "1") == NULL);
    {
        char* s[] = {"1"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "2") == NULL);
    {
        char* s[] = {"1", "2"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "3") == NULL);
    {
        char* s[] = {"2", "1", "3"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "4") == NULL);
    {
        char* s[] = {"2", "1", "3", "4"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "5") == NULL);
    {
        char* s[] = {"2", "1", "4", "3", "5"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "0") == NULL);
    {
        char* s[] = {"2", "1", "4", "0", "3", "5"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "7") == NULL);
    {
        char* s[] = {"2", "1", "4", "0", "3", "5", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(avl_insert(t, "6") == NULL);
    {
        char* s[] = {"2", "1", "4", "0", "3", "6", "5", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(strcmp(avl_insert(t, "5"), "5") == 0);
    {
        char* s[] = {"2", "1", "4", "0", "3", "6", "5", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    assert(strcmp(avl_insert(t, "2"), "2") == 0);
    {
        char* s[] = {"2", "1", "4", "0", "3", "6", "5", "7"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(avl_check(t) == 0);
    avl_destroy(t);
}

void test_avl_delete()
{
    AVLTree *t = avl_create(cmp);
    assert(avl_delete(t, "1") == NULL);
    avl_insert(t, "1");
    assert(avl_delete(t, "2") == NULL);
    {
        char* s[] = {"1"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(avl_delete(t, "1"), "1") == 0);
    assert(avl_delete(t, "1") == NULL);
    assert(t->root == NULL);
    avl_insert(t, "2");
    avl_insert(t, "3");
    avl_insert(t, "4");
    avl_insert(t, "5");
    avl_insert(t, "0");
    avl_insert(t, "7");
    avl_insert(t, "6");
    assert(strcmp(avl_delete(t, "2"), "2") == 0);
    {
        char* s[] = {"5", "3", "7", "0", "4", "6"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(avl_delete(t, "3"), "3") == 0);
    assert(strcmp(avl_delete(t, "0"), "0") == 0);
    assert(strcmp(avl_delete(t, "6"), "6") == 0);
    assert(strcmp(avl_delete(t, "5"), "5") == 0);
    assert(strcmp(avl_delete(t, "7"), "7") == 0);
    {
        char* s[] = {"4"};
        assert(t->root != NULL);
        assert(check_struct(t->root, s,
                            sizeof(s) / sizeof(char*)) == 0);
    }
    assert(strcmp(avl_delete(t, "4"), "4") == 0);
    assert(t->root == NULL);
    assert(avl_delete(t, "4") == NULL);
    avl_destroy(t);
}

void test_avl_foreach()
{
    AVLTree *t = avl_create(cmp);
    int res = 0;
    avl_foreach(t, f_each, &res);
    assert(res == 0);
    assert(t->root == NULL);
    avl_insert(t, "1");
    avl_insert(t, "2");
    avl_insert(t, "3");
    avl_insert(t, "4");
    avl_insert(t, "5");
    avl_insert(t, "0");
    avl_insert(t, "7");
    avl_insert(t, "6");
    avl_foreach(t, f_each, &res);
    assert(res == 0);
    avl_destroy(t);
}

void free_data(Pointer data, Pointer _)
{
    _=_;    // To remove compiler warning
    free(data);
}

void test_avl_stress_test()
{
    AVLTree *t = avl_create(cmp);
    for (int i = 35; i < 135; ++i) {
        for (int j = 35; j < 135; ++j) {
            unsigned char *c = malloc(sizeof(unsigned char) * 3);
            c[0] = i, c[1] = j, c[2] = '\0';
            avl_insert(t, c);
        }
    }
    // avl_foreach(t, free_data, NULL);
    for (int i = 35; i < 135; ++i) {
        for (int j = 35; j < 135; ++j) {
            char *c = malloc(sizeof(char) * 3);
            c[0] = i, c[1] = j, c[2] = '\0';
            char *del = avl_delete(t, c);
            assert(strcmp(del, c) == 0);
            free(del);
            free(c);
        }
    }
    avl_destroy(t);
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

static int cmp(Pointer data1, Pointer data2)
{
    // If data1 == data2 == NULL or it is the same piece of memory
    if (((size_t)data1 ^ (size_t)data2) == 0) {
        return 0;
    }
    return strcmp(data1, data2);   // To remove compiler warning
}

// Non-recursive bfs
static bool check_struct(AVLTreeNode *root,
                         char* expected[],
                         size_t size)
{
    size_t idx = 0;
    Queue q;
    Queue *pq = &q;
    queue_create(pq);
    queue_enqueue(pq, root);
    while (queue_size(pq) > 0 && idx < size) {
        AVLTreeNode *node = queue_dequeue(pq);
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
