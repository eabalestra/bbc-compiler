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
            if (leftChild->root->type != rightChild->root->type)
            {
                Node *leftChildNode = leftChild->root;
                Node *rightChildNode = rightChild->root;

                printf("Type Error [Line %d]: Variable '%s' is declared as type '%s', "
                       "but is assigned a value of incompatible type '%s'.\n",
                       leftChildNode->line_number, leftChildNode->name,
                       nodeTypeToString(leftChildNode->type), nodeTypeToString(rightChildNode->type));

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

