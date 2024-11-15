#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"  // Full definition of Node is needed here

typedef struct Tree {
    Node *root;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree* createTree(Node *root, Tree *left, Tree *right);
void printTree(Tree *tree);
void printTreeRecursive(Tree *tree, char *prefix, int isLast);
Node *findNodeInTree(Tree *tree, char *nodeName);
Node *findNodeInTreeRecursive(Tree *tree, char *nodeName);


#endif // TREE_H
