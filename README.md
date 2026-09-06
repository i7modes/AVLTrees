# Self-Balancing AVL Tree in C

[![CI](https://github.com/i7modes/AVLTrees/actions/workflows/ci.yml/badge.svg)](https://github.com/i7modes/AVLTrees/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Standard](https://img.shields.io/badge/C-C99-00599C.svg?logo=c)](https://en.wikipedia.org/wiki/C99)
[![Memory Safety](https://img.shields.io/badge/Valgrind-0%20Leaks-brightgreen.svg)]()

A modular, production-grade **Self-Balancing AVL Tree** implemented in standard ISO C99. Named after inventors Georgy Adelson-Velsky and Evgenii Landis, an AVL tree guarantees strict balance where the heights of the two child subtrees of any node differ by at most one, delivering guaranteed $\mathcal{O}(\log n)$ performance for search, insertion, and deletion.

---

## Features

- **Guaranteed Logarithmic Bounds**: Never degenerates into an $\mathcal{O}(n)$ linked list, even under sorted sequential inputs.
- **Complete Rebalancing Engine**: Implements all 4 rotation archetypes (LL, RR, LR, RL) for both insertion and deletion rebalancing.
- **Memory Leak Free**: 100% clean Valgrind and AddressSanitizer (ASan) verification on every commit.
- **2D ASCII Visualizer**: Built-in tree printer (`AVL_Print2D`) displaying hierarchical tree layouts annotated with balance factors.
- **Sorted Serialization**: Linear-time in-order buffer extraction (`AVL_ToArrayInOrder`).
- **AVL Invariant Validator**: Structural checker (`AVL_IsBalanced`) that verifies both BST ordering and $|BF| \le 1$ at every node.
- **Legacy Compatibility Layer**: Seamless drop-in compatibility for academic coursework function signatures (`MakeEmpty`, `Insert`, `Delete`, `SingleRotateToLeft`, etc.).

---

## Asymptotic Complexity

| Operation | AVL Tree (Average) | AVL Tree (Worst Case) | Standard Unbalanced BST (Worst Case) |
| :--- | :---: | :---: | :---: |
| **Search** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(n)$ |
| **Insert** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(n)$ |
| **Delete** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(n)$ |
| **Min / Max** | $\mathcal{O}(\log n)$ | $\mathcal{O}(\log n)$ | $\mathcal{O}(n)$ |
| **In-Order Traversal** | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |
| **Space Complexity** | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |

---

## Balance Invariant & Rotations

Each node maintains its `height` property:
$$\text{Height}(node) = 1 + \max(\text{Height}(node\to left), \text{Height}(node\to right))$$
$$\text{BalanceFactor}(node) = \text{Height}(node\to left) - \text{Height}(node\to right)$$

An AVL tree enforces:
$$\text{BalanceFactor}(node) \in \{-1, 0, 1\} \quad \forall\ node \in T$$

### The Four Rotation Archetypes

```
1. Left-Left (LL) - Single Right Rotation
         z                                      y
        / \                                   /   \
       y   T4      Rotate Right (z)          x     z
      / \          ===============>         / \   / \
     x   T3                                T1  T2 T3 T4
    / \
   T1  T2

2. Right-Right (RR) - Single Left Rotation
     z                                          y
    / \                                       /   \
   T1  y           Rotate Left (z)           z     x
      / \          ==============>          / \   / \
     T2  x                                 T1 T2 T3 T4
        / \
       T3  T4

3. Left-Right (LR) - Double Rotation
         z                                  z                                 x
        / \                                / \                              /   \
       y   T4      Rotate Left (y)        x   T4     Rotate Right (z)      y     z
      / \          ==============>       / \         ===============>     / \   / \
     T1  x                              y   T3                           T1 T2 T3 T4
        / \                            / \
       T2  T3                         T1  T2

4. Right-Left (RL) - Double Rotation
       z                                  z                                     x
      / \                                / \                                  /   \
     T1  y         Rotate Right (y)     T1  x         Rotate Left (z)        z     y
        / \        ===============>        / \        ==============>       / \   / \
       x   T4                             T2  y                            T1 T2 T3 T4
      / \                                    / \
     T2  T3                                 T3  T4
```

---

## Project Structure

```
AVLTrees/
├── .github/
│   └── workflows/
│       └── ci.yml               # Multi-platform CI (Valgrind, ASan, MinGW)
├── examples/
│   └── demo.c                  # Interactive visual demo and walkthrough
├── include/
│   └── avl_tree.h              # Public API and legacy compatibility wrappers
├── src/
│   └── avl_tree.c              # Core self-balancing implementation
├── tests/
│   └── test_avl_tree.c         # Unit test suite covering all 4 rotations
├── .gitignore
├── compile_flags.txt
├── LICENSE                     # MIT License
├── Makefile                    # Cross-platform build system
└── README.md
```

---

## Getting Started

### Prerequisites

- GCC or Clang with C99 support
- GNU Make (or `mingw32-make` on Windows)
- Valgrind (optional, for Linux memory checks)

### Building and Running

```bash
# Build the test suite and interactive demo
make all

# Run all automated unit tests
make check

# Run the interactive demonstration
make demo

# Clean build artifacts
make clean
```

---

## API Reference

### Lifecycle & Memory
```c
AVLNode* AVL_CreateNode(int value);
AVLNode* AVL_Clear(AVLNode *root);
void     AVL_Destroy(AVLNode **root_ptr);
```

### Core Operations
```c
AVLNode* AVL_Insert(AVLNode *root, int value);
AVLNode* AVL_Delete(AVLNode *root, int value);
AVLNode* AVL_Find(const AVLNode *root, int value);
AVLNode* AVL_FindMin(const AVLNode *root);
AVLNode* AVL_FindMax(const AVLNode *root);
bool     AVL_IsEmpty(const AVLNode *root);
size_t   AVL_Size(const AVLNode *root);
int      AVL_Height(const AVLNode *node);
int      AVL_BalanceFactor(const AVLNode *node);
```

### Rotations
```c
AVLNode* AVL_RotateRight(AVLNode *y);
AVLNode* AVL_RotateLeft(AVLNode *x);
AVLNode* AVL_RotateLeftRight(AVLNode *node);
AVLNode* AVL_RotateRightLeft(AVLNode *node);
```

### Traversals & Validation
```c
void     AVL_InOrder(const AVLNode *root, void (*visitor)(int));
void     AVL_PreOrder(const AVLNode *root, void (*visitor)(int));
void     AVL_PostOrder(const AVLNode *root, void (*visitor)(int));
size_t   AVL_ToArrayInOrder(const AVLNode *root, int *buffer, size_t max_len);
bool     AVL_IsBalanced(const AVLNode *root);
void     AVL_Print2D(const AVLNode *root);
void     AVL_PrintInOrder(const AVLNode *root);
```

### Legacy Compatibility API
```c
MakeEmpty(T)               -> AVL_Clear(T)
Find(X, T)                 -> AVL_Find(T, X)
FindMin(T)                 -> AVL_FindMin(T)
FindMax(T)                 -> AVL_FindMax(T)
Height(P)                  -> AVL_Height(P)
SingleRotateToRight(K2)    -> AVL_RotateRight(K2)
SingleRotateToLeft(K1)     -> AVL_RotateLeft(K1)
DoubleRotateToLeft(K3)     -> AVL_RotateLeftRight(K3)
DoubleRotateToRight(K1)    -> AVL_RotateRightLeft(K1)
Insert(X, T)               -> AVL_Insert(T, X)
Delete(X, T)               -> AVL_Delete(T, X)
PrintInOrder(T)            -> AVL_PrintInOrder(T)
```

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
