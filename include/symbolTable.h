#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolList.h"
#include "typeChecker.h"

typedef struct SymbolTable
{
    int levels;  // ammount of levels on the SymbolTable
    SymbolList *levelData;
    struct SymbolTable *next; 
    struct SymbolTable *previous;
} SymbolTable;

static const int MAX_PARAMS_ALLOWED = 100;

SymbolTable *createSymbolTable();
void pushLevelToSymbolTable(SymbolTable *table); 
void popLevelFromSymbolTable(SymbolTable *table);

Node *searchSymbolInTable(SymbolTable *table, char *symbol, int level);
void insertSymbolInSymbolTable(SymbolTable *table, Node *symbol, int level);  
void printSymbolTable(SymbolTable *table);

Node *searchAndValidateSymbol(SymbolTable *table, Node* nodeToSearch);

Tree *semanticCheck(SymbolTable *table, Tree *ast);
void buildSymbolTable(SymbolTable *pTable, Tree *pTree);

#endif // SYMBOL_TABLE_H