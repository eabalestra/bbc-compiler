#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enum.h"

typedef enum NodeType {
    PROG, BODY, EXPR, NUMBER, ID, PLUS, MULTIPLY,
    ASSIGN, COLON, DECLS, STMTS, RETURN, DECL, TYPE
} NodeType;

typedef struct Node {
    NodeType flag;
    Type type;
    void *value;
    char *name;
} Node;

typedef struct Tree {
    Node *root;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree* createTree(Node *root, Tree *left, Tree *right);
Node* createNode(NodeType flag, Type type, void *value, char *name);
void printTree(Tree *tree, int space);

#endif // TREE_H

