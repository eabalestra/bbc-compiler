#ifndef QUADRUPLE_LIST_H
#define QUADRUPLE_LIST_H

#include "quadruple.h"

typedef struct QuadrupleLinkedList
{
    Quadruple *quadruple;
    struct QuadrupleLinkedList *next;
} QuadrupleLinkedList;

QuadrupleLinkedList *newQuadrupleLinkedList(Quadruple *quad);
QuadrupleLinkedList *addQuadrupleLinkedList(QuadrupleLinkedList *list, Quadruple *quad);

void printQuadrupleLinkedList(QuadrupleLinkedList *list);

#endif // QUADRUPLE_LIST_H
