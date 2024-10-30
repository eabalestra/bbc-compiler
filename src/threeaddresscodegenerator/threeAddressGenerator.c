#include "../../include/threeAddressGenerator.h"

QuadrupleLinkedList *quadrupleList = NULL;

Node *newTemp();
Node *newLabel();

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

//************************* */ NO BORRAR PLISS *************************
/* Node *generateExprWithOrder(Tree *methodParameters, int order)
{
    if (methodParameters->root->flag != EMPTY) {
        printf("entro aca??????\n");
        Node *arg1 = generateThreeAddressCode(methodParameters->left);
        Node *paramOrder = createNode(NUMBER, NONTYPE, order, NULL, NULL);
        printf(paramOrder->value);
        Quadruple *quad = generateBinaryQuadruple(PARAM, arg1, order);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
    }

    return NULL;
} */

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
    Node *label = NULL;

    switch (flag)
    {
    case EXPR:
        // TODO agregarle el numero del parametro (orden)
        arg1 = generateThreeAddressCode(tree->left);
        quad = newUnaryQuadruple(PARAM, arg1, NULL);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return NULL;
    case NUMBER:
    case BOOL:
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

    case METHODCALL:
        arg1 = generateThreeAddressCode(tree->left);

        Tree *methodParameters = tree->right;
        int i = 0;
        while (methodParameters != NULL)
        {
            i++;
            Node *param = generateThreeAddressCode(methodParameters);
            // Node *param = generateExprWithOrder(methodParameters, i);
            methodParameters = methodParameters->right;
        }
        Node *paramsNode = createNode(NUMBER, NONTYPE, i, NULL, NULL);
        quad = generateBinaryQuadruple(CALL, arg1, paramsNode);
        return NULL;

    case METHODDECL:
        arg1 = generateThreeAddressCode(tree->left);
        quad = newSimpleQuadruple(INITMETHOD, arg1);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        generateThreeAddressCode(tree->right);
        quad = newSimpleQuadruple(ENDMETHOD, arg1);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return NULL;

    case IF: // if-then
        if (tree->right->right == NULL)
        {
            arg1 = generateThreeAddressCode(tree->left);
            label = newLabel();
            quad = newUnaryQuadruple(JMPF, arg1, label);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            generateThreeAddressCode(tree->right);
            quad = newSimpleQuadruple(LABEL, label);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return NULL;
        }
        arg1 = generateThreeAddressCode(tree->left); // condicion del if
        label = newLabel();
        quad = newUnaryQuadruple(JMPF, arg1, label); // JMPF T L
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        generateThreeAddressCode(tree->right->left); // then
        Node *labelGoto = newLabel();
        quad = newSimpleQuadruple(GOTO, labelGoto); // GOTO del THEN
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);

        quad = newSimpleQuadruple(LABEL, label); // LABEL del else
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        generateThreeAddressCode(tree->right->right); // else
        quad = newSimpleQuadruple(LABEL, labelGoto);  // LABEL del THEN
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return NULL;

    case WHILE:
        Node *labelBeginWhile = newLabel();
        quad = newSimpleQuadruple(LABEL, labelBeginWhile);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
    
        arg1 = generateThreeAddressCode(tree->left);
        Node *labelEndWhile = newLabel();
        quad = newUnaryQuadruple(JMPF, arg1, labelEndWhile);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        
        generateThreeAddressCode(tree->right);
        
        quad = newSimpleQuadruple(GOTO, labelBeginWhile);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        
        quad = newSimpleQuadruple(LABEL, labelEndWhile);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);

        return NULL;

    default:
        break;
    }

    /**
     * if (x == 2) then {
     *      y = x + x;
     * } else {
     *      y = x;
     * }
     *
     *
     *
     *
     */

    // opearadores bool, int, and relational -- OK
    // asignaciones -- OK
    // return ---- OK
    // method call ---- OK
    // if, if-else,  ---- OK
    // while ---- OK
    // method decl ---- OK

    // TODO:
    // paramNumber ---- ???

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

int j = 0;
Node *newLabel()
{
    char name[10];
    sprintf(name, "L%d", j);
    j = j + 1;
    Node *label = malloc(sizeof(Node));
    label->name = strdup(name);
    return label;
}

/*
LESS, A, B, T1
IF, T1, -, L1
RET, - , - , 1
GOTO, , L2


LABEL 2

*/