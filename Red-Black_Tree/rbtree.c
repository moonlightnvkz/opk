#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "rbtree.h"

typedef enum Side {
    LEFT,
    RIGHT
} Side;

static void* allocate_and_exit_if_failed(size_t size, const int line, const char *func);

static void destroy_node(RBTreeNode *node);

static void traverse(RBTreeNode *node, void (*func)(RBTreeNode *));

static Pointer dfs(RBTreeNode *node,
                   Pointer needle,
                   int (*cmp)(Pointer, Pointer));

static void traverse_foreach(RBTreeNode *node,
                             void (*foreach_func)(Pointer, Pointer),
                             Pointer extra_data);

static RBTreeNode* new_node(Pointer data);

static Side compare_res_to_side(int res);

static RBTreeNode *get_child(RBTreeNode *node, Side side);

static RBTreeNode *set_child(RBTreeNode *node, Side side, RBTreeNode *child);

static void fixup_tree_after_insertion(RBTree *tree, RBTreeNode *node);



RBTree *rb_create(CmpFunc cmp_func)
{
    assert(cmp_func != NULL);
    RBTree *tree = allocate_and_exit_if_failed(sizeof(RBTree), __LINE__, __FUNCTION__);
    tree->cmp_func = cmp_func;
    tree->root = NULL;
    return tree;
}

void rb_clear(RBTree *tree)
{
    assert(tree != NULL);
    traverse(tree->root, destroy_node);
    tree->cmp_func = NULL;
    tree->root = NULL;
    tree->size = 0;
}

void rb_destroy(RBTree *tree)
{
    assert(tree != NULL);
    rb_clear(tree);
    free(tree);
}

size_t rb_size(RBTree *tree)
{
    assert(tree != NULL);
    return tree->size;
}

Pointer rb_find(RBTree *tree, Pointer data)
{
    assert(tree != NULL);
    assert(tree->cmp_func != NULL);
    if (tree == NULL) {
        return NULL;
    }
    return dfs(tree->root, data, tree->cmp_func);
}

Pointer rb_insert(RBTree *tree, Pointer data)
{
    assert(tree != NULL);
    Pointer replaced = NULL;
    if (tree->root == NULL) {
        tree->root = new_node(data);
        tree->size++;
        return replaced;
    }
    RBTreeNode *node = tree->root;
    while (true) {
        int comp_res = tree->cmp_func(data, node->data);
        if (comp_res == 0) {
            replaced = node->data;
            node->data = data;
            // Exit from the loop
            break;
        } else {
            Side side = compare_res_to_side(comp_res);
            RBTreeNode *child = get_child(node, side);
            if (child == NULL) {
                set_child(node, side, new_node(data));
                fixup_tree_after_insertion(tree, get_child(node, side));
                // Exit from the loop
                tree->size++;
                break;
            }
            node = get_child(node, side);
        }
    }
    return replaced;
}

Pointer rb_delete(RBTree *tree, Pointer data)
{
    tree->size--;
    return data;
}

void rb_foreach(RBTree *tree, void (*foreach_func)(Pointer, Pointer), Pointer extra_data)
{
    assert(tree != NULL);
    assert(foreach_func != NULL);
    traverse_foreach(tree->root, foreach_func, extra_data);
}

bool rb_check(RBTree *tree, RBTreeNode *node)
{
    assert(tree != NULL);
    if (node->left == NULL || node->right == NULL) {
        return true;
    }
    if (node->color == RED &&
       (node->left->color != BLACK || node->right->color != BLACK)) {
        return false;
    }
    if (node->left != NULL) {
        if (tree->cmp_func(node->data, node->left->data) <= 0) {
            return false;
        }
        rb_check(tree, node->left);
    }
    if (node->right != NULL) {
        if (tree->cmp_func(node->data, node->right->data) <= 0) {
            return false;
        }
        rb_check(tree, node->right);
    }
    return true;
}

/*
 * ####################################################
 * ####                                            ####
 * ####             INTERNAL FUNCTIONS             ####
 * ####                                            ####
 * ####################################################
 */

static RBTreeNode* new_node(Pointer data)
{
    RBTreeNode *node = allocate_and_exit_if_failed(sizeof(RBTreeNode), __LINE__, __FUNCTION__);
    node->color = RED;
    node->data = data;
    node->father = node->left = node->right = NULL;
    return node;
}

static Side compare_res_to_side(int res)
{
    assert(res != 0);
    return res < 0 ? LEFT : RIGHT;
}

static Side opposite_side(Side side)
{
    return side == LEFT ? RIGHT : LEFT;
}

