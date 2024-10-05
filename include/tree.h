#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tag.h"
#include "type.h"

typedef struct Node {
    Tag flag;
    Type type;
    void *value;
    char *name;
    int line_number;
} Node;

typedef struct Tree {
    Node *root;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree* createTree(Node *root, Tree *left, Tree *right);
Node* createNonTerminalNode(Tag flag);
Node* createNode(Tag flag, Type type, void *value, char *name, int line_number);
char* nodeFlagToString(Tag flag);
void printTree(Tree *tree);
void printTreeRecursive(Tree *tree, char *prefix, int isLast);

#endif // TREE_H
