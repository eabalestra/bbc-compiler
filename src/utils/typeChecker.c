#include "../../include/typeChecker.h"

int checkTypes(Tree *tree)
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
                // TODO: MEJORAR MENSAJE; NUMERO DE LINEA; ETC; ETC
                printf("Incorrect types\n");
                exit(1);
            }
        }
    }
    //TODO: EDGAR 
    if (tree->root->flag == ASSIGN)
    {
        int rType = checkTypes(rightChild);
        
    }
    
}
