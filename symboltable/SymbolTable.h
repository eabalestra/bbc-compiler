#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../binarytree/Tree.h"

typedef struct SymbolTable
{
    Node *symbol;
    struct SymbolTable *next;
    int size;
} SymbolTable;

SymbolTable *createTable();
void insert(SymbolTable *table, Node *symbol);
Node *search(SymbolTable *table, char *symbol);
int exist(SymbolTable *table, char *symbol);
void printSymbolTable(SymbolTable *table);

SymbolTable* semanticCheck(Tree *tree);


#endif // SYMBOL_TABLE_H