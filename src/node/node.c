#include "../../include/node.h"

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
    case PARAM:
        return "PARAM";
    default:
        return "UNKNOWN";
    }
}

char *nodeTypeToString(Type type)
{
    switch (type)
    {
    case INTEGER:
        return "integer";
    case BOOLEAN:
        return "bool";
    case VOID:
        return "void";
    case NONTYPE:
        return "nontype";
    default:
        return "UNKNOWN";
    }
}