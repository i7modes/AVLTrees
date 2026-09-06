/**
 * @file test_avl_tree.c
 * @brief Automated unit test suite for Self-Balancing AVL Tree.
 * @author i7modes
 * @license MIT
 */

#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int total_tests = 0;
static int passed_tests = 0;

#define TEST_ASSERT(expr, msg) do { \
    total_tests++; \
    if (expr) { \
        passed_tests++; \
        printf("  [PASS] %s\n", msg); \
    } else { \
        printf("  [FAIL] %s (Line %d)\n", msg, __LINE__); \
    } \
} while (0)

static void test_empty_tree_safety(void)
{
    printf("\n--- Test: Empty Tree Safety ---\n");
    AVLNode *tree = NULL;

    TEST_ASSERT(AVL_IsEmpty(tree) == true, "NULL tree is empty");
    TEST_ASSERT(AVL_Size(tree) == 0, "NULL tree size is 0");
    TEST_ASSERT(AVL_Height(tree) == 0, "NULL tree height is 0");
    TEST_ASSERT(AVL_BalanceFactor(tree) == 0, "NULL tree balance factor is 0");
    TEST_ASSERT(AVL_Find(tree, 10) == NULL, "Find on empty tree returns NULL");
    TEST_ASSERT(AVL_FindMin(tree) == NULL, "FindMin on empty tree returns NULL");
    TEST_ASSERT(AVL_FindMax(tree) == NULL, "FindMax on empty tree returns NULL");
    TEST_ASSERT(AVL_Delete(tree, 10) == NULL, "Delete on empty tree returns NULL");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Empty tree is valid AVL");

    AVL_Destroy(&tree);
    TEST_ASSERT(tree == NULL, "AVL_Destroy on NULL tree is safe");
}

static void test_four_rotations(void)
{
    printf("\n--- Test: 4 Specific Rotation Cases (LL, RR, LR, RL) ---\n");

    // Case 1: Left-Left (Right Rotation)
    AVLNode *treeLL = NULL;
    treeLL = AVL_Insert(treeLL, 30);
    treeLL = AVL_Insert(treeLL, 20);
    treeLL = AVL_Insert(treeLL, 10);
    TEST_ASSERT(treeLL->data == 20 && treeLL->left->data == 10 && treeLL->right->data == 30,
                "LL Case triggers Right Rotation: root is 20");
    TEST_ASSERT(AVL_IsBalanced(treeLL) == true, "LL tree is balanced");
    AVL_Destroy(&treeLL);

    // Case 2: Right-Right (Left Rotation)
    AVLNode *treeRR = NULL;
    treeRR = AVL_Insert(treeRR, 10);
    treeRR = AVL_Insert(treeRR, 20);
    treeRR = AVL_Insert(treeRR, 30);
    TEST_ASSERT(treeRR->data == 20 && treeRR->left->data == 10 && treeRR->right->data == 30,
                "RR Case triggers Left Rotation: root is 20");
    TEST_ASSERT(AVL_IsBalanced(treeRR) == true, "RR tree is balanced");
    AVL_Destroy(&treeRR);

    // Case 3: Left-Right (Double Left-Right Rotation)
    AVLNode *treeLR = NULL;
    treeLR = AVL_Insert(treeLR, 30);
    treeLR = AVL_Insert(treeLR, 10);
    treeLR = AVL_Insert(treeLR, 20);
    TEST_ASSERT(treeLR->data == 20 && treeLR->left->data == 10 && treeLR->right->data == 30,
                "LR Case triggers Double Left-Right Rotation: root is 20");
    TEST_ASSERT(AVL_IsBalanced(treeLR) == true, "LR tree is balanced");
    AVL_Destroy(&treeLR);

    // Case 4: Right-Left (Double Right-Left Rotation)
    AVLNode *treeRL = NULL;
    treeRL = AVL_Insert(treeRL, 10);
    treeRL = AVL_Insert(treeRL, 30);
    treeRL = AVL_Insert(treeRL, 20);
    TEST_ASSERT(treeRL->data == 20 && treeRL->left->data == 10 && treeRL->right->data == 30,
                "RL Case triggers Double Right-Left Rotation: root is 20");
    TEST_ASSERT(AVL_IsBalanced(treeRL) == true, "RL tree is balanced");
    AVL_Destroy(&treeRL);
}

