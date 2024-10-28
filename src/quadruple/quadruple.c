#include "../../include/quadruple.h"

/**
 * Create a new quadruple with the given parameters
 */
Quadruple *newQuadruple(Tag op, Node *arg1, Node *arg2, Node *result)
{
    Quadruple *quad = (Quadruple *)malloc(sizeof(Quadruple));
    quad->op = op;
    quad->arg1 = arg1;
    quad->arg2 = arg2;
    quad->result = result;
    return quad;
}

/**
 * 
 * For cases like x = 1 or x = -a,
 */
Quadruple *newUnaryQuadruple(Tag op, Node *arg1, Node *result)
{
    Quadruple *quad = (Quadruple *)malloc(sizeof(Quadruple));
    quad->op = op;
    quad->arg1 = arg1;
    quad->arg2 = NULL;
    quad->result = result;
    return quad;
}

/**
 * For cases like PARAM
 */
Quadruple *newSimpleQuadruple(Tag op, Node *arg1)
{
    Quadruple *quad = (Quadruple *)malloc(sizeof(Quadruple));
    quad->op = op;
    quad->arg1 = arg1;
    quad->arg2 = NULL;
    quad->result = NULL;
    return quad;
}

/**
 *
 */
void printQuadruple(Quadruple *quad)
{
    printf("Op: %s, Arg1: %s, Arg2: %s, Result %s\n", nodeFlagToString(quad->op),
           quad->arg1 ? quad->arg1->name : "",
           quad->arg2 ? quad->arg2->name : "",
           quad->result ? quad->result->name : "");
}