#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "avl_tree.h"

typedef enum eRotationType {
    LEFT,
    RIGHT
} RotationType;

static void print_alloc_error(int line, const char* func);

static void dfv_for_nodes(AVLTreeNode *node,
                          void (*f_each)(AVLTreeNode *));

static void dfv_for_data(AVLTreeNode *node,
                         Pointer extra_data,
                         void (*f_each)(Pointer, Pointer));

static void dfv_for_check(AVLTreeNode *node,
                          bool (*check)(AVLTreeNode *),
                          size_t *num_of_false);

static size_t calculate_size_of_node(AVLTreeNode *node);

static AVLTreeNode *new_node(Pointer data);

static Pointer dfs(AVLTreeNode *node, Pointer needle,
                        int (*cmp)(Pointer, Pointer));

static void destroy_node(AVLTreeNode *node);

static size_t max(size_t a, size_t b);

static size_t calculate_height(AVLTreeNode *node);

static int get_balance(AVLTreeNode *node);

static AVLTreeNode *balance(AVLTreeNode *node);

static AVLTreeNode *rotate(AVLTreeNode *b, RotationType type);

static AVLTreeNode *big_rotate(AVLTreeNode *node, RotationType type);

static bool is_balanced(AVLTreeNode *node);


AVLTree *avl_create(CmpFunc cmp_func)
{
    assert(cmp_func != NULL);
    AVLTree *tree = malloc(sizeof(AVLTree));
    if (!tree) {
        print_alloc_error(__LINE__, __FUNCTION__);
        exit(1);
    }
    tree->cmp_func = cmp_func;
    tree->root = NULL;
    return tree;
}

void avl_clear(AVLTree *tree)
{
    if (tree == NULL) {
        return;
    }
    dfv_for_nodes(tree->root, destroy_node);
    tree->root = NULL;
}

void avl_destroy(AVLTree *tree)
{
    if (tree == NULL) {
        return;
    }
    dfv_for_nodes(tree->root, destroy_node);
    free(tree);
}

size_t avl_size(AVLTree *tree)
{
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }
    return calculate_size_of_node(tree->root);
}

/*
 * Returns deepest suitable node.
 */
Pointer avl_find(AVLTree *tree, Pointer data)
{
    if (tree == NULL) {
        return NULL;
    }
    if (tree->cmp_func == NULL) {
        fprintf(stderr, "Err:"
                "Compare function required:%s:%d",
                __FUNCTION__, __LINE__);
        exit(2);
    }
    return dfs(tree->root, data, tree->cmp_func);
}

static AVLTreeNode *insert_node(AVLTreeNode *node,
                                Pointer data,
                                int (*cmp)(Pointer, Pointer),
                                AVLTreeNode **replaced)
{
    assert(cmp != NULL);
    if (node == NULL) {
        return new_node(data);
    }
    if (cmp(data, node->data) < 0) {
        node->left = insert_node(node->left, data, cmp, replaced);
    }
    if (cmp(data, node->data) > 0){
        node->right = insert_node(node->right, data, cmp, replaced);
    }
    if (cmp(data, node->data) == 0){
        *replaced = node->data;
        node->data = data;
        return node;
    }
    return balance(node);
}

Pointer avl_insert(AVLTree *tree, Pointer data)
{
    assert(tree != NULL);
    AVLTreeNode *replaced = NULL;
    tree->root = insert_node(tree->root,
                             data,
                             tree->cmp_func,
                             &replaced);
    return replaced;
}

static AVLTreeNode *delete_node(AVLTreeNode *node,
                                Pointer data,
                                int (*cmp)(Pointer, Pointer),
                                Pointer **deleted)
{
    assert(cmp != NULL);
    if (node == NULL) {
        return NULL;
    }
    if (cmp(data, node->data) != 0) {
        if (cmp(data, node->data) < 0) {
            node->left = delete_node(node->left, data, cmp, deleted);
        } else {
            node->right = delete_node(node->right, data, cmp, deleted);
        }
    } else {
        AVLTreeNode *temp;
        if (node->left != NULL) {
            node->left->right = node->right;
            temp = node->left;
        } else {
            temp = node->right;
        }
        *deleted = node->data;
        destroy_node(node);
        node = temp;
    }
    return balance(node);
}

Pointer avl_delete(AVLTree *tree, Pointer data)
{
    assert(tree != NULL);
    Pointer *deleted = NULL;
    tree->root = delete_node(tree->root,
                             data,
                             tree->cmp_func,
                             &deleted);
    return deleted;
}

void avl_foreach(AVLTree *tree,
                 void (*foreach_func)(Pointer, Pointer),
                 Pointer extra_data)
{
    assert(tree != NULL);
    dfv_for_data(tree->root, extra_data, foreach_func);
}

int avl_check(AVLTree *tree)
{
    if (tree == NULL) {
        return true;
    }
    size_t res = 0;
    dfv_for_check(tree->root, is_balanced, &res);
    return res;
}


/*
 * ####################################################
 * ####                                            ####
 * ####             INTERNAL FUNCTIONS             ####
 * ####                                            ####
 * ####################################################
 */


