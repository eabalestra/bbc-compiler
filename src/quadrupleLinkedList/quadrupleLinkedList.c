#include "../../include/quadrupleLinkedList.h"

/**
 *
 */
QuadrupleLinkedList *newQuadrupleLinkedList(Quadruple *quad)
{
    QuadrupleLinkedList *list = (QuadrupleLinkedList *)malloc(sizeof(QuadrupleLinkedList));
    list->quadruple = quad;
    list->next = NULL;
    return list;
}

QuadrupleLinkedList *addQuadrupleLinkedList(QuadrupleLinkedList *list, Quadruple *quad)
{
    QuadrupleLinkedList *newQuadLinkedList = newQuadrupleLinkedList(quad);
    newQuadLinkedList->next = list;
    return newQuadLinkedList;
}

void printQuadrupleLinkedList(QuadrupleLinkedList *list)
{
    QuadrupleLinkedList *current = list;
    while (current->next != NULL)
    {
        printQuadruple(current->quadruple);
        current = current->next;
    }
}