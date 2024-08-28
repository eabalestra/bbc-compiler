#include "Tree.h"

int main() {
    Node *node1 = createNode("5", "Name1", nodeTypeInt, "int");
    Node *node2 = createNode("8", "Name2", nodeTypeInt, "int");
    Node *node3 = createNode("1", "Name3", nodeTypeInt, "int");

    // Create trees
    Tree *leftTree = createTree(node2, NULL, NULL);
    Tree *rightTree = createTree(node3, NULL, NULL);
    Tree *rootTree = createTree(node1, leftTree, rightTree);

    // Print the tree
    printf("Printing the tree:\n");
    printTree(rootTree);
}
