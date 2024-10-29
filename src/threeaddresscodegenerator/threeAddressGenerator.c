#include "../../include/threeAddressGenerator.h"

QuadrupleLinkedList *quadrupleList = NULL;

Node *newTemp();

QuadrupleLinkedList *getQuadrupleList()
{
    return quadrupleList;
}

Node *generateUnaryQuadruple(Tag flag, Node *arg1)
{
    Node *temp = newTemp();
    Quadruple *quad = newUnaryQuadruple(flag, arg1, temp);
    quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
    return temp;
}

Node *generateBinaryQuadruple(Tag flag, Node *arg1, Node *arg2)
{
    Node *temp = newTemp();
    Quadruple *quad = newQuadruple(flag, arg1, arg2, temp);
    quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
    return temp;
}

Node *generateThreeAddressCode(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return NULL;
    }
    Tag flag = tree->root->flag;
    Node *temp = NULL;
    Node *arg1 = NULL;
    Node *arg2 = NULL;

    switch (flag)
    {
    case PROG:
        break;

    case NUMBER:
    case BOOL:
    case PARAM:
    case ID:
        return tree->root;

    case MINUS:
        if (tree->right != NULL)
        {
            arg1 = generateThreeAddressCode(tree->left);
            arg2 = generateThreeAddressCode(tree->right);
            return generateBinaryQuadruple(flag, arg1, arg2);
        }
        arg1 = generateThreeAddressCode(tree->left);
        return generateUnaryQuadruple(flag, arg1);

    case NOT:
        arg1 = generateThreeAddressCode(tree->left);
        return generateUnaryQuadruple(flag, arg1);

    case VARDECL:
        if (tree->right != NULL)
        {
            arg1 = generateThreeAddressCode(tree->left);
            arg2 = generateThreeAddressCode(tree->right);
            Quadruple *quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return NULL;
        }
        arg1 = generateThreeAddressCode(tree->left);
        return generateUnaryQuadruple(ASSIGN, arg1);

    case ASSIGN:
        arg1 = generateThreeAddressCode(tree->left);
        arg2 = generateThreeAddressCode(tree->right);
        Quadruple *quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return NULL;

    case DIVISION:
    case MOD:
    case AND:
    case OR:
    case MULTIPLY:
    case LESSTHAN:
    case GRATERTHAN:
    case EQUALS:
    case PLUS:
        arg1 = generateThreeAddressCode(tree->left);
        arg2 = generateThreeAddressCode(tree->right);
        return generateBinaryQuadruple(flag, arg1, arg2);

    default:
        break;
    }

    // opearadores bool, int, and relational -- OK
    // asignaciones -- OK
    
    // if, if-else, while
    /// method decl
    // method call
    // return
    
    generateThreeAddressCode(tree->left);
    generateThreeAddressCode(tree->right);
    return NULL;
}

int i = 1;
Node *newTemp()
{
    char name[10];
    sprintf(name, "t%d", i);
    i = i + 1;
    Node *temp = malloc(sizeof(Node));
    temp->name = strdup(name);
    return temp;
}