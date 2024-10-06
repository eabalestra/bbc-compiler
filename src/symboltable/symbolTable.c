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
    newSymbolTable->levels = 0;

    return newSymbolTable;
}

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
 * Searches for a symbol in the provided symbol table. 
 * It first starts searching for the symbol at the given level, 
 * if it doesn't exist there, it will search on the previous level
 * and so on until it finds it or it runs out of levels to search in.
 *
 * @param symbol to search for in the symbol table.
 * @param table to search within.
 * @param level in which to search.
 * @return pointer to the symbol if it was found, otherwise returns NULL.
 */
Node *findSymbolNode(char *symbol, SymbolTable *table, int level)
{
    if (symbol == NULL || table->levels == -1 || table->levels < level || level < 0)
    {
        printf("Illegal arguments \n");
        return NULL;
    }

    SymbolTable *tableAux = table;

    int currentLevel = 0;
    while (currentLevel < level)
    {
        currentLevel++;
        tableAux = tableAux->next;
    }

    for (int i = level; i >= 0; i--)
    {
        Node *result = findNodeInLevel(tableAux->levelData, symbol);
        if (result != NULL)
        {
            return result;
        }
        if (i > 0) {
            tableAux = tableAux->previous;
        }
    }

    return NULL;
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

/**
 * Inserts a symbol into a specific level of the symbol table.
 *
 * @param symbol to be inserted.
 * @param table where the symbol will be inserted.
 * @param level of the symbol table where the symbol will be inserted.
 */
void insertSymbolInSymbolTable(Node *symbol, SymbolTable *table, int level)
{
    if (symbol == NULL || table == NULL)
    {
        printf("Illegal arguments \n");
        return;
    }

    if (level > table->levels || level < 0)
    {
        printf("Level doesn't exist \n");
        return;
    }

    SymbolTable *tableAux = table;
    int currentLevel = 0;
    while (currentLevel < level)
    {
        currentLevel++;
        tableAux = tableAux->next;
    }

    SymbolList *levelAux = tableAux->levelData;
    if (findNodeInLevel(levelAux, symbol->name) != NULL)
    {
        printf("Symbol %s already exists in level %d\n", symbol->name, level);
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
 * Adds a new level to the symbol table.
 *
 * @param table pointer to the SymbolTable to which the new level will be added.
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
 * Removes and frees the last level from a symbol table.
 *
 * @param table pointer to the symbol table.
 */
void popLevelFromSymbolTable(SymbolTable *table)
{
    if (table == NULL)
    {
        printf("Symbol table is null\n");
        return;
    }

    if (table->levels <= 0) 
    {
        printf("No more levels to pop\n");
        return;
    }

    SymbolTable *tableAux = table;
    SymbolTable *prev = NULL;

    while (tableAux->next != NULL)
    {
        prev = tableAux;
        tableAux = tableAux->next;
    }

    if (tableAux->levelData != NULL) 
    {
        freeSymbolList(tableAux->levelData);
    }

    if (prev != NULL)
    {
        prev->next = NULL;
    }

    table->levels--;
    free(tableAux);
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
 * Prints the contents of the symbol table, including all levels and their symbols.
 *
 * @param table pointer to the SymbolTable to be printed. 
 */
void printSymbolTable(SymbolTable *table)
{
    if (table == NULL)
    {
        printf("Empty table \n");
        return;
    }

    if (table->levelData == NULL)
    {
        printf("Empty level \n");
        return;
    }

    SymbolTable *tableAux = table;
    int levelAct = 0;

    printf("\n-- SYMBOL TABLE --\n");
    while (tableAux != NULL)
    {
        printf("\n--- LEVEL %d ---\n", levelAct);
        SymbolList *symbolList = tableAux->levelData;

        while (symbolList != NULL)
        {
            if (symbolList->symbol != NULL)
            {
                printf("NODE %s\n", nodeFlagToString(symbolList->symbol->flag));
                printf("type: %u\n", symbolList->symbol->type);
                printf("name: %s\n", symbolList->symbol->name);
                printf("value: %d\n", symbolList->symbol->value);
                printf("---------\n");
            }
            symbolList = symbolList->next;
        }

        tableAux = tableAux->next;
        levelAct++;
    }
}