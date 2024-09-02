#include "SymbolTable.h"
#include <errno.h>

SymbolTable *createTable()
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

Node *search(SymbolTable *table, char *symbol)
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
    printf("size = %d \n", table->size);
    SymbolTable *current_node = table;
   	while (current_node != NULL) {
        printf("%s\n", nodeTypeToString(current_node->symbol->flag));
        current_node = current_node->next;
    }
}