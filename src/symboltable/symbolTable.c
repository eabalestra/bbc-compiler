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

    /*  if (tableAux->levelData != NULL)
        {
            freeSymbolList(tableAux->levelData);
        } */

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

/**
 * Performs a semantic analysis on the provided abstract syntax tree (AST)
 * and builds a symbol table.
 *
 * @param ast tree to perform the semantic check on.
 * @return pointer to the constructed SymbolTable after processing the AST.
 */
SymbolTable *semanticCheck(Tree *ast)
{
    SymbolTable *table = createSymbolTable();
    Node *newNode = createNode(PROG, ast->root->type, NULL, "program", NULL);
    insertSymbolInSymbolTable(table, newNode, 0);

    buildSymbolTable(table, ast);
    return table;
}

void buildSymbolTable(SymbolTable *table, Tree *tree)
{
    Tag flag = tree->root->flag;
    if (tree == NULL || tree->root == NULL)
    {
        return NULL; // o return; para cortar la recursión.
    }
    if (flag == ID) {
        if (findSymbolNode(tree->root->name, table, table->levels) != NULL)
        {
            printf("Var already declared.\n");
            exit(1);
        }
        insertSymbolInSymbolTable(tree->root, table, table->levels);
        return;
    }
    if (flag == METHODDECL) {
        Node *node = tree->left;
        // buildSymbolTable (table ,tree->left)
        pushLevelToSymbolTable(table);
        insertSymbolInSymbolTable(node, table, table->levels);
        popLevelFromSymbolTable(table);
        return;
    }
    if (flag == BLOCK) {
        pushLevelToSymbolTable(table);
        if (tree->left != NULL)
        {
            buildSymbolTable(table, tree->left);
        }
        if (tree->right != NULL)
        {
            buildSymbolTable(table, tree->right);
        }
        popLevelFromSymbolTable(table);
        return;
    }
    buildSymbolTable(table, tree->left);
    buildSymbolTable(table, tree->right);


    /* switch (flag)
    {
    case (METHODEND):

        break;
    case (ID): // diferenciar los id de los parametros del metodo
        if (findSymbolNode(tree->root->name, table, table->levels) != NULL)
        {
            printf("Var already declared.\n");
            exit(1);
        }
        insertSymbolInSymbolTable(tree->root, table, table->levels);
        break;
    case (METHODDECL):
        Node *node = tree->left;
        // buildSymbolTable (table ,tree->left)
        pushLevelToSymbolTable(table);
        insertSymbolInSymbolTable(node, table, table->levels);
        popLevelFromSymbolTable(table);
        break;
    case (BLOCK):
        pushLevelToSymbolTable(table);
        if (tree->left != NULL)
        {
            buildSymbolTable(table, tree->left);
        }
        if (tree->right != NULL)
        {
            buildSymbolTable(table, tree->right);
        }
        popLevelFromSymbolTable(table);
        return;
        break;
    default:
        buildSymbolTable(table, tree->left);
        buildSymbolTable(table, tree->right);
        break;
    } */
}
// void *interpret(Tree * tree, SymbolTable * table)
// {
//     if (tree == NULL || tree->root == NULL)
//     {
//         return NULL;
//     }
//     if (tree->root->flag == ID)
//     {
//         // return findSymbol(table, tree->root->value)->value;
//     }
//     if (tree->root->flag == NUMBER)
//     {
//         return tree->root->value;
//     }
//     if (tree->root->flag == RETURN)
//     {
//         void *hi_value = interpret(tree->left, table);
//         Node *node = findSymbol(table, "main");
//         node->value = hi_value;

//         printf("RETURN %u\n", node->value);
//         return NULL;
//     }
//     if (tree->root->flag == PLUS)
//     {
//         void *hi_value = interpret(tree->left, table);
//         void *hd_value = interpret(tree->right, table);
//         int hi = ((int *)hi_value);
//         int hd = ((int *)hd_value);
//         int result = hi + hd;

//         return result;
//     }
//     if (tree->root->flag == MULTIPLY)
//     {
//         void *hi_value = interpret(tree->left, table);
//         void *hd_value = interpret(tree->right, table);
//         int hi = ((int *)hi_value);
//         int hd = ((int *)hd_value);
//         int result = hi * hd;

//         return result;
//     }
//     if (tree->root->flag == ASSIGN)
//     {
//         Node *node = findSymbol(table, tree->left->root->value);
//         if (node == NULL)
//         {
//             return NULL;
//         }
//         node->value = interpret(tree->right, table);
//         return NULL;
//     }
//     if (tree->root->flag == COLON)
//     {
//         return interpret(tree->left, table);
//     }
//     if (tree->root->flag == STMTS)
//     {
//         interpret(tree->left, table);
//         interpret(tree->right, table);
//     }

//     return NULL;
// }