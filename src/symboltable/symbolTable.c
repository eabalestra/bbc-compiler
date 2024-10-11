#include "../../include/symbolTable.h"

/**
 * Allocates memory for and creates a new SymbolTable.
 *
 * @return pointer to the newly created SymbolTable.
 */
SymbolTable *createSymbolTable()
{
    SymbolTable *newSymbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    newSymbolTable->currentLevel = createSymbolList(NULL);
    newSymbolTable->parent = NULL;

    return newSymbolTable;
}

SymbolTable *pushLevelToSymbolTable(SymbolTable* symbolTable)
{
    SymbolTable *newTable = NULL;
    newTable = (SymbolTable *) malloc(sizeof(SymbolTable));
    newTable->parent = symbolTable;
    newTable->currentLevel = createSymbolList(NULL);

    return newTable;
}

SymbolTable* popLevelFromSymbolTable(SymbolTable* symbolTable)
{
    SymbolTable *parent = symbolTable->parent;
    //free(symbolTable->currentLevel);
    //free(symbolTable);

    return parent;
}

void insertSymbolInSymbolTable(SymbolTable* symbolTable, Node* newSymbolNode)
{
    SymbolList *current = NULL;
    current = symbolTable->currentLevel;
    if (current->symbol == NULL)
    {
        symbolTable->currentLevel->symbol = newSymbolNode;
        return;
    }
    insertSymbolInSymbolNodeList(current, newSymbolNode);
}

Node* lookupInSymbolTable(SymbolTable* symbolTable, char* name)
{
    if (symbolTable == NULL) {
        return NULL;
    }

    SymbolList *currentSymbolNodeList = symbolTable->currentLevel;
    while (currentSymbolNodeList != NULL) {
        if (strcmp(currentSymbolNodeList->symbol->name, name) == 0) {
            return currentSymbolNodeList->symbol;
        }
        currentSymbolNodeList = currentSymbolNodeList->next;
    }

    return lookupInSymbolTable(symbolTable->parent, name);
}

/**
 * Prints the contents of the symbol symbolTable, including all levels and their symbols.
 *
 * @param symbolTable pointer to the SymbolTable to be printed.
 */
void printSymbolTable(SymbolTable* symbolTable)
{
    if (symbolTable == NULL)
    {
        printf("SymbolTable is NULL\n");
        return;
    }

    int level = 0;
    SymbolTable *currentTable = symbolTable;
    while (currentTable->parent != NULL)
    {
        level++;
        currentTable = currentTable->parent;
    }

    currentTable = symbolTable;
    while (currentTable != NULL)
    {
        printf("Level %d:\n", level);
        SymbolList *currentList = currentTable->currentLevel;

        while (currentList != NULL)
        {
            if (currentList->symbol != NULL)
            {
                printf("  Symbol: %s\n", currentList->symbol->name);
            }
            currentList = currentList->next;
        }

        currentTable = currentTable->parent;
        level--;
    }
}

int getCurrentLevel(SymbolTable *symbolTable) {
    int level = 0;
    while (symbolTable->parent != NULL) {
        level++;
        symbolTable = symbolTable->parent;
    }
    return level;
}


SymbolTable *semanticCheck(SymbolTable *symbolTable, Tree *tree)
{
    buildSymbolTable(symbolTable, tree);
    return symbolTable;
}

void buildSymbolTable(SymbolTable *symbolTable, Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }
    Tag flag = tree->root->flag;

    if (flag == VARDECL) {
        Node *leftChild = tree->left->root;
        int levels = getCurrentLevel(symbolTable);
        printf("flag == VARDECL, inserting %s at level %d.\n", leftChild->name, levels);
        insertSymbolInSymbolTable(symbolTable, leftChild);

        buildSymbolTable(symbolTable, tree->left);
        buildSymbolTable(symbolTable, tree->right);
    }
    else if (flag == METHODDECL)
    {
        Node *leftChild = tree->left->root;

        int levels = getCurrentLevel(symbolTable);
        printf("flag == METHODDECL, inserting %s at level %d.\n", leftChild->name, levels);
        insertSymbolInSymbolTable(symbolTable, leftChild);

        symbolTable = pushLevelToSymbolTable(symbolTable);
        if (tree->right != NULL)
        {
            buildSymbolTable(symbolTable, tree->right);
        }
        symbolTable = popLevelFromSymbolTable(symbolTable);
    }
    else if (flag == PARAM)
    {
        Node *paramNode = tree->root;
        int levels = getCurrentLevel(symbolTable);
        printf("flag == PARAM, inserting %s at level %d.\n", paramNode->name, levels);
        insertSymbolInSymbolTable(symbolTable, paramNode);

        buildSymbolTable(symbolTable, tree->left);
        buildSymbolTable(symbolTable, tree->right);
    }
    else if (flag == METHODEND)
    {
        buildSymbolTable(symbolTable, tree->left);
        buildSymbolTable(symbolTable, tree->right);
    }
    else if (flag == BLOCK)
    {
        symbolTable = pushLevelToSymbolTable(symbolTable);

        buildSymbolTable(symbolTable, tree->left);
        buildSymbolTable(symbolTable, tree->right);

        symbolTable = popLevelFromSymbolTable(symbolTable);
    }
    else
    {
        buildSymbolTable(symbolTable, tree->left);
        buildSymbolTable(symbolTable, tree->right);
    }
}