static RBTreeNode *get_child(RBTreeNode *node, Side side)
{
    if (node == NULL) {
        return NULL;
    }
    return side == LEFT ? node->left : node->right;
}

static RBTreeNode *set_child(RBTreeNode *node, Side side, RBTreeNode *child)
{
    RBTreeNode *old_child = get_child(node, side);
    switch (side) {
    case LEFT:
        node->left = child;
        if (child != NULL)
            node->left->father = node;
        break;
    case RIGHT:
        node->right = child;
        if (child != NULL)
            node->right->father = node;
        break;
    default:
        assert(false);
    }
    return old_child;
}

static bool is_red(RBTreeNode *node)
{
    return (node == NULL || node->color == BLACK) ? false : true;
}

static bool is_root(RBTree *tree, RBTreeNode *node)
{
    return tree->root == node && node != NULL;
}

static void set_color(RBTreeNode *node, RBColor color)
{
    if (node != NULL) {
        node->color = color;
    }
}

static RBTreeNode *father(RBTreeNode *node)
{
    return node == NULL ? NULL : node->father;
}

static RBTreeNode *grandfather(RBTreeNode *node)
{
    return (node == NULL || node->father == NULL) ? NULL : node->father->father;
}

static RBTreeNode *get_left(RBTreeNode *node)
{
    return node == NULL ? NULL : node->left;
}

static RBTreeNode *get_right(RBTreeNode *node)
{
    return node == NULL ? NULL : node->right;
}

static Side get_side(RBTreeNode *node)
{
    return (node == get_left(father(node))) ? LEFT : RIGHT;
}

static bool is_opposite_side(Side side1, Side side2)
{
    return side1 == side2 ? false : true;
}

static RBTreeNode *sibling_of(RBTreeNode *node)
{
    return (node == NULL || father(node) == NULL) ? NULL :
           (node == get_left(father(node))) ?
            get_right(father(node)) : get_left(father(node));
}

static void rotate(RBTreeNode *node, Side side)
{
    if (node == NULL) {
        return;
    }
    RBTreeNode *axis = get_child(node, opposite_side(side));
    axis->father = node->father; /* при этом, возможно, axis становится корнем дерева */
    if (father(node) != NULL) {
        set_child(father(node), get_side(node), axis);
    }

    set_child(node, opposite_side(side), get_child(axis, side));
    if (get_child(axis, side) != NULL) {
        get_child(axis, side)->father = node;
    }

    node->father = axis;
    set_child(axis, side, node);
}

static void fixup_tree_after_insertion(RBTree *tree, RBTreeNode *node)
{
    if (node != NULL && !is_root(tree, node) && is_red(father(node))) {
        if (is_red(sibling_of(father(node)))) {
            set_color(father(node), BLACK);
            set_color(sibling_of(father(node)), BLACK);
            set_color(grandfather(node), RED);
            fixup_tree_after_insertion(tree, grandfather(node));
        } else if (father(node) != NULL && grandfather(node) != NULL) {
            Side side_of_father = get_side(father(node));
            if (is_opposite_side(get_side(node), side_of_father)) {
                node = father(node);
                rotate(node, side_of_father);
            }
            set_color(father(node), BLACK);
            set_color(grandfather(node), RED);
            if (tree->root == grandfather(node)) {
                tree->root = father(node);
            }
            rotate(grandfather(node), opposite_side(side_of_father));
        }
    }
    set_color(tree->root, BLACK);
}

static void* allocate_and_exit_if_failed(size_t size, const int line, const char *func)
{
    void* temp = malloc(size);
    if (!temp) {
        fprintf(stderr, "Err:Failed to allocate memory:%d:%s", line, func);
        exit(1);
    }
    return temp;
}

static void destroy_node(RBTreeNode *node)
{
    free(node);
}

static void traverse(RBTreeNode *node, void (*func)(RBTreeNode *))
{
    assert(func != NULL);
    if (node == NULL) {
        return;
    }
    traverse(node->left, func);
    traverse(node->right, func);
    func(node);
}

static Pointer dfs(RBTreeNode *node,
                   Pointer needle,
                   int (*cmp)(Pointer, Pointer))
{
    assert(cmp != NULL);
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

static void traverse_foreach(RBTreeNode *node,
                             void (*foreach_func)(Pointer, Pointer),
                             Pointer extra_data)
{
    if (node == NULL) {
        return;
    }
    traverse_foreach(node->left, foreach_func, extra_data);
    traverse_foreach(node->right, foreach_func, extra_data);
    foreach_func(node->data, extra_data);
}
