#include <stdlib.h>
#include <stdio.h>

typedef struct AVLnode *AVLNode;

struct AVLnode
{
    int Element;
    AVLNode Left;
    AVLNode Right;
    int Height; // Balance information
};

AVLNode MakeEmpty(AVLNode T);
AVLNode Find(int, AVLNode);
AVLNode FindMin(AVLNode);
AVLNode FindMax(AVLNode T);
int Height(AVLNode P);
int Max(int, int);
AVLNode SingleRotateToRight(AVLNode);
AVLNode SingleRotateToLeft(AVLNode);
AVLNode DoubleRotateToLeft(AVLNode);
AVLNode DoubleRotateToRight(AVLNode K1);
AVLNode Insert(int, AVLNode);
void PrintInOrder(AVLNode);

int main()
{

    AVLNode tree = MakeEmpty(NULL);

    tree = Insert(1, tree);
    tree = Insert(2, tree);
    tree = Insert(3, tree);
    tree = Insert(4, tree);
    tree = Insert(5, tree);
    tree = Insert(6, tree);
    tree = Insert(7, tree);
    tree = Insert(16, tree);
    tree = Insert(15, tree);
    tree = Insert(14, tree);
    tree = Insert(13, tree);
    tree = Insert(12, tree);
    tree = Insert(11, tree);
    tree = Insert(10, tree);

    printf("Test to print the tree (In-Order):\n");
    PrintInOrder(tree);

    return 0;
}

AVLNode MakeEmpty(AVLNode T)
{
    if (T != NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

AVLNode Find(int X, AVLNode T)
{
    if (T == NULL)
        return NULL;
    if (X < T->Element)
        return Find(X, T->Left);
    else if (X > T->Element)
        return Find(X, T->Right);
    else
        return T;
}

AVLNode FindMin(AVLNode T)
{
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

AVLNode FindMax(AVLNode T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;

    return T;
}

int Height(AVLNode P)
{
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

int Max(int Lhs, int Rhs)
{
    return Lhs > Rhs ? Lhs : Rhs;
}

AVLNode SingleRotateToRight(AVLNode K2)
{
    AVLNode K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), K2->Height) + 1;

    return K1;
}

AVLNode SingleRotateToLeft(AVLNode K1)
{
    AVLNode K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), K1->Height) + 1;

    return K2;
}

AVLNode DoubleRotateToLeft(AVLNode K3)
{
    K3->Left = SingleRotateToLeft(K3->Left);
    return SingleRotateToRight(K3);
}

AVLNode DoubleRotateToRight(AVLNode K1)
{
    K1->Right = SingleRotateToRight(K1->Right);
    return SingleRotateToLeft(K1);
}

AVLNode Insert(int X, AVLNode T)
{
    if (T == NULL)
    {

        T = malloc(sizeof(struct AVLnode));
        if (T == NULL)
            printf("Out of space!!!");
        else
        {
            T->Element = X;
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if (X < T->Element)
    {
        T->Left = Insert(X, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
            if (X < T->Left->Element)
                T = SingleRotateToRight(T);
            else
                T = DoubleRotateToLeft(T);
    }
    else if (X > T->Element)
    {
        T->Right = Insert(X, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
            if (X > T->Right->Element)
                T = SingleRotateToLeft(T);
            else
                T = DoubleRotateToRight(T);
    }
    /* Else X is in the tree already; we'll do nothing */

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

AVLNode Delete(int X, AVLNode T)
{
    if (T == NULL)
    {
        printf("\nThe word you entered not found\n\n");
        return NULL;
    }

    if (X < T->Element)
    {
        T->Left = Delete(X, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
        {
            if (X < T->Left->Element)
            {
                T = SingleRotateToRight(T);
            }
            else
            {
                T = DoubleRotateToLeft(T);
            }
        }
    }
    else if (X > T->Element)
    {
        T->Right = Delete(X, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
        {
            if (X > T->Right->Element)
            {
                T = SingleRotateToLeft(T);
            }
            else
            {
                T = DoubleRotateToRight(T);
            }
        }
    }
    else
    {
        AVLNode temp;
        if (T->Left && T->Right)
        {
            temp = FindMin(T->Right);
            T->Element = temp->Element;
            T->Right = Delete(X, T->Right);

            if (Height(T->Right) - Height(T->Left) == 2)
            {
                if (X > T->Right->Element)
                {
                    T = SingleRotateToLeft(T);
                }
                else
                {
                    T = DoubleRotateToRight(T);
                }
            }
        }
        else
        {
            temp = T;
            if (T->Left == NULL)
            {
                T = T->Right;
            }
            else if (T->Right == NULL)
            {
                T = T->Left;
            }
            free(temp);
            printf("\nThe word have been deleted successfully\n\n");
        }
    }

    if (T == NULL)
    {
        return T;
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

void PrintInOrder(AVLNode t)
{
    if (t != NULL)
    {
        PrintInOrder(t->Left);
        printf("%d\n", t->Element);
        PrintInOrder(t->Right);
    }
}
