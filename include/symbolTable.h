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

SymbolTable *createSymbolTable();   
void pushLevelToSymbolTable(SymbolTable *table); 
void popLevelFromSymbolTable(SymbolTable *table);

Node *findSymbolNode(SymbolTable *table, char *symbol, int level);
void insertSymbolInSymbolTable(SymbolTable *table, Node *symbol, int level);  
void printSymbolTable(SymbolTable *table);
SymbolTable *semanticCheck(SymbolTable *table, Tree *ast);
void buildSymbolTable(SymbolTable *pTable, Tree *pTree);

void handleVarDecl(SymbolTable *table, Tree *tree);
void handleMethodDecl(SymbolTable *table, Tree *tree);
void handleMethodEnd(SymbolTable *table, Tree *tree);
void handleMethodCall(SymbolTable *table, Tree *tree);
void handleThenOrElse(SymbolTable *table, Tree *tree);
void handleWhile(SymbolTable *table, Tree *tree);
void handleAssign(SymbolTable *table, Tree *tree);
void handleCondition(SymbolTable *table, Tree *tree);

#endif // SYMBOL_TABLE_H