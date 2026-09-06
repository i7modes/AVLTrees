/**
 * @file avl_tree.c
 * @brief Implementation of Self-Balancing AVL Tree in C.
 * @author i7modes
 * @license MIT
 */

#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static inline int max_int(int a, int b)
{
    return (a > b) ? a : b;
}

static inline void update_height(AVLNode *node)
{
    if (node != NULL)
    {
        node->height = 1 + max_int(AVL_Height(node->left), AVL_Height(node->right));
    }
}

/* -------------------------------------------------------------------------- */
/*                         Lifecycle & Memory Management                      */
/* -------------------------------------------------------------------------- */

AVLNode* AVL_CreateNode(int value)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (node == NULL)
    {
        fprintf(stderr, "Error: Out of memory in AVL_CreateNode.\n");
        return NULL;
    }

    node->data = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AVLNode* AVL_Clear(AVLNode *root)
{
    if (root != NULL)
    {
        AVL_Clear(root->left);
        AVL_Clear(root->right);
        free(root);
    }
    return NULL;
}

void AVL_Destroy(AVLNode **root_ptr)
{
    if (root_ptr == NULL || *root_ptr == NULL)
    {
        return;
    }

    *root_ptr = AVL_Clear(*root_ptr);
}

/* -------------------------------------------------------------------------- */
/*                              Core Queries & Rotations                      */
/* -------------------------------------------------------------------------- */

int AVL_Height(const AVLNode *node)
{
    return (node != NULL) ? node->height : 0;
}

int AVL_BalanceFactor(const AVLNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return AVL_Height(node->left) - AVL_Height(node->right);
}

