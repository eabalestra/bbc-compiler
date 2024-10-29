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
    if (list == NULL)
    {
        return newQuadLinkedList;
    }
    QuadrupleLinkedList *current = list;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newQuadLinkedList;
    return list;
}

void printQuadrupleLinkedList(QuadrupleLinkedList *list)
{
    QuadrupleLinkedList *current = list;
    while (current != NULL)
    {
        printQuadruple(current->quadruple);
        current = current->next;
    }
}