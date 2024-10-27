#include "../../include/symbolTable.h"

void checkForDuplicateParameters(Tree *tree, const Node *leftChild);
void handleVarDecl(SymbolTable *table, Tree *tree);
void handleMethodDecl(SymbolTable *table, Tree *tree);
void handleMethodCall(SymbolTable *table, Tree *tree);
void handleThenOrElse(SymbolTable *table, Tree *tree);
void handleWhile(SymbolTable *table, Tree *tree);
void handleAssign(SymbolTable *table, Tree *tree);
void handleCondition(SymbolTable *table, Tree *tree);

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
Node *searchSymbolInTable(SymbolTable *table, char *symbol, int level)
{

    if (symbol == NULL || table->levels == -1 || table->levels < level || level < 0)
    {
        printf("searchSymbolInTable: Illegal arguments \n");
        exit(1);
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
    printSymbolTable(table);
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

    printf("\n+------------------ SYMBOL TABLE ------------------+\n");
    while (tableAux != NULL)
    {
        printf("\n+-------------------- LEVEL %d --------------------+\n", levelAct);
        SymbolList *symbolList = tableAux->levelData;

        printf("| %-10s | %-10s | %-10s | %-10s |\n", "NODE", "TYPE", "NAME", "VALUE");
        printf("+------------+------------+------------+------------+\n");

        while (symbolList != NULL)
        {
            if (symbolList->symbol != NULL)
            {
                printf("| %-10s | %-10s | %-10s | %-10d |\n",
                       nodeFlagToString(symbolList->symbol->flag),
                       nodeTypeToString(symbolList->symbol->type),
                       symbolList->symbol->name,
                       symbolList->symbol->value);
            }
            symbolList = symbolList->next;
        }

        printf("+------------+------------+------------+------------+\n");

        tableAux = tableAux->next;
        levelAct++;
    }
    printf("+--------------------------------------------------+\n");
}

/**
 * Performs a semantic analysis on the provided abstract syntax tree (AST)
 * and builds a symbol table.
 *
 * @param ast tree to perform the semantic checkForDuplicateParameters on.
 * @return pointer to the constructed SymbolTable after processing the AST.
 */
SymbolTable *semanticCheck(SymbolTable *table, Tree *ast)
{
    buildSymbolTable(table, ast);
    printSymbolTable(table);
    checkTypes(ast);

    return table;
}

/**
 * Traverses the provided AST and builds the symbol pTable.
 *
 * @param pTable pointer to the symbol pTable to be built.
 * @param pTree pointer to the AST pTree representing the program structure.
 */
void buildSymbolTable(SymbolTable *pTable, Tree *pTree)
{
    if (pTree == NULL || pTree->root == NULL)
    {
        return;
    }

    Tag flag = pTree->root->flag;

    switch (flag)
    {
        case VARDECL:
            handleVarDecl(pTable, pTree);
            break;
        case PARAM:
            insertSymbolInSymbolTable(pTable, pTree->root, pTable->levels);
            pTree->root = searchSymbolInTable(pTable, pTree->root->name, pTable->levels);
            buildSymbolTable(pTable, pTree->left);
            break;
        case METHODDECL:
            handleMethodDecl(pTable, pTree);
            break;
        case METHODCALL:
            handleMethodCall(pTable, pTree);
            break;
        case METHODEND:
            pushLevelToSymbolTable(pTable);
            buildSymbolTable(pTable, pTree->left);
            buildSymbolTable(pTable, pTree->right);
            popLevelFromSymbolTable(pTable);
            break;
        case THEN:
        case ELSE:
            handleThenOrElse(pTable, pTree);
            break;
        case IF:
        case WHILE:
            handleWhile(pTable, pTree);
            break;
        case ASSIGN:
            handleAssign(pTable, pTree);
            break;
        case GRATERTHAN:
        case LESSTHAN:
        case MINUS:
        case MULTIPLY:
        case MOD:
        case DIVISION:
        case PLUS:
            handleCondition(pTable, pTree);
            break;
        default:
            buildSymbolTable(pTable, pTree->left);
            buildSymbolTable(pTable, pTree->right);
            break;
    }
}

void handleWhile(SymbolTable *table, Tree *tree)
{
    pushLevelToSymbolTable(table);
    handleCondition(table, tree->left);
    buildSymbolTable(table, tree->left);
    buildSymbolTable(table, tree->right);
    popLevelFromSymbolTable(table);
}

void handleCondition(SymbolTable *table, Tree *tree)
{
    if (tree == NULL)
        return;

    if (tree->root->flag == ID || tree->root->flag == PARAM)
    {
        Node *nodeFound = searchSymbolInTable(table, tree->root->name, table->levels);
        if (nodeFound == NULL)
        {
            printf("buildSymbolTable: Variable %s not declared. \n", tree->root->name);
            exit(1);
        }
        tree->root = nodeFound;
    }

    handleCondition(table, tree->left);
    handleCondition(table, tree->right);
}

void handleThenOrElse(SymbolTable *table, Tree *tree)
{
    pushLevelToSymbolTable(table);
    buildSymbolTable(table, tree->left);
    popLevelFromSymbolTable(table);
    buildSymbolTable(table, tree->right);
}

void handleMethodCall(SymbolTable *table, Tree *tree)
{
    Node *leftChild = tree->left->root;
    Node *nodeFound = searchSymbolInTable(table, leftChild->name, table->levels);
    if (nodeFound == NULL)
    {
        printf("buildSymbolTable: Method %s not declared. \n", leftChild->name);
        exit(1);
    }
    tree->left->root = nodeFound;
    tree->left->root->parameters = nodeFound->parameters;
}

void handleMethodDecl(SymbolTable *table, Tree *tree)
{
    Node *leftChild = tree->left->root;
    if (tree->right->left->root->flag != EMPTY)
    {
        leftChild->parameters = tree->right->left;
    }
    checkForDuplicateParameters(tree, leftChild);
    insertSymbolInSymbolTable(table, leftChild, table->levels);
    buildSymbolTable(table, tree->right);
}


void handleVarDecl(SymbolTable *table, Tree *tree)
{
    Node *leftChild = tree->left->root;
    Tree *rightTree = tree->right;

    if (rightTree != NULL)
    {
        Node *rightChild = tree->right->root;
        if (rightChild->value != NULL)
        {
            leftChild->value = rightChild->value;
        }
        if (rightChild->flag == ID)
        {
            Node *nodeToInsert = searchSymbolInTable(table, rightChild->name, table->levels);
            if (nodeToInsert == NULL)
            {
                printf("buildSymbolTable: Variable %s not declared. \n", rightChild->name);
                exit(1);
            }
            leftChild->value = nodeToInsert->value;
            rightChild->type = nodeToInsert->type;
        }
    }
    insertSymbolInSymbolTable(table, leftChild, table->levels);
    buildSymbolTable(table, tree->right);
}

void handleAssign(SymbolTable *table, Tree *tree)
{
    Node *leftChild = tree->left->root;
    Node *rightChild = tree->right->root;

    Node *nodeFound = searchSymbolInTable(table, leftChild->name, table->levels);

    if (nodeFound == NULL)
    {
        printf("buildSymbolTable: Variable %s not declared. \n", rightChild->name);
        exit(1);
    }

    leftChild->type = nodeFound->type;

    if (rightChild->value != NULL)
    {
        leftChild->value = rightChild->value;
    }
    if (rightChild->flag == ID)
    {
        Node *nodeToInsert = searchSymbolInTable(table, rightChild->name, table->levels);

        if (nodeToInsert == NULL)
        {
            printf("buildSymbolTable: Variable %s not declared\n", rightChild->name);
            exit(1);
        }

        leftChild->value = nodeToInsert->value;
        rightChild->type = nodeToInsert->type;
    }

    buildSymbolTable(table, tree->right);
}

Node *searchAndValidateSymbol(SymbolTable *table, Node* nodeToSearch)
{
    Node *searchedNode = searchSymbolInTable(table, nodeToSearch->name, table->levels);
    if (searchedNode == NULL)
    {
        printf("Variable %s not declared. \n", nodeToSearch->name);
        exit(1);
    }
    return searchedNode;
}

void checkForDuplicateParameters(Tree *tree, const Node *leftChild) {
    Tree *paramList = tree->right->left;
    char *paramNames[MAX_PARAMS_ALLOWED];
    int paramCount = 0;

    while (paramList != NULL)
    {
        Node *paramNode = paramList->root;
        for (int i = 0; i < paramCount; i++)
        {
            if (strcmp(paramNames[i], paramNode->name) == 0)
            {
                printf("Parameter %s is duplicated in method %s\n", paramNode->name, leftChild->name);
                exit(1);
            }
        }
        paramNames[paramCount++] = paramNode->name;
        paramList = paramList->left;
    }
}
