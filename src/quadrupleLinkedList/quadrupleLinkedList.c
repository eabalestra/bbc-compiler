#include "../../include/quadrupleLinkedList.h"

/**
 * Creates a new `QuadrupleLinkedList` node containing the given quadruple.
 *
 * @param quad A pointer to the `Quadruple` to be stored in the linked list node.
 * @return A pointer to the newly created `QuadrupleLinkedList`.
 */
QuadrupleLinkedList *newQuadrupleLinkedList(Quadruple *quad)
{
    QuadrupleLinkedList *list = (QuadrupleLinkedList *)malloc(sizeof(QuadrupleLinkedList));
    list->quadruple = quad;
    list->next = NULL;
    return list;
}

/**
 * Adds a new `Quadruple` to the end of the `QuadrupleLinkedList`.
 *
 * @param list The head of the `QuadrupleLinkedList`. Can be `NULL` to create a new list.
 * @param quad The `Quadruple` to add to the list.
 * @return The head of the updated `QuadrupleLinkedList`.
 */
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

/**
 * Prints all the quadruples stored in a `QuadrupleLinkedList`.
 *
 * @param list The head of the `QuadrupleLinkedList` to print.
 */
void printQuadrupleLinkedList(QuadrupleLinkedList *list)
{
    QuadrupleLinkedList *current = list;
    while (current != NULL)
    {
        printQuadruple(current->quadruple);
        current = current->next;
    }
}