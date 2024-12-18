#include "../../include/threeAddressGenerator.h"

QuadrupleLinkedList *quadrupleList = NULL;

Node *newTemp();
Node *newLabel();

int checkIfMethodIsExternal(Tree *pTree);

/**
 * Retrieves the global quadruple list.
 */
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

/**
 * Generates three-address code for method parameters with a specified order.
 *
 * @param methodParameters The tree representation of method parameters.
 * @param order The order of the parameter in the method call.
 */
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

/** The current offset used for variable declarations. */
int currentOffset = 0;
/** Flag indicating if a method is currently being declared. */
int methodDeclaredflag = 0;
/** Tree storing all declared IDs. */
Tree *idsDeclaredTreeList = NULL;
/** Flag indicating if a method context is active. */
int methodFlag = 0;

/**
 * Generates three-address code for the given tree.
 * 
 * @param tree The tree representing the code.
 * @return The resulting Node or NULL if no result is produced.
 */
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
        Node *foundParamNode = findNodeInTree(idsDeclaredTreeList, paramNode->name);
        if (idsDeclaredTreeList == NULL)
        {
            currentOffset++;
            paramNode->offset = currentOffset;
            idsDeclaredTreeList = createTree(paramNode, NULL, NULL);
        }
        else if (foundParamNode == NULL)
        {
            currentOffset++;
            paramNode->offset = currentOffset;
            idsDeclaredTreeList = createTree(paramNode, idsDeclaredTreeList, NULL);
        }
        else
        {
            paramNode->offset = foundParamNode->offset;
        }
        generateThreeAddressCode(tree->left);
        return paramNode;

    case ID:
        Node *idNode = tree->root;
        Node *foundNode = findNodeInTree(idsDeclaredTreeList, idNode->name);
        if (tree->root->isGlobal)
        {
            idNode->isGlobal = 1;
        }
        if (idsDeclaredTreeList == NULL && methodFlag != 1)
        {
            currentOffset++;
            idNode->offset = currentOffset;
            idsDeclaredTreeList = createTree(idNode, NULL, NULL);
        }
        else if (foundNode == NULL && methodFlag != 1)
        {
            currentOffset++;
            idNode->offset = currentOffset;
            idsDeclaredTreeList = createTree(idNode, idsDeclaredTreeList, NULL);
        }
        else if (foundNode != NULL)
        {
            idNode->offset = foundNode->offset;
        }
        return tree->root;

    case NUMBER:
    case BOOL:
        return tree->root;

    case MINUS:
        if (tree->right != NULL)
        {
            arg1 = generateThreeAddressCode(tree->left);
            arg2 = generateThreeAddressCode(tree->right);
            return generateBinaryQuadruple(SUBTRACTION, arg1, arg2);
        }
        arg1 = generateThreeAddressCode(tree->left);
        return generateUnaryQuadruple(flag, arg1);

    case NOT:
        arg1 = generateThreeAddressCode(tree->left);
        return generateUnaryQuadruple(flag, arg1);

    case VARDECL:
        arg1 = generateThreeAddressCode(tree->left);
        if (tree->right != NULL)
        {
            arg2 = generateThreeAddressCode(tree->right);
            if (methodDeclaredflag == 1)
            {
                quad = newUnaryQuadruple(ASSIGN, arg2, arg1);
            }
            else
            {
                quad = newUnaryQuadruple(GASSIGN, arg2, arg1);
                tree->left->root->isGlobal = 1;
            }
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return NULL;
        }
        if (methodDeclaredflag == 1)
        {
            // case for only declaration without assignment, ej: integer y;
            Quadruple *quad = newUnaryQuadruple(WASSIGN, arg1, NULL);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            return arg1;
        }
        tree->left->root->isGlobal = 1;
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
        if (checkIfMethodIsExternal(tree))
        {
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
            temp = generateBinaryQuadruple(EXTERNAL_CALL, arg1, paramsNode);
            return temp;
        }
        else
        {
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
        }

    case METHODDECL:
        if (checkIfMethodIsExternal(tree))
        {
            methodDeclaredflag = 1;
            methodFlag = 1;
            arg1 = generateThreeAddressCode(tree->left);

            quad = newSimpleQuadruple(INIT_EXTERNAL_METHOD, arg1);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);
            methodFlag = 0;
            generateThreeAddressCode(tree->right);

            arg1->offset = currentOffset;
            quad = newSimpleQuadruple(END_EXTERNAL_METHOD, arg1);
            quadrupleList = addQuadrupleLinkedList(quadrupleList, quad);

            // reset
            currentOffset = 0;
            idsDeclaredTreeList = NULL;
        }
        else
        {
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
        }
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

/** Counter for generating unique temporary variable names. */
int tempCount = 1;

/**
 * Creates a new temporary variable node with a unique name.
 * 
 * @return A new Node representing a temporary variable.
 */
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

    currentOffset++;
    temp->offset = currentOffset;
    return temp;
}

/** Counter for generating unique label names. */
int labelCount = 0;

/**
 * Creates a new label node with a unique name.
 * 
 * @return A new Node representing a label.
 */
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