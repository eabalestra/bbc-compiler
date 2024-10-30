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
Quadruple *newUnaryQuadruple(Tag op, Node *arg2, Node *result)
{
    Quadruple *quad = (Quadruple *)malloc(sizeof(Quadruple));
    quad->op = op;
    quad->arg1 = NULL;
    quad->arg2 = arg2;
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
    // VERSION CUADRUPLOS
    // printf("%s %s %s %s\n", nodeFlagToString(quad->op),
    //        quad->arg1 ? quad->arg1->name : "",
    //        quad->arg2 ? quad->arg2->name : "",
    //        quad->result ? quad->result->name : "");

    if (quad->result && quad->result->name[0] != '\0' && quad->op != JMPF && quad->op != GOTO)
    {
        printf("%s = ", quad->result->name);
    }

    if (quad->arg1)
    {
        if (quad->arg1->flag == NUMBER)
        {
            printf("%d ", quad->arg1->value);
        }
        else
        {
            printf("%s ", quad->arg1->name); //
        }
    }

    if (quad->op == JMPF)
    {
        printf("JMPF %s %s\n", quad->arg2 ? quad->arg2->name : "", quad->result ? quad->result->name : "");
        return;
    }

    if (quad->op == LABEL)
    {
        printf("\n");
        return;
    }

    printf("%s ", nodeFlagToString(quad->op));

    if (quad->arg2)
    {
        if (quad->arg2->flag == NUMBER)
        {
            printf("%d", quad->arg2->value);
        }
        else
        {
            printf("%s", quad->arg2->name);
        }
    }

    printf("\n");
}
