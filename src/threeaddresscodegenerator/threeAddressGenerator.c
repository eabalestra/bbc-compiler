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
    Quadruple *quad = NULL;
    Node *temp = NULL;
    Node *arg1 = NULL;
    Node *arg2 = NULL;

    switch (flag)
    {
    case EXPR:
        arg1 = generateThreeAddressCode(tree->left);
        quad = newUnaryQuadruple(PARAM, arg1, NULL);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return NULL;
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
            quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return NULL;
        }
        arg1 = generateThreeAddressCode(tree->left);
        return generateUnaryQuadruple(ASSIGN, arg1);

    case ASSIGN:
        arg1 = generateThreeAddressCode(tree->left);
        arg2 = generateThreeAddressCode(tree->right);
        quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
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

    // TODO: PREGUNTAR si como devolver el return al pancho o si hay que devolverlo
    case RETURN:
        if (tree->left == NULL)
        {
            quad = newUnaryQuadruple(flag, NULL, NULL);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return NULL;
        }
        arg1 = generateThreeAddressCode(tree->left);
        quad = newUnaryQuadruple(flag, arg1, NULL);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return NULL;

    // TODO: Preguntar al pancho si concide esta idea con el del libro.
    case METHODCALL:
        arg1 = generateThreeAddressCode(tree->left);

        Tree *methodParameters = tree->right;
        int i = 0;
        while (methodParameters != NULL)
        {
            i++;
            Node *param = generateThreeAddressCode(methodParameters);
            methodParameters = methodParameters->right;
        }

        Node *paramsNode = createNode(NUMBER, NONTYPE, i, NULL, NULL);
        quad = generateBinaryQuadruple(CALL, arg1, paramsNode);
        return NULL;

        // param x1
        // param x2
        // param x3
        // param xn
        // call p n
    default:
        break;
    }

    // opearadores bool, int, and relational -- OK
    // asignaciones -- OK
    // return ---- OK
    // method call ---- OK

    // if, if-else, while
    /// method decl

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