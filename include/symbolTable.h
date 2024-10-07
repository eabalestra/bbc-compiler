#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolList.h"

typedef struct SymbolTable
{
    int levels;  // ammount of levels on the SymbolTable
    SymbolList *levelData;
    struct SymbolTable *next; 
    struct SymbolTable *previous;
} SymbolTable;

SymbolTable *createSymbolTable();   
void pushLevelToSymbolTable(SymbolTable *table); 
void popLevelFromSymbolTable(SymbolTable *table);

Node *findSymbolNode(char *symbol, SymbolTable *table, int level);
void insertSymbolInSymbolTable(Node *symbol, SymbolTable *table, int level);  

void printSymbolTable(SymbolTable *table);

SymbolTable *semanticCheck(Tree *ast);

#endif // SYMBOL_TABLE_H