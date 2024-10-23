#include "../../include/typeChecker.h"

Node *currentMethod; // variable global para la funcion actual en la que estoy

void checkTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Tree *leftChild = tree->left;
    Tree *rightChild = tree->right;
    Tag rootTag = tree->root->flag;

    if (rootTag == METHODDECL)
    {
        currentMethod = tree->left->root;
    }

    if (rightChild != NULL && (rootTag == VARDECL || rootTag == ASSIGN || rootTag == EQUALS || rootTag == LESSTHAN || rootTag == GRATERTHAN || rootTag == AND || rootTag == OR))
    {

        Node *leftChildNode = leftChild->root;
        Node *rightChildNode = rightChild->root;
        Type leftChildType;
        Type rightChildType;

        if (rootTag == LESSTHAN || rootTag == GRATERTHAN)
        {
            if (leftChild->root->type == BOOLEAN || rightChild->root->type == BOOLEAN)
            {
                printf("Type Error [Line %d]: Arithmetic operator with boolean values.\n", leftChildNode->line_number);
                exit(1);
            }
        }

        if ((rootTag == AND || rootTag == OR))
        {

            if (leftChild->root->type == INTEGER || rightChild->root->type == INTEGER)
            {
                printf("Type Error [Line %d]: Boolean operator with integer values.\n", leftChildNode->line_number);
                exit(1);
            }
        }

        if (rightChildNode->flag == ID || rightChildNode->flag == NUMBER || rightChildNode->flag == BOOL)
        {
            rightChildType = rightChild->root->type;
        }
        else
        {
            rightChildType = checkExpressionTypes(rightChild);
        }

        if (leftChildNode->flag == ID || leftChildNode->flag == NUMBER || leftChildNode->flag == BOOL)
        {
            leftChildType = leftChild->root->type;
        }
        else
        {
            leftChildType = checkExpressionTypes(leftChild);
        }

        if (leftChildType != rightChildType)
        {
            printf("Type Error [Line %d]: Variable '%s' is declared as type '%s', "
                   "but is assigned a value of incompatible type '%s'.\n",
                   leftChildNode->line_number, leftChildNode->name,
                   nodeTypeToString(leftChildNode->type), nodeTypeToString(rightChildType));
            exit(1);
        }
    }
    if (rootTag == RETURN)
    {
        Type currentMethodType = currentMethod->type;
        if (tree->left != NULL)
        {
            Type returnType = checkExpressionTypes(tree->left);
            if (currentMethodType != returnType)
            {
                printf("Type Error [Line %d]: Function '%s' with return type '%s' is different from actual return type '%s'.\n",
                       tree->left->root->line_number, currentMethod->name,
                       nodeTypeToString(currentMethodType), nodeTypeToString(returnType));
                exit(1);
            }
        }
        else
        {
            if (currentMethodType != VOID)
            {
                printf("Type Error [Line %d]: Function '%s' with return type '%s' has no actual return type.\n",
                       tree->root->line_number, currentMethod->name,
                       nodeTypeToString(currentMethodType));
                exit(1);
            }
        }
    }
    if (rootTag == METHODCALL)
    {
        checkParameters(tree->left, tree->left->root->parameters, tree->right->root->parameters);
    }

    // TODO
    // caso parametros
    checkTypes(tree->left);
    checkTypes(tree->right);
}

/**
 *
 */
Type checkExpressionTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Node *node = tree->root;
    Tag nodeFlag = node->flag;

    if (nodeFlag == METHODCALL)
    {
        return checkExpressionTypes(tree->left); // chequea el tipo del metodo
    }
    if (nodeFlag == ID || nodeFlag == NUMBER || nodeFlag == BOOL)
    {
        return node->type;
    }

    Type leftType = checkExpressionTypes(tree->left);
    Type rightType = checkExpressionTypes(tree->right);

    if (nodeFlag == PLUS || nodeFlag == MINUS || nodeFlag == MULTIPLY || nodeFlag == MOD || nodeFlag == DIVISION)
    {
        if (leftType == INTEGER && rightType == INTEGER)
        {
            return INTEGER;
        }
    }
    if (nodeFlag == AND || nodeFlag == OR || nodeFlag == NOT)
    {
        if (leftType == BOOLEAN && rightType == BOOLEAN)
        {
            return BOOLEAN;
        }
    }
    if (nodeFlag == NOT)
    {
        if (leftType == BOOLEAN)
        {
            return BOOLEAN;
        }
    }
    if (nodeFlag == EQUALS)
    {
        if (leftType == BOOLEAN && rightType == BOOLEAN || leftType == INTEGER && rightType == INTEGER)
        {
            return BOOLEAN;
        }
    }
    if (nodeFlag == GRATERTHAN || nodeFlag == LESSTHAN)
    {
        if (leftType == INTEGER && rightType == INTEGER)
        {
            return BOOLEAN;
        }
    }
    else
    {
        printf("Type Error [Line %d]: Operator '%s' is not defined for types '%s' and '%s'.\n",
               node->line_number, nodeFlagToString(nodeFlag), nodeTypeToString(leftType), nodeTypeToString(rightType));
        exit(1);
    }
}

void checkParameters(Tree *method, Tree *formalParameters, Tree *actualParameters)
{
    if (formalParameters == NULL && actualParameters == NULL)
    {
        return;
    }
    printf("formalParameters: %d\n", formalParameters == NULL);
    printf("actualParameteres: %d\n", actualParameters == NULL);
    if (formalParameters == NULL || actualParameters == NULL)
    {
        printf("Type Error [Line (completar)]: Number of parameters does not match for method %s.\n", method->root->name);
        exit(1);
    }
    
    Node *formalParameter = formalParameters->root;
    Node *actualParameter = actualParameters->root;

    printf("PARAMETROS\n");
    printf("formales\n");
    printTree(formalParameters);
    printf("name: %s\n", formalParameter->name);
    printf("actuales\n");
    printTree(actualParameters);
    printf("name: %s\n", actualParameter->name);

    if (formalParameter != NULL && actualParameter != NULL)
    {
        printf("formalParameter: %s\n", nodeFlagToString(formalParameter->flag));
        if (formalParameter->type != actualParameter->type)
        {
            printf("Type Error [Line %d]: Parameter '%s' is declared as type '%s', "
                   "but is assigned a value of incompatible type '%s'.\n",
                   actualParameter->line_number, formalParameter->name,
                   nodeTypeToString(formalParameter->type), nodeTypeToString(actualParameter->type));
            exit(1);
        }
    }
    else
    {
        printf("Type Error [Line %d]: Number of parameters does not match for method %s.\n", actualParameters->root->line_number, method->root->name);
        exit(1);
    }


    checkParameters(method, formalParameters->left, actualParameters->left);
}