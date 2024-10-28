#ifndef QUADRUPLES_H
#define QUADRUPLES_H

#include "node.h"

// op, arg1, arg2 y result
typedef struct Quadruple
{
    Tag op;
    Node *arg1;
    Node *arg2;
    Node *result;
} Quadruple;

Quadruple *newQuadruple(Tag op, Node *arg1, Node *arg2, Node *result);
Quadruple *newUnaryQuadruple(Tag op, Node *arg1, Node *result);
Quadruple *newSimpleQuadruple(Tag op, Node *arg1);

void printQuadruple(Quadruple *quad);

#endif // QUADRUPLES_H