AVLNode* AVL_RotateRight(AVLNode *y)
{
    if (y == NULL || y->left == NULL)
    {
        return y;
    }

    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

AVLNode* AVL_RotateLeft(AVLNode *x)
{
    if (x == NULL || x->right == NULL)
    {
        return x;
    }

    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

AVLNode* AVL_RotateLeftRight(AVLNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    node->left = AVL_RotateLeft(node->left);
    return AVL_RotateRight(node);
}

AVLNode* AVL_RotateRightLeft(AVLNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    node->right = AVL_RotateRight(node->right);
    return AVL_RotateLeft(node);
}

/* -------------------------------------------------------------------------- */
/*                              Insert & Delete                               */
/* -------------------------------------------------------------------------- */

AVLNode* AVL_Insert(AVLNode *root, int value)
{
    if (root == NULL)
    {
        return AVL_CreateNode(value);
    }

    if (value < root->data)
    {
        root->left = AVL_Insert(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = AVL_Insert(root->right, value);
    }
    else
    {
        return root; /* Duplicate keys not inserted */
    }

    update_height(root);
    int balance = AVL_BalanceFactor(root);

    /* Left-Left Case */
    if (balance > 1 && value < root->left->data)
    {
        return AVL_RotateRight(root);
    }

    /* Right-Right Case */
    if (balance < -1 && value > root->right->data)
    {
        return AVL_RotateLeft(root);
    }

    /* Left-Right Case */
    if (balance > 1 && value > root->left->data)
    {
        return AVL_RotateLeftRight(root);
    }

    /* Right-Left Case */
    if (balance < -1 && value < root->right->data)
    {
        return AVL_RotateRightLeft(root);
    }

    return root;
}

AVLNode* AVL_FindMin(const AVLNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    const AVLNode *current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return (AVLNode *)current;
}

AVLNode* AVL_FindMax(const AVLNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    const AVLNode *current = root;
    while (current->right != NULL)
    {
        current = current->right;
    }
    return (AVLNode *)current;
}

AVLNode* AVL_Find(const AVLNode *root, int value)
{
    const AVLNode *current = root;

    while (current != NULL)
    {
        if (value == current->data)
        {
            return (AVLNode *)current;
        }
        else if (value < current->data)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return NULL;
}

AVLNode* AVL_Delete(AVLNode *root, int value)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (value < root->data)
    {
        root->left = AVL_Delete(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = AVL_Delete(root->right, value);
    }
    else
    {
        /* Target node found */
        if (root->left == NULL || root->right == NULL)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                /* No child case */
                temp = root;
                root = NULL;
            }
            else
            {
                /* One child case: copy contents */
                *root = *temp;
            }
            free(temp);
        }
        else
        {
            /* Two children case: get in-order successor */
            AVLNode *successor = AVL_FindMin(root->right);
            root->data = successor->data;
            root->right = AVL_Delete(root->right, successor->data);
        }
    }

    if (root == NULL)
    {
        return NULL;
    }

    update_height(root);
    int balance = AVL_BalanceFactor(root);

    /* Left-Left Case */
    if (balance > 1 && AVL_BalanceFactor(root->left) >= 0)
    {
        return AVL_RotateRight(root);
    }

    /* Left-Right Case */
    if (balance > 1 && AVL_BalanceFactor(root->left) < 0)
    {
        return AVL_RotateLeftRight(root);
    }

    /* Right-Right Case */
    if (balance < -1 && AVL_BalanceFactor(root->right) <= 0)
    {
        return AVL_RotateLeft(root);
    }

    /* Right-Left Case */
    if (balance < -1 && AVL_BalanceFactor(root->right) > 0)
    {
        return AVL_RotateRightLeft(root);
    }

    return root;
}

bool AVL_IsEmpty(const AVLNode *root)
{
    return (root == NULL);
}

size_t AVL_Size(const AVLNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    return 1 + AVL_Size(root->left) + AVL_Size(root->right);
}

/* -------------------------------------------------------------------------- */
/*                           Traversals & Serialization                       */
/* -------------------------------------------------------------------------- */

void AVL_InOrder(const AVLNode *root, void (*visitor)(int))
{
    if (root == NULL || visitor == NULL)
    {
        return;
    }

    AVL_InOrder(root->left, visitor);
    visitor(root->data);
    AVL_InOrder(root->right, visitor);
}

void AVL_PreOrder(const AVLNode *root, void (*visitor)(int))
{
    if (root == NULL || visitor == NULL)
    {
        return;
    }

    visitor(root->data);
    AVL_PreOrder(root->left, visitor);
    AVL_PreOrder(root->right, visitor);
}

void AVL_PostOrder(const AVLNode *root, void (*visitor)(int))
{
    if (root == NULL || visitor == NULL)
    {
        return;
    }

    AVL_PostOrder(root->left, visitor);
    AVL_PostOrder(root->right, visitor);
    visitor(root->data);
}

static void in_order_collector(const AVLNode *root, int *buffer, size_t *count, size_t max_len)
{
    if (root == NULL || *count >= max_len)
    {
        return;
    }

    in_order_collector(root->left, buffer, count, max_len);
    if (*count < max_len)
    {
        buffer[(*count)++] = root->data;
    }
    in_order_collector(root->right, buffer, count, max_len);
}

size_t AVL_ToArrayInOrder(const AVLNode *root, int *buffer, size_t max_len)
{
    if (root == NULL || buffer == NULL || max_len == 0)
    {
        return 0;
    }

    size_t count = 0;
    in_order_collector(root, buffer, &count, max_len);
    return count;
}

/* -------------------------------------------------------------------------- */
/*                           Validation & Visualization                       */
/* -------------------------------------------------------------------------- */

static bool is_valid_avl_helper(const AVLNode *root, long min_val, long max_val)
{
    if (root == NULL)
    {
        return true;
    }

    if (root->data <= min_val || root->data >= max_val)
    {
        return false;
    }

    int bf = AVL_BalanceFactor(root);
    if (bf < -1 || bf > 1)
    {
        return false;
    }

    return is_valid_avl_helper(root->left, min_val, root->data) &&
           is_valid_avl_helper(root->right, root->data, max_val);
}

bool AVL_IsBalanced(const AVLNode *root)
{
    return is_valid_avl_helper(root, LONG_MIN, LONG_MAX);
}

static void print_2d_util(const AVLNode *root, int space)
{
    if (root == NULL)
    {
        return;
    }

    space += 9;
    print_2d_util(root->right, space);

    printf("\n");
    for (int i = 9; i < space; i++)
    {
        printf(" ");
    }
    printf("[%d (bf:%d)]\n", root->data, AVL_BalanceFactor(root));

    print_2d_util(root->left, space);
}

void AVL_Print2D(const AVLNode *root)
{
    if (root == NULL)
    {
        printf("(empty tree)\n");
        return;
    }
    print_2d_util(root, 0);
}

static void print_node_val(int val)
{
    printf("%d ", val);
}

void AVL_PrintInOrder(const AVLNode *root)
{
    if (root == NULL)
    {
        printf("(empty tree)\n");
        return;
    }

    printf("[ ");
    AVL_InOrder(root, print_node_val);
    printf("]\n");
}
