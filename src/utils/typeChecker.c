#include "../../include/typeChecker.h"

void checkTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Tree *leftChild = tree->left;
    Tree *rightChild = tree->right;

    if (tree->root->flag == VARDECL)
    {
        if (rightChild != NULL)
        {
            Type righChildtype;
            Node *righChildNode = rightChild->root;

            if (righChildNode->flag != ID || righChildNode->flag != NUMBER || righChildNode->flag != BOOLEAN)
            {
                righChildtype = checkExpressionTypes(rightChild);
            }
            else
            {
                righChildtype = rightChild->root->type;
            }

            if (leftChild->root->type != righChildtype)
            {
                Node *leftChildNode = leftChild->root;
                printf("Type Error [Line %d]: Variable '%s' is declared as type '%s', "
                       "but is assigned a value of incompatible type '%s'.\n",
                       leftChildNode->line_number, leftChildNode->name,
                       nodeTypeToString(leftChildNode->type), nodeTypeToString(righChildtype));

                exit(1);
            }
        }
    }

    /* if (tree->root->flag == ASSIGN)
    {
        int rType = checkTypes(rightChild);

    } */

    // caso return

    checkTypes(tree->left);
    checkTypes(tree->right);
}

Type checkExpressionTypes(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Node *node = tree->root;
    Tag nodeFlag = node->flag;

    if (nodeFlag == ID || nodeFlag == NUMBER || nodeFlag == BOOL || nodeFlag == METHODCALL)
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

    printf("Type Error [Line %d]: Operator '%s' is not defined for types '%s' and '%s'.\n",
           node->line_number, nodeFlagToString(nodeFlag), nodeTypeToString(leftType), nodeTypeToString(rightType));
    exit(1);
}