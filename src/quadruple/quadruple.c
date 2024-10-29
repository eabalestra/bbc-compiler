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
    quad->arg2 = arg1;
    quad->arg1 = NULL;
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
    // printf("%s %s %s %s\n", nodeFlagToString(quad->op),
    //        quad->arg1 ? quad->arg1->name : "",
    //        quad->arg2 ? quad->arg2->name : "",
    //        quad->result ? quad->result->name : "");

    printf("%s = %s %s %s\n", quad->result ? quad->result->name : "", quad->arg1 ? quad->arg1->name : "", nodeFlagToString(quad->op), quad->arg2 ? quad->arg2->name : "");
}



/* void printQuadruple(Quadruple *quad)
{
    printf("%s = ", quad->result ? quad->result->name : "");

    if (quad->arg1) {
        if (quad->arg1->flag == ID) {
            printf("%s ", quad->arg1->name);
        } else if (quad->arg1->flag == BOOLEAN) {
            printf("%s ", quad->arg1->value); 
        } else if (quad->arg1->flag == NUMBER) {
            printf("%d ", quad->arg1->value); 
        }
    }

    printf("%s ", nodeFlagToString(quad->op));

    if (quad->arg2) {
        if (quad->arg2->flag == ID) {
            printf("%s", quad->arg2->name);
        } else if (quad->arg1->flag == BOOLEAN) {
            printf("%s ", quad->arg1->value);
        } else if (quad->arg2->flag == NUMBER) {
            printf("%d", quad->arg2->value);
        }
    }

    printf("\n");
} */