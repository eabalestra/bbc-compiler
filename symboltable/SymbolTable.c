#include "SymbolTable.h"

void handleDeclaration(Tree *tree, SymbolTable *table);
void handleStatements(Tree *tree, SymbolTable *table);

SymbolTable* createSymbolTable()
{
    SymbolTable *newSymbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    newSymbolTable->symbol = NULL;
    newSymbolTable->next = NULL;
    newSymbolTable->size = 0;

    return newSymbolTable;
}

void insertSymbol(SymbolTable *table, Node *symbol)
{
    int valueExist = symbolExists(table, symbol->name);
    if (valueExist == 1) {
        printf("Can't insertSymbol new element, it already exists.\n");
        return;
    }
    
    SymbolTable *newSymbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    newSymbolTable->symbol = symbol;
    newSymbolTable->next = NULL;

    if (table->symbol == NULL) {
        table->symbol = symbol;
    } else {
        SymbolTable *aux = table;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = newSymbolTable;
    }
    table->size++;
}

Node* findSymbol(SymbolTable *table, char *symbol)
{
    if (symbol == NULL || table->size == 0)
    {
        return NULL;
    }
    SymbolTable *aux = table;
    while (aux != NULL) {
        if (strcmp(aux->symbol->name, symbol) == 0) {
                return aux->symbol;
        }
        aux = aux->next;
    }
    return NULL;
}

int symbolExists(SymbolTable *table, char *symbol)
{
    if (findSymbol(table, symbol) != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void printSymbolTable(SymbolTable *table)
{
    printf("\n-- SYMBOL TABLE --\n");
    printf("SIZE = %d \n", table->size);
    SymbolTable *current_node = table;
   	while (current_node != NULL) {
        printf("\nNODE %s\n",nodeTypeToString(current_node->symbol->flag));
        printf("type: %u\n",current_node->symbol->type);
        printf("name: %s\n", current_node->symbol->name);
        printf("value: %d\n",current_node->symbol->value);
        current_node = current_node->next;
    }
}

SymbolTable* performSemanticCheck(Tree *tree)
{
    SymbolTable *table = createSymbolTable();
    Node *newNode = createNode(PROG, tree->root->type, NULL, "main");
    insertSymbol(table, newNode);
    
    checkDeclarations(tree->left, table);
    checkStatements(tree->right, table);
    return table;
}

void checkDeclarations(Tree *tree, SymbolTable *table)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }
    if (tree->root->flag == DECL)
    {
        handleDeclaration(tree, table);
    }
    checkDeclarations(tree->left, table);
    checkDeclarations(tree->right, table);
}

void handleDeclaration(Tree *tree, SymbolTable *table)
{
    Tree *hi = tree->left;
    Node *newNode = createNode(VAR, hi->root->type, NULL, hi->root->value);
    if (tree->right != NULL && tree->right->root != NULL)
    {
        Tree *hd = tree->right;
        if (hd->root->flag == NUMBER || hd->root->flag == BOOL) { // TODO: PREGUNTAR
            if (newNode->type == hd->root->type) {
                newNode->value = hd->root->value;
            } else {
                printf("Incorrect types \n");
                return; // en vez de un return un exit para que se termine la ejecucion
            }
        } else {
            Node *aux = findSymbol(table, hd->root->value);
            if (aux == NULL) {
                printf("Id not declared \n");
                return;  // en vez de un return un exit para que se termine la ejecucion
            }
            if (newNode->type == aux->type) {
                newNode->value = aux->value;
            }
        }
    }
    insertSymbol(table, newNode);
}

void checkStatements(Tree *tree, SymbolTable *table) 
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }
    if (tree->root->flag == ASSIGN || tree->root->flag == RETURN || tree->root->flag == COLON)
    {
        handleStatements(tree, table);
    }
    checkStatements(tree->left, table);
    checkStatements(tree->right, table);
}

void handleStatements(Tree *tree, SymbolTable *table)
{
    if (tree->root->flag == ASSIGN)
    {
        if (!isVarDeclared(table, tree->left->root->value)) {
            return;
        }
        Node *hiNode = findSymbol(table, tree->left->root->value);
        int idType = hiNode->type;

        if (isTreeAnOperator(tree->right)) {
            printf("LLEGUE por que es un operator \n");
            Tree *operator = tree->right;
            Node *hiOperatorNode = findSymbol(table, operator->left->root->value);
            Node *hdOperatorNode = findSymbol(table, operator->right->root->value);

            if (hiOperatorNode->type != idType || hdOperatorNode->type != idType) { // ver que esten declaradas
                printf("Incorrect types \n");
                return;
            }
        } else {
            printf("LLEGUE por que NO es un operator \n");
            Node *hiNode = findSymbol(table, tree->right->root->value);
            if (hiNode->type != idType) {
                printf("Incorrect types \n"); // en vez de un return un exit para que se termine la ejecucion
                return;
            }
        }
    }
    if (tree->root->flag == RETURN)
    {
        Node *mainNode = findSymbol(table, "main");
        int mainType = mainNode->type;
        if (isTreeAnOperator(tree->left)) {
            printf("RETURN LLEGUE por que es un operator \n");

            Tree *operator = tree->left;
            Node *hiOperatorNode = findSymbol(table, operator->left->root->value);
            Node *hdOperatorNode = findSymbol(table, operator->right->root->value);

            if (hiOperatorNode->type != mainType || hdOperatorNode->type != mainType) { // ver que esten declaradas
                printf("Incorrect main and return types \n");
                return;
            }
        } else {
            printf("RETURN LLEGUE por que NO es un operator \n");
            Node *hiNode = findSymbol(table, tree->left->root->value);
            if (hiNode->type != mainType) {
                printf("Incorrect main and return types \n");
                return;
            }
        }
    }
}

int checkOperatorTypes(Tree *operator, SymbolTable *table, int expectedType)
{
    Node *leftOperand = findSymbol(table, operator->left->root->value);
    Node *rightOperand = findSymbol(table, operator->right->root->value);

    if (leftOperand->type != expectedType || rightOperand->type != expectedType) {
        return 0;
    }
}

int isVarDeclared(SymbolTable *table, char* symbol) {
    Node *foundNode = findSymbol(table, symbol);
    if (foundNode == NULL)
    {
        printf("Error: var %s is not declared", symbol);
        return 0;
    }
}

int isTreeAnOperator(Tree *tree) {
    switch (tree->root->flag) {
        case PLUS:
        case MULTIPLY:
        case AND:
        case OR:
            return 1;
        default:
            return 0;
    }
}