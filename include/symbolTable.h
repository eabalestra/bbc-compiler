#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

typedef struct SymbolList
{
    Node *symbol;
    int size;
    struct SymbolList *next;

} SymbolList;

typedef struct SymbolTable
{
    int levels;  // ammount of levels on the SymbolTable
    SymbolList *levelData;
    struct SymbolTable *next; 
    struct SymbolTable *previous;
} SymbolTable;

SymbolTable *createSymbolTable();
SymbolList *createSymbolList(Node *symbol);
void pushLevelToSymbolTable(SymbolTable *table);
void popLevelFromSymbolTable(SymbolTable *table);

Node *findSymbolNode(char *symbol, SymbolTable *table, int level);
Node* findNodeInLevel(SymbolList *symbolList, char *symbol);
void insertSymbolInSymbolTable(Node *symbol, SymbolTable *table, int level);

void printSymbolTable(SymbolTable *table);

#endif // SYMBOL_TABLE_H