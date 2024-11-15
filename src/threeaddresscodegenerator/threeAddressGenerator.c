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

Node *generateExprWithOrder(Tree *methodParameters, int order)
{
    if (methodParameters == NULL || methodParameters->root == NULL)
    {
        return NULL;
    }
    if (methodParameters->root->flag != EMPTY)
    {
        Node *arg1 = generateThreeAddressCode(methodParameters->left);
        Node *paramOrder = createNode(NUMBER, NONTYPE, order, NULL, NULL);
        Quadruple *quad = newQuadruple(PARAM, arg1, paramOrder, NULL);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
    }
    return NULL;
}

int currentOffset = 0;
int methodDeclaredflag = 0;

Tree *idsDeclaredTreeList = NULL;
int methodFlag = 0;

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
    case PARAM:
        Node *paramNode = tree->root;
        if (idsDeclaredTreeList == NULL)
        {
            currentOffset++;
            paramNode->offset = currentOffset;
            printf("INCREMENTE POR %s\n", paramNode->name);
            idsDeclaredTreeList = createTree(paramNode, NULL, NULL);
        }
        else if (findNodeInTree(idsDeclaredTreeList, paramNode->name) == NULL)
        {
            currentOffset++;
            paramNode->offset = currentOffset;
            printf("INCREMENTE POR %s\n", paramNode->name);
            idsDeclaredTreeList = createTree(paramNode, idsDeclaredTreeList, NULL);
        }
        generateThreeAddressCode(tree->left);
        return paramNode;
        /* case PARAM:
            Node *paramNode = tree->root;
            if (paramNode->offset == 0)
            {
                printf("INCREMENTE POR %s\n", paramNode->name);
                currentOffset++;
                paramNode->offset = currentOffset;
            }
            generateThreeAddressCode(tree->left);
            return paramNode; */

    case ID:
        Node *idNode = tree->root;
        if (idsDeclaredTreeList == NULL && methodFlag != 1)
        {
            currentOffset++;
            idNode->offset = currentOffset;
            printf("INCREMENTE POR %s\n", idNode->name);
            idsDeclaredTreeList = createTree(idNode, NULL, NULL);
        }
        else if (findNodeInTree(idsDeclaredTreeList, idNode->name) == NULL && methodFlag != 1)
        {
            currentOffset++;
            idNode->offset = currentOffset;
            printf("INCREMENTE POR %s\n", idNode->name);
            idsDeclaredTreeList = createTree(idNode, idsDeclaredTreeList, NULL);
        }
        return tree->root;

        /* case ID:
            Node *idNode = tree->root;
            if (idNode->offset == 0 && methodFlag != 1)
            {
                printf("INCREMENTE POR %s\n", idNode->name);
                currentOffset++;
                idNode->offset = currentOffset;
            }
            return tree->root; */

    case NUMBER:
    case BOOL:
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
            if (methodDeclaredflag == 1)
            {
                quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
            }
            else
            {
                quad = newUnaryQuadruple(GASSIGN, arg2, arg1);
            }
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return NULL;
        }
        arg1 = generateThreeAddressCode(tree->left);
        if (methodDeclaredflag == 1)
        {
            return generateUnaryQuadruple(ASSIGN, arg1);
        }
        return generateUnaryQuadruple(GASSIGN, arg1);

    case ASSIGN:
        arg1 = generateThreeAddressCode(tree->left);
        arg2 = generateThreeAddressCode(tree->right);
        if (methodDeclaredflag == 1)
        {
            quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
        }
        else
        {
            quad = newUnaryQuadruple(GASSIGN, arg2, arg1);
        }
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        return arg1;

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
        methodFlag = 1;
        arg1 = generateThreeAddressCode(tree->left);
        methodFlag = 0;

        Tree *methodParameters = tree->right;
        int i = 0;
        while (methodParameters != NULL && methodParameters->root->flag != EMPTY)
        {
            i++;
            Node *param = generateExprWithOrder(methodParameters, i);
            methodParameters = methodParameters->right;
        }
        Node *paramsNode = createNode(NUMBER, NONTYPE, i, NULL, NULL);
        temp = generateBinaryQuadruple(CALL, arg1, paramsNode);
        return temp;

    case METHODDECL:
        methodDeclaredflag = 1;
        methodFlag = 1;
        arg1 = generateThreeAddressCode(tree->left);

        quad = newSimpleQuadruple(INITMETHOD, arg1);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        methodFlag = 0;
        generateThreeAddressCode(tree->right);

        arg1->offset = currentOffset;
        quad = newSimpleQuadruple(ENDMETHOD, arg1);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);

        // reset
        currentOffset = 0;
        idsDeclaredTreeList = NULL;
        return NULL;

    case IF:
        if (tree->right->right == NULL) // case if-then
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
        arg1 = generateThreeAddressCode(tree->left);
        label = newLabel();
        quad = newUnaryQuadruple(JMPF, arg1, label);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        generateThreeAddressCode(tree->right->left);
        Node *labelGoto = newLabel();
        quad = newSimpleQuadruple(GOTO, labelGoto);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);

        quad = newSimpleQuadruple(LABEL, label);
        quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
        generateThreeAddressCode(tree->right->right);
        quad = newSimpleQuadruple(LABEL, labelGoto);
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
    generateThreeAddressCode(tree->left);
    generateThreeAddressCode(tree->right);

    return NULL;
}

int tempCount = 1;
Node *newTemp()
{
    char name[10];
    sprintf(name, "t%d", tempCount);
    tempCount++;
    Node *temp = malloc(sizeof(Node));
    if (temp == NULL)
    {
        printf("Error creating temp node\n");
        exit(1);
    }
    temp->flag = TEMP;
    temp->name = strdup(name);

    printf("INCREMENTE POR %s\n", temp->name);
    currentOffset++;
    temp->offset = currentOffset;
    return temp;
}

int labelCount = 0;
Node *newLabel()
{
    char name[10];
    sprintf(name, "L%d", labelCount);
    labelCount++;
    Node *label = malloc(sizeof(Node));
    if (label == NULL)
    {
        printf("Error creating label node\n");
        exit(1);
    }
    label->name = strdup(name);
    return label;
}