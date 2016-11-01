#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdlib.h>

typedef void * Pointer;

typedef int (*CmpFunc)(Pointer data1, Pointer data2);

typedef struct tAVLTreeNode {
    Pointer data;
    struct tAVLTreeNode *left;
    struct tAVLTreeNode *right;
    size_t height;
} AVLTreeNode;

typedef struct tAVLTree {
    AVLTreeNode *root;
    CmpFunc cmp_func; /* all data comparisons should be done
                         with help of this func! */
} AVLTree;

typedef struct tAVLTree AVLTree;

// Create empty tree
AVLTree *avl_create(CmpFunc cmp_func);

// Clear tree but do not destroy tree struct
void avl_clear(AVLTree *tree);

// Completely destroy tree
void avl_destroy(AVLTree *tree);

size_t avl_size(AVLTree *tree);

// Find element with equal data and return its data if any else NULL
Pointer avl_find(AVLTree *tree, Pointer data);

// Return data which was replaced by this insertion if any else NULL
Pointer avl_insert(AVLTree *tree, Pointer data);

// Delete node with equal data and return its data if any else NULL
Pointer avl_delete(AVLTree *tree, Pointer data);

// Call foreach_func for every node's data in tree passing given extra_data
void avl_foreach(AVLTree *tree,
                 void (*foreach_func)(Pointer data, Pointer extra_data),
                 Pointer extra_data);

// Returns 0 if tree is correct;
int avl_check(AVLTree *tree);

#endif // AVL_TREE_H
