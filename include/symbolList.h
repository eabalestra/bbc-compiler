#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

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

SymbolList *createSymbolList(Node *symbol);

void insertSymbolInSymbolNodeList(SymbolList *symbolList, Node* newSymbolNode);

Node* findNodeInLevel(SymbolList *symbolList, char *symbol);

void freeSymbolList(SymbolList *symbolList);

#endif // SYMBOL_LIST_H