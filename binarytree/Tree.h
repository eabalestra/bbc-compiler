#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum NodeType {
    nodeTypeProg,           // represents a program
    nodeTypeBody,           // represents a body
    nodeTypeAssignment,     // represents an assignment
    nodeTypeExpr,           // represents an expression
    nodeTypeInt,            // represents an integer
    nodeTypeId,
    nodeTypePlus,
    nodeTypeMultiply,
    nodeTypeDecl
} NodeType;

typedef struct Node {
    char *value;
    char *name;
    NodeType flag;
    char *type;
} Node;

typedef struct Tree {
    Node *root;
    struct Tree *left;
    struct Tree *right;
} Tree;

/* Node* createNode(NodeType flag, char *type, char *name, char *value);
Node* createNodeWithSymbol(int symbol);
void printTree(Node *node);
Node* createTree(int symbol, Node *left, Node *right); */

Tree* createTree(Node *root, Tree *left, Tree *right);
Node* createNode(char *value, char *name, NodeType flag, char *type);
void printTree(Tree *tree);
//void freeTree(Tree *tree);

#endif // TREE_H

