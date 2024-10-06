#include "../../include/tree.h"

/**
 * Allocates memory for and creates a new Tree.
 *
 * @param root pointer to the root node.
 * @param left pointer to the left subtree.
 * @param right pointer to the right subtree.
 * @return pointer to the newly created Tree.
 */
Tree *createTree(Node *root, Tree *left, Tree *right)
{
    Tree *newTree = (Tree *)malloc(sizeof(Tree));
    newTree->root = root;
    newTree->left = left;
    newTree->right = right;

    return newTree;
}

/**
 * Prints the structure of a binary tree.
 * 
 * @param tree pointer to the tree to print.
 */
void printTree(Tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    printTreeRecursive(tree, "", 1);
}

/**
 * Recursively prints the structure of a binary tree.
 * 
 * @param tree pointer to the tree to print.
 * @param prefix to print before the node (to maintain visual hierarchy).
 * @param isLast indicates whether the current node is the last child of its parent.
 */
void printTreeRecursive(Tree *tree, char *prefix, int isLast) {
    if (tree == NULL || tree->root == NULL)
        return;

    printf("%s", prefix);
    printf(isLast ? "└── " : "├── ");
    printf("%s\n", nodeFlagToString(tree->root->flag));

    char newPrefix[1000];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "│   ");

    int hasLeftChild = tree->left != NULL;
    int hasRightChild = tree->right != NULL;

    if (hasLeftChild) {
        printTreeRecursive(tree->left, newPrefix, !hasRightChild); 
    }

    if (hasRightChild) {
        printTreeRecursive(tree->right, newPrefix, 1); 
    }
}