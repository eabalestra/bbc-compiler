#include "../../include/threeAddressGenerator.h"

QuadrupleLinkedList *quadrupleList = NULL;

Node *newTemp();

Node *generateThreeAddressCode(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return NULL;
    }

    Tag flag = tree->root->flag;
    Node *temp;
    Node *arg1;
    Node *arg2;
    Quadruple *quad;

    switch (flag)
    {
    case PROG:
        quadrupleList = newQuadrupleLinkedList(NULL);
        break;
    case ID:
        return tree->root;
        break;
    case ASSIGN:
        temp = newTemp();
        arg1 = generateThreeAddressCode(tree->right);
        quad = newUnaryQuadruple(flag, arg1, temp);
        addQuadrupleLinkedList(quadrupleList, temp);
        return temp;
        break;
    case PLUS:
        temp = newTemp();
        arg1 = generateThreeAddressCode(tree->left);
        arg2 = generateThreeAddressCode(tree->right);
        Quadruple *quad = newQuadruple(flag, arg1, arg2, temp);
        addQuadrupleLinkedList(quadrupleList, temp);
        return temp;
        break;
    default:
        break;
    }
    generateThreeAddressCode(tree->left);
    generateThreeAddressCode(tree->right);
    if (quadrupleList != NULL)
    {
        printQuadrupleLinkedList(quadrupleList);
    }
}

int i = 0;
Node *newTemp()
{
    char *name = "t" + i;
    i = i + 1;
    Node *temp = malloc(sizeof(Node));
    temp->name = name;
    return temp;
}

/*

QuadrupleLinkedList *quadrupleList

void *recorrer(Tree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    recorrer(tree->left);
    acción(tree->root, quadrupleList);
    recorrer(tree->right);

}

void acción(Node *node, QuadrupleLinkedList *list)
{
    Tag tag = node->flag;


    

}

*/