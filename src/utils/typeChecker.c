#include "../../include/typeChecker.h"

void checkTypes(Tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        return;
    }

    Tree *lTree = tree->left;
    Tree *rTree = tree->right;
    Tag rootTag = tree->root->flag;

    if ((rootTag == VARDECL || rootTag == ASSIGN) ||
        (rootTag == AND || rootTag == OR) ||
        (rootTag == PLUS || rootTag == MINUS || rootTag == MULTIPLY || rootTag == DIVISION || rootTag == MOD)) {

        Type leftChildType = getTreeType(tree->left);
        Type rightChildType;

        if (rTree == NULL)
        {
            rightChildType = leftChildType;
        }
        else
        {
            rightChildType = getTreeType(tree->right);
        }

        // TODO VER ESTA PARTE
        if (leftChildType != rightChildType)
        {
            Node *lChildNode = tree->left->root;
            printf("Type Error [Line %d]: Variable '%s' is declared as type '%s', "
                   "but is assigned a value of incompatible type '%s'.\n",
                   lChildNode->line_number, lChildNode->name,
                   nodeTypeToString(leftChildType), nodeTypeToString(rightChildType));
            exit(1);
        }
    }
    if (rootTag == GRATERTHAN || rootTag == LESSTHAN || rootTag == EQUALS) {
        // chequear tipo de los hijos
    }
    if (rootTag == RETURN) {

    }
    if (rootTag == MINUS) // UNARIOS
    {

    }
    if (rootTag == METHODCALL) {

    }

    checkTypes(tree->left);
    checkTypes(tree->right);
}
