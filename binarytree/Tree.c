#include "Tree.h"

Tree* createTree(Node *root, Tree *left, Tree *right) 
{
    Tree *newTree = (Tree *)malloc(sizeof(Tree));
    newTree->root = root;
    newTree->left = left;
    newTree->right = right;

    return newTree;
}

Node* createNode(NodeType flag, char *type, char *value, char *name)
{
    Node *newNode = malloc(sizeof(Node));
    
    newNode->value= value;
    newNode->name= name;
    newNode->flag= flag;
    newNode->type= type;
    
    return newNode;
}

void printTree(Tree *tree)
{
    if (tree == NULL)
    {
        return;
    }
    if (tree->root != NULL)
    {
        printf("Node value: %s\n", tree->root->value);
    }
    printTree(tree->left);
    printTree(tree->right);
}


/* void freeNode(Node *node) {
    if (node == NULL) {
        return;
    }

    // Free dynamically allocated strings
    if (node->value != NULL) {
        free(node->value);
    }
    if (node->name != NULL) {
        free(node->name);
    }
    if (node->type != NULL) {
        free(node->type);
    }

    // Free the Node structure itself
    free(node);
}

// Function to recursively free a Tree
void freeTree(Tree *tree) {
    if (tree == NULL) {
        return;
    }

    // Recursively free the left and right subtrees
    freeTree(tree->left);
    freeTree(tree->right);

    // Free the root Node
    if (tree->root != NULL) {
        freeNode(tree->root);
    }

    // Free the Tree structure itself
    free(tree);
} */