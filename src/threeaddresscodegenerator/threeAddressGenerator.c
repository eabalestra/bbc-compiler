#include "../../include/threeAddressGenerator.h"

QuadrupleLinkedList *quadrupleList;

Node *newTemp();

QuadrupleLinkedList *getQuadrupleList() {
    return quadrupleList;
}

Node *generateThreeAddressCode(Tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }
    printf("\nENTRE A GENERATE\n");
    Tag flag = tree->root->flag;
    printf("FLAG: %s\n", nodeFlagToString(flag));
    Node *temp = NULL;
    Node *arg1 = NULL;
    Node *arg2 = NULL;
    Quadruple *quad = NULL;

    switch (flag) {
        case PROG:
            quadrupleList = newQuadrupleLinkedList(NULL);
            break;
        case PARAM:
        case ID:
            return tree->root;
        case ASSIGN:
            printf("ENTRE A ASSIGN\n");
            temp = newTemp();
            arg1 = generateThreeAddressCode(tree->right);
            quad = newUnaryQuadruple(flag, arg1, temp);
            quadrupleList = addQuadrupleLinkedList(quadrupleList,  quad);
            return temp;
        case PLUS:
            temp = newTemp();
            arg1 = generateThreeAddressCode(tree->left);
            arg2 = generateThreeAddressCode(tree->right);
            quad = newQuadruple(flag, arg1, arg2, temp);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return temp;
        default:
            break;
    }
    generateThreeAddressCode(tree->left);
    generateThreeAddressCode(tree->right);
    return NULL;
}

int i = 0;
Node *newTemp()
{
    char name[10];
    sprintf(name, "t%d", i);
    i = i + 1;
    Node *temp = malloc(sizeof(Node));
    temp->name = strdup(name);
    return temp;
}
