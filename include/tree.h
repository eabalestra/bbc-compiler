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
} Node;

typedef struct Tree {
    Node *root;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree* createTree(Node *root, Tree *left, Tree *right);
Node* createNode(Tag flag, Type type, void *value, char *name);
char* nodeFlagToString(Tag flag);
void printTree(Tree *tree);
void printTreeR(Tree *tree, int space);

#endif // TREE_H