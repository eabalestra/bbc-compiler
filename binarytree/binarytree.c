#include "binarytree.h"

Node *createNode(NodeType flag, char *type, char *name, char *value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode->symbol = (Symbol *)malloc(sizeof(Symbol));

    // newNode->symbol->flag = flag;
    // newNode->symbol->value = value;
    // newNode->symbol->name = name;
    // newNode->symbol->type = type;

    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Node *createNodeWithSymbol(int symbol) 
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->symbol = symbol;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
} 

void printTree(Node *node)
{
    if (node == NULL)
    {
        return;
    }

    // printf("flag: %d \n", node->symbol->flag);
    // printf("type: %s \n", node->symbol->type);
    // printf("value: %s \n", node->symbol->value);

    printTree(node->left);
    printTree(node->right);
}

Node *createTree(int symbol, Node *left, Node *right)
{
    Node *root = (Node *)malloc(sizeof(Node));
    root->symbol = symbol;
    root->left = left;
    root->right = right;
    return root;
}