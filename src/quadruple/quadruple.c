#include "../../include/quadruple.h"

/**
 * Create a new quadruple with the given parameters
 */
/**
 * Creates a new quadruple with the given operator and operands.
 *
 * @param op The operator.
 * @param arg1 The first operand.
 * @param arg2 The second operand.
 * @param result The result node where the operation's output is stored.
 * @return A pointer to the newly created `Quadruple`.
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
 * Creates a new unary quadruple for operations involving a single operand.
 *
 * @param op The operator.
 * @param arg2 The operand involved in the operation.
 * @param result The result node where the operation's output is stored.
 * @return A pointer to the newly created `Quadruple`.
 *
 * Example:
 *  x = 1 or x = -a.
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
 * Creates a new simple quadruple.
 *
 * @param op The operator.
 * @param arg1 The operand.
 * @return A pointer to the newly created `Quadruple`.
 *
 * Example:
 *  For cases like PARAM.
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
 * Prints the details of a given quadruple.
 *
 * @param quad The quadruple to print.
 *
 * Format:
 * - Conditional jumps: Prints `JMPF` followed by the condition and the target label.
 * - Unconditional jumps: Prints `GOTO` followed by the target label.
 * - General operations: Prints the result node, the operator, and the operands (if applicable).
 *
 * Example Output:
 * - `LABEL L1`
 * - `JMPF condition label`
 * - `GOTO L2`
 * - `result = operand1 operator operand2`
 */
void printQuadruple(Quadruple *quad)
{
    if (quad->op == LABEL)
    {
        printf("LABEL %s\n", quad->arg1 ? quad->arg1->name : "");
        return;
    }

    if (quad->op == JMPF)
    {
        printf("JMPF %s %s\n", quad->arg2 ? quad->arg2->name : "", quad->result ? quad->result->name : "");
        return;
    }

    if (quad->op == GOTO)
    {
        printf("GOTO %s\n", quad->arg1 ? quad->arg1->name : "");
        return;
    }

    if (quad->result && quad->result->name[0] != '\0')
    {
        printf("%s = ", quad->result->name);
    }

    if (quad->arg1)
    {
        if (quad->arg1->flag == NUMBER || quad->arg1->flag == BOOL)
        {
            printf("%d ", quad->arg1->value);
        }
        else
        {
            printf("%s ", quad->arg1->name);
        }
    }

    printf("%s ", nodeFlagToString(quad->op));

    if (quad->arg2)
    {
        if (quad->arg2->flag == NUMBER || quad->arg2->flag == BOOL)
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