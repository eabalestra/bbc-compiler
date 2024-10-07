#include "../../include/symbolList.h"

/**
 * Allocates memory for and creates a new SymbolList.
 *
 * @param symbol pointer to the symbol to be added to the new list.
 * @return pointer to the newly created SymbolList.
 */
SymbolList *createSymbolList(Node *symbol)
{
    SymbolList *newSymbolList = (SymbolList *)malloc(sizeof(SymbolList));
    newSymbolList->symbol = symbol;
    newSymbolList->next = NULL;
    newSymbolList->size = 0;

    return newSymbolList;
}

/**
 * Frees the memory allocated for a SymbolList.
 *
 * @param symbolList pointer to the head of the SymbolList to be freed.
 *
 * Note: After calling this function, the caller should ensure
 *       that the pointer to the head of the list is set to NULL
 *       to avoid dangling pointers.
 */
void freeSymbolList(SymbolList *symbolList)
{
    while (symbolList != NULL)
    {
        SymbolList *next = symbolList->next;
        free(symbolList);
        symbolList = next;
    }
}

/**
 * Given a SymbolList and a symbol, returns the symbol if it exists in
 * said table.
 *
 * @param symbolList to search within.
 * @param symbol to search for.
 * @return a pointer to the symbol if it exists in the SymbolList; otherwise, returns NULL
 */
Node *findNodeInLevel(SymbolList *symbolList, char *symbol)
{
    if (symbolList == NULL || symbol == NULL)
    {
        printf("Illegal arguments \n");
        return NULL;
    }

    SymbolList *levelAux = symbolList;
    while (levelAux != NULL)
    {
        if (levelAux->symbol != NULL)
        {
            if (strcmp(levelAux->symbol->name, symbol) == 0)
            {
                return levelAux->symbol;
            }
        }
        levelAux = levelAux->next;
    }
    return NULL;
}