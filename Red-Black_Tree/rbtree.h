#ifndef RBTREE_H
#define RBTREE_H
#include <stdlib.h>

typedef void * Pointer;

typedef int (*CmpFunc)(Pointer data1, Pointer data2);

typedef enum eRBColor {
    RED,
    BLACK
} RBColor;

typedef struct tRBTreeNode {
    Pointer data;
    struct tRBTreeNode *father;
    struct tRBTreeNode *left;
    struct tRBTreeNode *right;
    RBColor color;
} RBTreeNode;

typedef struct tRBTree {
    RBTreeNode *root;
    CmpFunc cmp_func; /* all data comparisons should be done
                         with help of this func! */
    size_t size;
} RBTree;

// Create empty tree
RBTree * rb_create(CmpFunc cmp_func);

// Clear tree but do not destroy tree struct
void rb_clear(RBTree *tree);

// Completely destroy tree
void rb_destroy(RBTree *tree);

size_t rb_size(RBTree *tree);

// Find element with equal data and return its data if any else NULL
Pointer rb_find(RBTree *tree, Pointer data);

// Return data which was replaced by this insertion if any else NULL
Pointer rb_insert(RBTree *tree, Pointer data);

// Delete node with equal data and return its data if any else NULL
Pointer rb_delete(RBTree *tree, Pointer data);

// Call foreach_func for every node's data in tree passing given extra_data
void rb_foreach(RBTree *tree,
                void (*foreach_func)(Pointer data, Pointer extra_data),
                Pointer extra_data);

#endif // RBTREE_H
