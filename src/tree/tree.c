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

void printTree(Tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    printTreeRecursive(tree, "", 1); // Start printing from root
}

void printTreeRecursive(Tree *tree, char *prefix, int isLast) {
    if (tree == NULL || tree->root == NULL)
        return;

    // Print the current node
    printf("%s", prefix);
    printf(isLast ? "└── " : "├── ");
    printf("%s\n", nodeFlagToString(tree->root->flag));

    // Prepare the new prefix for children
    char newPrefix[1000];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "│   ");

    // If there are child nodes, print them recursively
    int hasLeftChild = tree->left != NULL;
    int hasRightChild = tree->right != NULL;

    if (hasLeftChild) {
        printTreeRecursive(tree->left, newPrefix, !hasRightChild);  // Left child (if it's the last, pass `!hasRightChild`)
    }

    if (hasRightChild) {
        printTreeRecursive(tree->right, newPrefix, 1);  // Right child is always the last in its subtree
    }
}