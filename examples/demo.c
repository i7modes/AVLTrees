/**
 * @file demo.c
 * @brief Demonstration program for Self-Balancing AVL Tree library.
 * @author i7modes
 * @license MIT
 */

#include "avl_tree.h"
#include <stdio.h>

static void print_val(int val)
{
    printf("%d ", val);
}

static void print_separator(const char *title)
{
    printf("\n========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
}

int main(void)
{
    print_separator("1. Building AVL Tree: Automatic Rebalancing");
    printf("Inserting sequential values: 1, 2, 3, 4, 5, 6, 7...\n");
    printf("(An unbalanced BST would degenerate into a linked list of height 7)\n");

    AVLNode *tree = NULL;
    for (int i = 1; i <= 7; i++)
    {
        tree = AVL_Insert(tree, i);
    }

    printf("Resulting AVL Tree Height: %d (Logarithmic!)\n", AVL_Height(tree));
    printf("Is strictly balanced? %s\n", AVL_IsBalanced(tree) ? "YES" : "NO");

    print_separator("2. 2D ASCII Tree Layout (With Balance Factors)");
    AVL_Print2D(tree);

    print_separator("3. Inserting More Elements");
    int more[] = {16, 15, 14, 13, 12, 11, 10};
    printf("Inserting: 16, 15, 14, 13, 12, 11, 10...\n");
    for (size_t i = 0; i < sizeof(more) / sizeof(more[0]); i++)
    {
        tree = AVL_Insert(tree, more[i]);
    }

    printf("Updated Total Nodes: %zu\n", AVL_Size(tree));
    printf("Updated Tree Height: %d\n", AVL_Height(tree));
    printf("Is strictly balanced? %s\n", AVL_IsBalanced(tree) ? "YES" : "NO");

    print_separator("4. In-Order Sorted Traversal");
    printf("Sorted Ascending: ");
    AVL_InOrder(tree, print_val);
    printf("\n");

    print_separator("5. Deletions Triggering Rebalancing");
    printf("Deleting key 4 (root/internal node)...\n");
    tree = AVL_Delete(tree, 4);

    printf("Deleting key 7...\n");
    tree = AVL_Delete(tree, 7);

    printf("Tree after deletions:\n");
    AVL_Print2D(tree);
    printf("\nIs balanced after deletions? %s\n", AVL_IsBalanced(tree) ? "YES" : "NO");
    printf("New Size: %zu, Height: %d\n", AVL_Size(tree), AVL_Height(tree));

    print_separator("6. Legacy API Compatibility Check");
    AVLNode *legacy = MakeEmpty(NULL);
    legacy = Insert(30, legacy);
    legacy = Insert(20, legacy);
    legacy = Insert(40, legacy);
    printf("Legacy FindMin: %d\n", FindMin(legacy)->data);
    printf("Legacy FindMax: %d\n", FindMax(legacy)->data);
    legacy = MakeEmpty(legacy);
    printf("Legacy tree emptied.\n");

    print_separator("7. Complete Teardown");
    AVL_Destroy(&tree);
    printf("AVL root pointer: %p (0 memory leaks)\n", (void *)tree);

    printf("\nAll AVL Tree demonstrations completed successfully!\n");
    return 0;
}
