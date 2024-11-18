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
void printTree(Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
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
void printTreeRecursive(Tree *tree, char *prefix, int isLast)
{
    if (tree == NULL || tree->root == NULL)
        return;

    printf("%s", prefix);
    printf(isLast ? "└── " : "├── ");
    printf("%s\n", nodeFlagToString(tree->root->flag));

    char newPrefix[1000];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "│   ");

    int hasLeftChild = tree->left != NULL;
    int hasRightChild = tree->right != NULL;

    if (hasLeftChild)
    {
        printTreeRecursive(tree->left, newPrefix, !hasRightChild);
    }

    if (hasRightChild)
    {
        printTreeRecursive(tree->right, newPrefix, 1);
    }
}

/**
 * Searches for a node with a specified name in a tree.
 *
 * @param tree a pointer to the tree to search in.
 * @param nodeName a string containing the name of the node to search for.
 * @return a pointer to the node with the matching name, or NULL if no match is found.
 */
Node *findNodeInTree(Tree *tree, char *nodeName)
{
    if (tree == NULL || tree->root == NULL)
    {
        return NULL;
    }

    if (strcmp(tree->root->name, nodeName) == 0)
    {
        return tree->root;
    }

    Node *leftResult = findNodeInTree(tree->left, nodeName);
    if (leftResult != NULL)
    {
        return leftResult;
    }

    Node *rightResult = findNodeInTree(tree->right, nodeName);
    if (rightResult != NULL)
    {
        return rightResult;
    }

    return NULL;
}

/**
 * Checks whether a method node in the binary tree is marked as external.
 *
 * @param pTree a pointer to the tree to check.
 * @return 1 if the method node is external, 0 otherwise.
 */
int checkIfMethodIsExternal(Tree *pTree)
{
    if (pTree == NULL || pTree->root == NULL) {
        return 0;
    }
    Node *methodNode = pTree->root;
    if (methodNode->isExternal == 1) {
        return 1;
    }
    return checkIfMethodIsExternal(pTree->right);;
}