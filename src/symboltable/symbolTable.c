#include "../../include/symbolTable.h"

/**
 * Allocates memory for and creates a new SymbolTable.
 *
 * @return pointer to the newly created SymbolTable.
 */
SymbolTable *createSymbolTable()
{
    SymbolTable *newSymbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    newSymbolTable->levelData = createSymbolList(NULL);
    newSymbolTable->next = NULL;
    newSymbolTable->previous = NULL;
    newSymbolTable->levels = -1;

    return newSymbolTable;
}

/**
 *
 */
SymbolList *createSymbolList(Node *symbol)
{
    SymbolList *newSymbolList = (SymbolList *)malloc(sizeof(SymbolList));
    newSymbolList->symbol = symbol;
    newSymbolList->next = NULL;
    newSymbolList->size = 1;

    return newSymbolList;
}

/**
 * Searches for a symbol in the provided symbol table.
 *
 * @param symbol to search for in the symbol table.
 * @param table to search within.
 * @return pointer to the symbol if it was found, otherwise returns NULL.
 */
Node *findSymbolNode(char *symbol, SymbolTable *table, int level)
{
    if (symbol == NULL || table->levels == -1)
    {
        printf("Illegal arguments \n");
        return NULL;
    }

    SymbolTable *tableAux = table;

    while (tableAux->levels > level)
    {
        tableAux = tableAux->next;
    }

    for (int i = level; i > 0; i--)
    {
        Node *result = findNodeInLevel(tableAux->levelData, symbol);
        if (result != NULL)
        {
            return result;
        }
        tableAux = tableAux->previous;
    }
    // Tratamiento del level 0
    Node *result = findNodeInLevel(tableAux->levelData, symbol);
    if (result != NULL)
    {
        return result;
    }

    return NULL;
}

/**
 * Given a SymbolList and a symbol, returns the symbol if it exists in
 * said table.
 *
 * @param symbolTableNode to search within.
 * @param symbol to search for.
 * @returns the symbol if it exists on the symbolTableNode, otherwise returns NULL;
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

/**
 * Inserts a symbol into a specific level of the symbol table.
 *
 * @param symbol to be inserted.
 * @param table where the symbol will be inserted.
 * @param level of the symbol table where the symbol will be inserted.
 */
void insertSymbolInSymbolTable(Node *symbol, SymbolTable *table, int level)
{
    if (symbol == NULL || table == NULL || level == -1)
    {
        printf("Incorrect parameters \n");
        return;
    }

    if (level > table->levels)
    {
        printf("Level doesn't exist \n");
        return;
    }

    SymbolTable *tableAux = table;
    while (table->levels > level)
    {
        tableAux = table->next;
    }

    SymbolList *levelAux = tableAux->levelData;
    if (findNodeInLevel(levelAux, symbol->name) != NULL)
    {
        printf("Symbol already exists in level %d\n", level);
        return;
    }
    while (levelAux->next != NULL)
    {
        levelAux = levelAux->next;
    }

    SymbolList *newSymbolList = createSymbolList(symbol);
    levelAux->next = newSymbolList;
    tableAux->levelData->size++;
}

/**
 */
void pushLevelToSymbolTable(SymbolTable *table)
{
    if (table == NULL)
    {
        printf("Symbol table is null \n");
        return;
    }
    SymbolTable *newLevel = createSymbolTable();
    SymbolTable *aux = table;

    while (aux->next != NULL)
    {
        aux = aux->next;
    }

    aux->next = newLevel;
    aux->next->previous = aux;
    table->levels++;
}

/**
 * Removes and frees the last level from a symbol table
 *
 * @param table pointer to the symbol table.
 */
void popLevelFromSymbolTable(SymbolTable *table)
{
    if (table == NULL)
    {
        printf("Symbol table is null \n");
        return;
    }
    if (table->next == NULL)
    {
        free(table->levelData);
        table->levelData = NULL;
        table->levels = -1;
        return;
    }

    SymbolTable *tableAux = table;
    SymbolTable *prev = NULL;

    while (tableAux->next != NULL)
    {
        prev = tableAux;
        tableAux = tableAux->next;
    }

    if (prev != NULL)
    {
        prev->next = NULL;
    }

    table->levels--;
    free(tableAux);
}

void printSymbolTable(SymbolTable *table)
{
    if (table == NULL)
    {
        printf("Empty table \n");
        return;
    }

    SymbolTable *tableAux = table;
    int levelAct = 0;

    printf("\n-- SYMBOL TABLE --\n");
    // while (tableAux != NULL && tableAux->levels >= levelAct)
    while (tableAux != NULL)
    {
        printf("\n-- LEVEL %d --\n", levelAct);
        SymbolList *symbolList = tableAux->levelData;

        while (symbolList != NULL)
        {
            if (symbolList->symbol != NULL)
            {
                printf("NODE %s\n", nodeFlagToString(symbolList->symbol->flag));
                printf("type: %u\n", symbolList->symbol->type);
                printf("name: %s\n", symbolList->symbol->name);
                printf("value: %d\n", symbolList->symbol->value);
                printf("--------------------------------------\n");
            }
            symbolList = symbolList->next;
        }

        tableAux = tableAux->next;
        levelAct++;
    }
}
