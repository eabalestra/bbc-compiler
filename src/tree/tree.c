#include "tree.h"
#define COUNT 10

/**
 * Allocates memory for and creates a new Tree.
 * 
 * @param root pointer to the root node.
 * @param left pointer to the left subtree.
 * @param right pointer to the right subtree.
 * @return pointer to the newly created Tree.
 */
Tree* createTree(Node *root, Tree *left, Tree *right) 
{
    Tree *newTree = (Tree *)malloc(sizeof(Tree));
    newTree->root = root;
    newTree->left = left;
    newTree->right = right;

    return newTree;
}

/**
 * Allocates memory for and creates a new Node.
 * 
 * @param flag tag indicating the type of node.
 * @param type data type of the node.
 * @param value pointer to the value of the node.
 * @param name of the node.
 * @return pointer to the newly created Node.
 */
Node* createNode(Tag flag, Type type, void *value, char *name)
{
    Node *newNode = malloc(sizeof(Node));
    
    newNode->value = value;
    newNode->name = name;
    newNode->flag = flag;
    newNode->type = type;
    
    return newNode;
}

/**
 * Converts a node's tag to a string representation.
 * 
 * @param flag the tag to convert.
 * @return String representation of the tag.
 */
char* nodeFlagToString(Tag flag)
{
    switch (flag) {
        case PROG: return "PROG";
        case BODY: return "BODY";
        case EXPR: return "EXPR";
        case NUMBER: return "NUMBER";
        case ID: return "ID";
        case PLUS: return "PLUS";
        case MULTIPLY: return "MULTIPLY";
        case ASSIGN: return "ASSIGN";
        case COLON: return "COLON";
        case DECLS: return "DECLS";
        case STMTS: return "STMTS";
        case RETURN: return "RETURN";
        case DECL: return "DECL";
        case TYPE: return "TYPE";
        case VAR: return "VAR";
        case BOOL: return "BOOLEAN";
        default: return "UNKNOWN";
    }
}

/**
 * Prints the entire tree in a structured format.
 * 
 * @param tree pointer to the tree to print.
 */
void printTree(Tree *tree)
{
    if (tree == NULL)
    {
        printf("");
        return;
    }
    printTreeR(tree, 0);
}

/**
 * Recursive function to print the tree with indentation.
 * 
 * @param tree pointer to the tree to print.
 * @param space the current level of indentation.
 */
void printTreeR(Tree *tree, int space)
{
    if (tree == NULL || tree->root == NULL)
        return;
    space += COUNT;
    printTreeR(tree->right, space);
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%s\n", nodeTypeToString(tree->root->flag));
    
    printTreeR(tree->left, space);
}