#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolList.h"

typedef struct SymbolTable
{
    SymbolList* currentLevel;
    struct SymbolTable* parent;
} SymbolTable;

SymbolTable* createSymbolTable();
SymbolTable* pushLevelToSymbolTable(SymbolTable* symbolTable);
SymbolTable* popLevelFromSymbolTable(SymbolTable* symbolTable);

void insertSymbolInSymbolTable(SymbolTable* symbolTable, Node* newSymbolNode);
Node* lookupInSymbolTable(SymbolTable* symbolTable, char* name);

void printSymbolTable(SymbolTable *symbolTable);

SymbolTable *semanticCheck(SymbolTable *symbolTable, Tree *tree);
void buildSymbolTable(SymbolTable *symbolTable, Tree *tree);

#endif // SYMBOL_TABLE_H