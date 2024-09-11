#include "Tree.h"
#define COUNT 10

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

char* nodeTypeToString(NodeType type)
{
    switch (type) {
        case PROG: return "PROG";
        case BODY: return "BODY";
        case EXPR: return "EXPR";
        case NUMBER: return "NUMBER";
        case ID: return "ID";
        case PLUS: return "PLUS";
        case MULTIPLY: return "MULTIPLY";
        case ASSIGN: return "ASSIGN";
        case COLON: return "COLON";
        case DECLS: return "DECLS";
        case STMTS: return "STMTS";
        case RETURN: return "RETURN";
        case DECL: return "DECL";
        case TYPE: return "TYPE";
        case VAR: return "VAR";
        case BOOL: return "BOOLEAN";
        default: return "UNKNOWN";
    }
}

void printTree(Tree *tree)
{
    if (tree == NULL)
    {
        printf("");
        return;
    }
    printTreeR(tree, 0);
}

void printTreeR(Tree *tree, int space)
{
    if (tree == NULL || tree->root == NULL)
        return;
    space += COUNT;
    printTreeR(tree->right, space);
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s\n", nodeTypeToString(tree->root->flag));
    
    printTreeR(tree->left, space);
}
