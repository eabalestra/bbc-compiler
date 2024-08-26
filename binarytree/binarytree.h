#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum NodeType {
    nodeTypeProg,         // represents a program
    nodeTypeBody,         // represents a body
    nodeTypeAssignment,   // represents an assignment
    nodeTypeExpr,         // represents an expression
    nodeTypeInt           // represents an integer
} NodeType;

typedef struct Symbol {
    NodeType flag;
    char *type;
    char *name;
    char *value;
} Symbol;

typedef struct Node {
    struct Node *left;
    struct Node *right;
    int symbol;
} Node;


Node* createNode(NodeType flag, char *type, char *name, char *value);
Node* createNodeWithSymbol(int symbol);
void printTree(Node *node);
Node* createTree(int symbol, Node *left, Node *right);

#endif // BINARY_TREE_H