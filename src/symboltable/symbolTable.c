#include "../../include/symbolTable.h"
#include "../../include/typeChecker.h"

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
Node *findSymbolNode(SymbolTable *table, char *symbol, int level)
{

    if (symbol == NULL || table->levels == -1 || table->levels < level || level < 0)
    {
        printf("findSymbolNode: Illegal arguments \n");
        exit(1);
    }

    // printf("name del id es null? %s\n", symbol);
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
        if (i > 0)
        {
            tableAux = tableAux->previous;
        }
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
void insertSymbolInSymbolTable(SymbolTable *table, Node *symbol, int level)
{
    if (symbol == NULL || table == NULL)
    {
        printf("insertSymbolInSymbolTable: Table or Symbol are null\n");
        exit(1);
    }
    if (level > table->levels || level < 0)
    {
        printf("insertSymbolInSymbolTable: Level %d doesn't exist. Current levels: %d \n", level, table->levels);
        exit(1);
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
        printf("insertSymbolInSymbolTable: symbol %s already exists in level %d\n", symbol->name, level);
        exit(1);
    }
    while (levelAux->next != NULL)
    {
        levelAux = levelAux->next;
    }

    SymbolList *newSymbolList = createSymbolList(symbol);
    levelAux->next = newSymbolList;
    tableAux->levelData->size++;
    // printSymbolTable(table);
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
        printf("pushLevelToSymbolTable: Symbol table is null \n");
        exit(1);
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
        printf("popLevelFromSymbolTable: Symbol table is null\n");
        return;
    }
    if (table->levels <= 0)
    {
        printf("popLevelFromSymbolTable: No more levels to pop\n");
        return;
    }

    SymbolTable *tableAux = table;
    SymbolTable *prev = NULL;

    while (tableAux->next != NULL)
    {
        prev = tableAux;
        tableAux = tableAux->next;
    }

    //  if (tableAux->levelData != NULL)
    //  {
    //      freeSymbolList(tableAux->levelData);
    //  }

    if (prev != NULL)
    {
        prev->next = NULL;
    }

    table->levels--;
    //    free(tableAux);
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
        printf("printSymbolTable: Empty table \n");
        return;
    }

    if (table->levelData == NULL)
    {
        printf("printSymbolTable: Empty level \n");
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

/**
 * Performs a semantic analysis on the provided abstract syntax tree (AST)
 * and builds a symbol table.
 *
 * @param ast tree to perform the semantic check on.
 * @return pointer to the constructed SymbolTable after processing the AST.
 */
SymbolTable *semanticCheck(SymbolTable *table, Tree *ast)
{
    buildSymbolTable(table, ast);
    checkTypes(ast);
    return table;
}

/**
 * Traverses the provided AST and builds the symbol table.
 *
 * @param table pointer to the symbol table to be built.
 * @param tree pointer to the AST tree representing the program structure.
 */
void buildSymbolTable(SymbolTable *table, Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }

    Tag flag = tree->root->flag;

    if (flag == VARDECL)
    {
        Node *leftChild = tree->left->root;
        if (leftChild->name != NULL)
        {
            printf("flag == VARDECL, insertando un %s en el level %d.\n", leftChild->name, table->levels);
            insertSymbolInSymbolTable(table, leftChild, table->levels);
        }
        else
        {
            printf("Error: tree->root->name is NULL\n");
            exit(1);
        }

        // actualizar el nodo en el arbol
    }
    else if (flag == METHODDECL)
    {
        Node *leftChild = tree->left->root;
        printf("flag == METHODDECL, insertando un %s en el level %d.\n", leftChild->name, table->levels);
        insertSymbolInSymbolTable(table, leftChild, table->levels);
        buildSymbolTable(table, tree->right);
    }
    else if (flag == METHODEND)
    {
        pushLevelToSymbolTable(table);
        Node *leftChild = tree->left->root;
        if (leftChild->flag == ID)
        {
            printf("flag == METHODEND, insertando un %s en el level %d.\n", leftChild->name, table->levels);
            insertSymbolInSymbolTable(table, leftChild, table->levels);
        }
        buildSymbolTable(table, tree->left);
        buildSymbolTable(table, tree->right);

        popLevelFromSymbolTable(table);
    }

    else if (flag == PARAMSLIST)
    {

        Node *leftChild = tree->left->root;
        Node *rightChild = tree->right->root;

        if (leftChild->flag == ID)
        {
            printf("flag == PARAMSLIST, insertando un %s en el level %d.\n", leftChild->name, table->levels);
            insertSymbolInSymbolTable(table, leftChild, table->levels);
        }
        if (rightChild->flag == ID)
        {
            printf("flag == PARAMSLIST, insertando un %s en el level %d.\n", rightChild->name, table->levels);
            insertSymbolInSymbolTable(table, rightChild, table->levels);
        }
        buildSymbolTable(table, tree->left);
        buildSymbolTable(table, tree->right);
    }

    else if (flag == BLOCK)
    {
        buildSymbolTable(table, tree->left);
        buildSymbolTable(table, tree->right);
    }
    else if (flag == THEN || flag == ELSE)
    {
        pushLevelToSymbolTable(table);
        buildSymbolTable(table, tree->left);
        popLevelFromSymbolTable(table);
        buildSymbolTable(table, tree->right);
    }
    else if (flag == WHILE)
    {
        pushLevelToSymbolTable(table);
        buildSymbolTable(table, tree->left);
        buildSymbolTable(table, tree->right);
        popLevelFromSymbolTable(table);
    }
    else
    {
        buildSymbolTable(table, tree->left);
        buildSymbolTable(table, tree->right);
    }
}