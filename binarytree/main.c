#include "binarytree.h"

int main() {
    
    Node *left = createNode(nodeTypeInt, "integer", "", "5");
    Node *right = createNode(nodeTypeInt, "integer", "", "8");

    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol)); 
    symbol->value = "+";

    Node *root = createTree(symbol, left, right);

    printf("Binary Tree:\n");
    printTree(root);

    return 0;
}
