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
    SymbolList *current = symbolTable->currentLevel;
    if (current->symbol == NULL)
    {
        current->symbol = newSymbolNode;
    }
    else
    {
        insertSymbolInSymbolNodeList(current, newSymbolNode);
    }
}

Node* lookupInSymbolTable(SymbolTable* symbolTable, char* name)
{
    if (symbolTable == NULL || name == NULL)
    {
        return NULL;
    }

    SymbolList *currentSymbolNodeList = symbolTable->currentLevel;
    while (currentSymbolNodeList->next != NULL) {
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

    int level = getSymbolTableSize(symbolTable);
    SymbolTable *currentTable = symbolTable;
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

int getSymbolTableSize(SymbolTable *symbolTable) {
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
        if (leftChild == NULL || leftChild->name == NULL) {
            printf("ERROR: leftChild or leftChild->name is NULL.\n");
            exit(1);
        }
        if (lookupInSymbolTable(symbolTable, leftChild->name) != NULL)
        {
            printf("ERROR: Variable %s already declared in this scope.\n", leftChild->name);
            exit(1);
        }
        int levels = getSymbolTableSize(symbolTable);
        printf("flag == VARDECL, inserting %s at level %d.\n", leftChild->name, levels);
        insertSymbolInSymbolTable(symbolTable, leftChild);

        buildSymbolTable(symbolTable, tree->left);
        buildSymbolTable(symbolTable, tree->right);
    }
    else if (flag == METHODDECL)
    {
        Node *leftChild = tree->left->root;
        if (leftChild == NULL || leftChild->name == NULL) {
            printf("ERROR: leftChild or leftChild->name is NULL.\n");
            exit(1);
        }
        if (lookupInSymbolTable(symbolTable, leftChild->name) != NULL)
        {
            printf("ERROR: Method %s already declared.\n", leftChild->name);
            exit(1);
        }
        int levels = getSymbolTableSize(symbolTable);
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
        if (paramNode == NULL || paramNode->name == NULL) {
            printf("ERROR: paramNode or paramNode->name is NULL.\n");
            exit(1);
        }
        int levels = getSymbolTableSize(symbolTable);
        printf("flag == PARAM, inserting %s at level %d.\n", paramNode->name, levels);
        insertSymbolInSymbolTable(symbolTable, paramNode);

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