static void test_large_sequential_balancing(void)
{
    printf("\n--- Test: 100 Sequential Insertions & Height Bound ---\n");
    AVLNode *tree = NULL;

    for (int i = 1; i <= 100; i++)
    {
        tree = AVL_Insert(tree, i);
    }

    TEST_ASSERT(AVL_Size(tree) == 100, "Tree size is exactly 100");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Tree remains strictly balanced across 100 sequential inserts");

    // In an unbalanced tree, height would be 100.
    // For AVL tree with 100 nodes, max theoretical height is 1.44 * log2(100) = ~9.
    int height = AVL_Height(tree);
    TEST_ASSERT(height <= 9, "Tree height is <= 9 (strict logarithmic O(log n))");

    // In-order traversal must be strictly sorted 1..100
    int buffer[100];
    size_t copied = AVL_ToArrayInOrder(tree, buffer, 100);
    TEST_ASSERT(copied == 100, "Copied 100 items");

    bool sorted = true;
    for (int i = 0; i < 100; i++)
    {
        if (buffer[i] != i + 1)
        {
            sorted = false;
            break;
        }
    }
    TEST_ASSERT(sorted == true, "In-order array is sorted exactly 1 through 100");

    AVL_Destroy(&tree);
    TEST_ASSERT(tree == NULL, "Tree destroyed cleanly");
}

static void test_deletions_with_rebalancing(void)
{
    printf("\n--- Test: Deletions & Rebalancing Invariants ---\n");
    AVLNode *tree = NULL;
    int vals[] = {50, 20, 80, 10, 30, 70, 90, 5, 15, 25, 35};
    size_t n = sizeof(vals) / sizeof(vals[0]);

    for (size_t i = 0; i < n; i++) tree = AVL_Insert(tree, vals[i]);
    TEST_ASSERT(AVL_Size(tree) == 11, "Initial size is 11");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Initial tree is balanced");

    // Delete leaf node (5)
    tree = AVL_Delete(tree, 5);
    TEST_ASSERT(AVL_Size(tree) == 10, "Size is 10");
    TEST_ASSERT(AVL_Find(tree, 5) == NULL, "5 is deleted");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Tree balanced after leaf delete");

    // Delete node with 1 child (10)
    tree = AVL_Delete(tree, 10);
    TEST_ASSERT(AVL_Size(tree) == 9, "Size is 9");
    TEST_ASSERT(AVL_Find(tree, 10) == NULL, "10 is deleted");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Tree balanced after 1-child delete");

    // Delete node with 2 children (20)
    tree = AVL_Delete(tree, 20);
    TEST_ASSERT(AVL_Size(tree) == 8, "Size is 8");
    TEST_ASSERT(AVL_Find(tree, 20) == NULL, "20 is deleted");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Tree balanced after 2-child delete");

    // Delete root (50)
    tree = AVL_Delete(tree, 50);
    TEST_ASSERT(AVL_Size(tree) == 7, "Size is 7");
    TEST_ASSERT(AVL_Find(tree, 50) == NULL, "50 is deleted");
    TEST_ASSERT(AVL_IsBalanced(tree) == true, "Tree balanced after root delete");

    AVL_Destroy(&tree);
}

int main(void)
{
    printf("========================================\n");
    printf("  AVL Tree Automated Tests\n");
    printf("========================================\n");

    test_empty_tree_safety();
    test_four_rotations();
    test_large_sequential_balancing();
    test_deletions_with_rebalancing();

    printf("\n========================================\n");
    printf("  Results: %d/%d assertions passed\n", passed_tests, total_tests);
    printf("========================================\n");

    return (passed_tests == total_tests) ? 0 : 1;
}
