#include "Tree.h"

Tree* createTree(Node *root, Tree *left, Tree *right) 
{
    Tree *newTree = (Tree *)malloc(sizeof(Tree));
    newTree->root = root;
    newTree->left = left;
    newTree->right = right;

    return newTree;
}

Node* createNode(NodeType flag, Type type, void *value, char *name)
{
    Node *newNode = malloc(sizeof(Node));
    
    newNode->value = value;
    newNode->name = name;
    newNode->flag = flag;
    newNode->type = type;
    
    return newNode;
}

void printTree(Tree *tree)
{
    if (tree == NULL)
    {
        return;
    }
    printf("ROOT: \n");
    if(tree->root->flag == 0){
        printf("FLAG: PROG\n");
    
    }
    if(tree->root->flag == 1){
        printf("FLAG: BODY\n");
    
    }
    if(tree->root->flag == 2){
        printf("FLAG: EXPR\n");
    
    }
    if(tree->root->flag == 3){
        printf("FLAG: NUMBER\n");
    
    }
    if(tree->root->flag == 4){
        printf("FLAG: ID\n");
    
    }
    
    if(tree->root->flag == 5){
        printf("FLAG: PLUS \n");
    
    }
    
    if(tree->root->flag == 6){
        printf("FLAG: MULTIPLY\n");
    
    }
    if(tree->root->flag == 7){
        printf("FLAG: ASSIGN\n");
    
    }
    //printf("FLAG: %d \n", tree->root->flag);
    printf("HIZO IZQUIERDO: \n");
    printTree(tree->left);
    printf("HIZO DERECHO: \n");
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