static void print_alloc_error(int line, const char *func)
{
    fprintf(stderr, "Err:Can't allocate memory:%s:%d",
            func, line);
}

AVLTreeNode *new_node(Pointer data)
{
    AVLTreeNode *node = malloc(sizeof(AVLTreeNode));
    if (!node) {
        print_alloc_error(__LINE__, __FUNCTION__);
        exit(1);
    }
    node->data = data;
    node->left = node->right = 0;
    node->height = 1;
    return node;
}

static void dfv_for_nodes(AVLTreeNode *node,
                          void (*f_each)(AVLTreeNode *))
{
    if (node == NULL) {
        return;
    }
    dfv_for_nodes(node->left, f_each);
    dfv_for_nodes(node->right, f_each);
    f_each(node);
}

static void dfv_for_data(AVLTreeNode *node,
                         Pointer extra_data,
                         void (*f_each)(Pointer, Pointer))
{
    if (node == NULL) {
        return;
    }
    dfv_for_data(node->left, extra_data, f_each);
    dfv_for_data(node->right, extra_data, f_each);
    f_each(node->data, extra_data);
}

static Pointer dfs(AVLTreeNode *node,
                        Pointer needle,
                        int (*cmp)(Pointer, Pointer))
{
    if (node == NULL) {
        return node;
    }
    if (cmp(node->data, needle) == 0) {
        return node->data;
    }
    Pointer res = NULL;
    res = dfs(node->left, needle, cmp);
    if (res != NULL) {
        return res;
    }
    res = dfs(node->right, needle, cmp);
    if (res != NULL) {
        return res;
    }
    return NULL;
}

static void destroy_node(AVLTreeNode *node)
{
    node->left = node->right = node->data = NULL;
    free(node);
}

static size_t calculate_size_of_node(AVLTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }
    return calculate_size_of_node(node->left)  +
           calculate_size_of_node(node->right) + 1;
}

static size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}

static size_t calculate_height(AVLTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }
    return max(calculate_height(node->left),
               calculate_height(node->right)) + 1;
}

static int get_balance(AVLTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }
    return calculate_height(node->left) -
           calculate_height(node->right);
}

static AVLTreeNode *balance(AVLTreeNode *node)
{
    if (node == NULL) {
        return node;
    }
    node->height = calculate_height(node);
    int balance = get_balance(node);
    int balance_left = get_balance(node->left);
    int balance_right = get_balance(node->right);
    if (balance >= 2 && balance_left >= 0) {
        return rotate(node, LEFT);
    }
    if (balance <= -2 && balance_right <= 0) {
        return rotate(node, RIGHT);
    }
    if (balance >= 2 && balance_left <= -1) {
        return big_rotate(node, LEFT);
    }
    if (balance <= -2 && balance_right >= 1) {
        return big_rotate(node, RIGHT);
    }
    return node;
}

static AVLTreeNode **get_child_p(AVLTreeNode *parent, RotationType type)
{
    switch (type) {
    case LEFT: return &parent->left;
    case RIGHT: return &parent->right;
    default: assert(false); return NULL;
    }
}

static AVLTreeNode **get_another_child_p(AVLTreeNode *parent, RotationType type)
{
    switch (type) {
    case LEFT: return &parent->right;
    case RIGHT: return &parent->left;
    default: assert(false); return NULL;
    }
}

static void set_child(AVLTreeNode *parent, RotationType type, AVLTreeNode *child)
{
    switch (type) {
    case LEFT: parent->left = child; return;
    case RIGHT: parent->right = child; return;
    default: assert(false); return;
    }
}

static void set_another_child(AVLTreeNode *parent, RotationType type, AVLTreeNode *child)
{
    switch (type) {
    case LEFT: parent->right = child; return;
    case RIGHT: parent->left = child; return;
    default: assert(false); return;
    }
}

static AVLTreeNode *rotate(AVLTreeNode *b, RotationType type)
{
    AVLTreeNode *a, *temp;
    a = *get_child_p(b, type);
    temp = *get_another_child_p(a, type);
    set_another_child(a, type, b);
    set_child(b, type, temp);
    a->height = calculate_height(a);
    b->height = calculate_height(b);
    return a;
}

static AVLTreeNode *do_rotate(AVLTreeNode *b, RotationType type)
{
    return rotate(b, type);
}

static AVLTreeNode *do_another_rotate(AVLTreeNode *b, RotationType type)
{
    return rotate(b, type == LEFT ? RIGHT : LEFT);
}

static AVLTreeNode *big_rotate(AVLTreeNode *node, RotationType type)
{
    AVLTreeNode **child = get_child_p(node, type);
    *child = do_another_rotate(*child, type);
    return do_rotate(node, type);
}

static bool is_balanced(AVLTreeNode *node)
{
    return abs(get_balance(node)) <= 1;
}

static void dfv_for_check(AVLTreeNode *node, bool (*check)(AVLTreeNode *), size_t *num_of_false)
{
    if (node == NULL) {
        return;
    }
    dfv_for_check(node->left, check, num_of_false);
    if (!check(node)) {
        ++(*num_of_false);
    }
    dfv_for_check(node->right, check, num_of_false);
}
