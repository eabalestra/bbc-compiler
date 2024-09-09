#include "SymbolTable.h"
#include <errno.h>

SymbolTable* createTable()
{
    SymbolTable *newSymbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    newSymbolTable->symbol = NULL;
    newSymbolTable->next = NULL;
    newSymbolTable->size = 0;

    return newSymbolTable;
}

void insert(SymbolTable *table, Node *symbol)
{
    int valueExist = exist(table, symbol->value);
    if (valueExist == 1) {
        printf("Can't insert new element, it already exists.\n");
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

Node* search(SymbolTable *table, char *symbol)
{
    if (symbol == NULL)
    {
        return NULL;
    }

    SymbolTable *aux = table;
    while (aux->next != NULL)
    {
        if (aux->symbol->value == symbol)
        {
            return aux->symbol;
        }
        aux = aux->next;
    }
    return NULL;
}

int exist(SymbolTable *table, char *symbol)
{
    if (search(table, symbol) != NULL)
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
    printf("\n-- TABLE --\n");
    printf("SIZE = %d \n", table->size);
    SymbolTable *current_node = table;
   	while (current_node != NULL) {
        printf("\nNODE %s\n",nodeTypeToString(current_node->symbol->flag));
        printf("%s\n", current_node->symbol->name);
        printf("%p\n",current_node->symbol->value);
        current_node = current_node->next;
    }
}

SymbolTable* semanticCheck(Tree *tree)
{
    SymbolTable *table = createTable();
    declarationCheck(tree->left, table);
    return table;
}

void declarationCheck(Tree *tree, SymbolTable *table)
{
    if (tree == NULL || tree->root == NULL)
    {
        return;
    }
    if (tree->left != NULL && tree->left->root->flag == ID)
    {
        Tree *hi = tree->left;
        Node *newNode = createNode(VAR, hi->root->type, NULL, hi->root->value);
        if (tree->right != NULL && tree->right->root != NULL)
        {
            Tree *hd = tree->right;
            if (newNode->type == hd->root->type)
            {
                newNode->value = hd->root->value;
            } else 
            {
                printf("Incorrect types \n");
            }
        }
        insert(table, newNode);
    }
    declarationCheck(tree->left, table);
    declarationCheck(tree->right, table);
}
