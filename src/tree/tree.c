#include "../../include/tree.h"
#define COUNT 10

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
 * Allocates memory for and creates a new Node.
 *
 * @param flag tag indicating the type of node.
 * @param type data type of the node.
 * @param value pointer to the value of the node.
 * @param name of the node.
 * @return pointer to the newly created Node.
 */
Node *createNode(Tag flag, Type type, void *value, char *name, int line_number)
{
    Node *newNode = malloc(sizeof(Node));

    newNode->value = value;
    newNode->name = name;
    newNode->flag = flag;
    newNode->type = type;
    newNode->line_number = line_number;

    return newNode;
}

/**
 * Allocates memory for and creates a new Non Terminal Node.
 *
 * @param flag tag indicating the type of node.
 * @return pointer to the newly created Non Terminal Node.
 */
Node *createNonTerminalNode(Tag flag)
{
    Node *newNode = malloc(sizeof(Node));

    newNode->value = NULL;
    newNode->name = NULL;
    newNode->flag = flag;
    newNode->type = NONTYPE;
    newNode->line_number = -1;

    return newNode;
}

/**
 * Converts a node's tag to a string representation.
 *
 * @param flag the tag to convert.
 * @return String representation of the tag.
 */
char *nodeFlagToString(Tag flag)
{
    switch (flag)
    {
    case PROG:
        return "PROG";
    case BODY:
        return "BODY";
    case EXPR:
        return "EXPR";
    case NUMBER:
        return "NUMBER";
    case ID:
        return "ID";
    case PLUS:
        return "PLUS";
    case MULTIPLY:
        return "MULTIPLY";
    case ASSIGN:
        return "ASSIGN";
    case COLON:
        return "COLON";
    case DECLS:
        return "DECLS";
    case STMTS:
        return "STMTS";
    case RETURN:
        return "RETURN";
    case DECL:
        return "DECL";
    case TYPE:
        return "TYPE";
    case VAR:
        return "VAR";
    case BOOL:
        return "BOOLEAN";
    case MOD:
        return "MOD";
    case MINUS:
        return "MINUS";
    case EXPRLIST:
        return "EXPRLIST";
    case EXTERN:
        return "EXTERN";
    case BLOCK:
        return "BLOCK";
    case METHODCALL:
        return "METHODCALL";
    case DIVISION:
        return "DIVISION";
    case PARAMSLIST:
        return "PARAMSLIST";
    case METHODEND:
        return "METHODEND";
    case METHODDECL:
        return "METHODDECL";
    case VARDECL:
        return "VARDECL";
    case METHODDECLS:
        return "METHODDECLS";
    case NOT:
        return "NOT";
    case EQUALS:
        return "EQUALS";
    case GRATERTHAN:
        return "GRATERTHAN";
    case LESSTHAN:
        return "LESSTHAN";
    case ELSE:
        return "ELSE";
    case THEN:
        return "THEN";
    case IF:
        return "IF";
    case WHILE:
        return "WHILE";
    case OR:
        return "OR";
    case AND:
        return "AND";
    case EMPTY:
        return "EMPTY";
    
    default:
        return "UNKNOWN";
    }
}

void printTree(Tree *tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    printTreeR(tree, "", 1); // Start printing from root
}

void printTreeR(Tree *tree, char *prefix, int isLast) {
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
        printTreeR(tree->left, newPrefix, !hasRightChild);  // Left child (if it's the last, pass `!hasRightChild`)
    }

    if (hasRightChild) {
        printTreeR(tree->right, newPrefix, 1);  // Right child is always the last in its subtree
    }
}