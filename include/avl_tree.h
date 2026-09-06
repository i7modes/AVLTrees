/**
 * @file avl_tree.h
 * @brief Self-Balancing AVL Tree Implementation in C.
 * @author i7modes
 * @license MIT
 */

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Node structure in an AVL Tree.
 */
typedef struct AVLNode {
    int data;                   /**< Node payload */
    int height;                 /**< Height of this node in the tree */
    struct AVLNode *left;       /**< Pointer to left child */
    struct AVLNode *right;      /**< Pointer to right child */
} AVLNode;

/* -------------------------------------------------------------------------- */
/*                         Lifecycle & Memory Management                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Allocates and initializes a new AVL node with height = 1.
 * @param value Value to store.
 * @return Pointer to new node, or NULL on allocation failure.
 */
AVLNode* AVL_CreateNode(int value);

/**
 * @brief Recursively frees all nodes in the tree, returning NULL.
 * @param root Pointer to tree root.
 * @return NULL.
 */
AVLNode* AVL_Clear(AVLNode *root);

/**
 * @brief Completely deallocates all nodes in the tree, setting *root_ptr to NULL.
 * @param root_ptr Pointer to the root pointer variable.
 */
void AVL_Destroy(AVLNode **root_ptr);

/* -------------------------------------------------------------------------- */
/*                              Core Operations                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Inserts a value into the AVL tree, performing rotations as necessary.
 * Duplicates are ignored. Runs in O(log n) time.
 * @param root Pointer to the tree root.
 * @param value Value to insert.
 * @return Pointer to the new root of the subtree.
 */
AVLNode* AVL_Insert(AVLNode *root, int value);

/**
 * @brief Deletes a value from the AVL tree, performing rotations as necessary.
 * Runs in O(log n) time.
 * @param root Pointer to the tree root.
 * @param value Value to delete.
 * @return Pointer to the new root of the subtree.
 */
AVLNode* AVL_Delete(AVLNode *root, int value);

/**
 * @brief Searches for a node containing the specified value in O(log n) time.
 * @return Pointer to matching node, or NULL if not found.
 */
AVLNode* AVL_Find(const AVLNode *root, int value);

/**
 * @brief Finds the node with the minimum value.
 */
AVLNode* AVL_FindMin(const AVLNode *root);

/**
 * @brief Finds the node with the maximum value.
 */
AVLNode* AVL_FindMax(const AVLNode *root);

/**
 * @brief Checks if the tree is empty.
 */
bool AVL_IsEmpty(const AVLNode *root);

/**
 * @brief Returns the total number of nodes in the tree.
 */
size_t AVL_Size(const AVLNode *root);

/**
 * @brief Returns the height of a node (0 if NULL, 1 for leaf).
 */
int AVL_Height(const AVLNode *node);

/**
 * @brief Returns the balance factor of a node (height(left) - height(right)).
 */
int AVL_BalanceFactor(const AVLNode *node);

/* -------------------------------------------------------------------------- */
/*                                  Rotations                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Performs a Single Right Rotation (LL case).
 */
AVLNode* AVL_RotateRight(AVLNode *y);

/**
 * @brief Performs a Single Left Rotation (RR case).
 */
AVLNode* AVL_RotateLeft(AVLNode *x);

/**
 * @brief Performs a Double Left-Right Rotation (LR case).
 */
AVLNode* AVL_RotateLeftRight(AVLNode *node);

/**
 * @brief Performs a Double Right-Left Rotation (RL case).
 */
AVLNode* AVL_RotateRightLeft(AVLNode *node);

/* -------------------------------------------------------------------------- */
/*                           Traversals & Serialization                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief In-Order traversal (produces values in strictly ascending sorted order).
 */
void AVL_InOrder(const AVLNode *root, void (*visitor)(int));

/**
 * @brief Pre-Order traversal.
 */
void AVL_PreOrder(const AVLNode *root, void (*visitor)(int));

/**
 * @brief Post-Order traversal.
 */
void AVL_PostOrder(const AVLNode *root, void (*visitor)(int));

/**
 * @brief Copies elements in ascending order into an output buffer.
 * @return Number of elements copied.
 */
size_t AVL_ToArrayInOrder(const AVLNode *root, int *buffer, size_t max_len);

/* -------------------------------------------------------------------------- */
/*                           Validation & Visualization                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief Verifies that every subtree strictly satisfies both BST ordering and
 * the AVL balance condition (|balance_factor| <= 1).
 * @return true if valid AVL tree, false otherwise.
 */
bool AVL_IsBalanced(const AVLNode *root);

/**
 * @brief Prints a 2D ASCII horizontal representation of the tree with balance factors.
 */
void AVL_Print2D(const AVLNode *root);

/**
 * @brief Prints in-order elements horizontally: [ 1 2 3 ]
 */
void AVL_PrintInOrder(const AVLNode *root);

/* -------------------------------------------------------------------------- */
/*                       Legacy API Compatibility Layer                       */
/* -------------------------------------------------------------------------- */

#define AVLnode AVLNode
#define Element data
#define Left left
#define Right right
#define Height height

static inline AVLNode* MakeEmpty(AVLNode *T) {
    return AVL_Clear(T);
}

static inline AVLNode* Find(int X, AVLNode *T) {
    return AVL_Find(T, X);
}

static inline AVLNode* FindMin(AVLNode *T) {
    return AVL_FindMin(T);
}

static inline AVLNode* FindMax(AVLNode *T) {
    return AVL_FindMax(T);
}

static inline int Height(AVLNode *P) {
    return AVL_Height(P);
}

static inline AVLNode* SingleRotateToRight(AVLNode *K2) {
    return AVL_RotateRight(K2);
}

static inline AVLNode* SingleRotateToLeft(AVLNode *K1) {
    return AVL_RotateLeft(K1);
}

static inline AVLNode* DoubleRotateToLeft(AVLNode *K3) {
    return AVL_RotateLeftRight(K3);
}

static inline AVLNode* DoubleRotateToRight(AVLNode *K1) {
    return AVL_RotateRightLeft(K1);
}

static inline AVLNode* Insert(int X, AVLNode *T) {
    return AVL_Insert(T, X);
}

static inline AVLNode* Delete(int X, AVLNode *T) {
    return AVL_Delete(T, X);
}

static inline void PrintInOrder(AVLNode *T) {
    AVL_PrintInOrder(T);
}

#ifdef __cplusplus
}
#endif

#endif /* AVL_TREE_H */
