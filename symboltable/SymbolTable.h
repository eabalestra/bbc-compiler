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

SymbolTable *createSymbolTable();
void insertSymbol(SymbolTable *table, Node *symbol);
Node *findSymbol(SymbolTable *table, char *symbol);
int symbolExists(SymbolTable *table, char *symbol);
void printSymbolTable(SymbolTable *table);

SymbolTable* performSemanticCheck(Tree *tree);
void checkDeclarations(Tree *tree, SymbolTable *table);
void interpreter(Tree *tree, SymbolTable *table);
void *interpret(Tree *tree, SymbolTable *table);

#endif // SYMBOL_TABLE_H