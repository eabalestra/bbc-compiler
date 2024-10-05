#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

typedef struct Tree {
    Node *root;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree* createTree(Node *root, Tree *left, Tree *right);
void printTree(Tree *tree);
void printTreeRecursive(Tree *tree, char *prefix, int isLast);

#endif // TREE_H
