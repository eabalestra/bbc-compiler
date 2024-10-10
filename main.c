#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/symbolTable.h"
#include "include/tree.h"

extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;
extern char *yytext;
extern int yyparse(void);

void yyerror(){
    printf("Syntax ERROR at line: %d, near '%s'\n", yylineno, yytext);
    exit(1);
}

//int main() {
int main(int argc,char *argv[]){
    /*SymbolTable *symbolTable = createSymbolTable();

    Node *node1 = createNode(ID, INTEGER, (void *) 2, "a", 1);
    Node *node2 = createNode(ID, INTEGER, (void *) 2, "b", 1);

    insertSymbolInSymbolTable(symbolTable, node1);
    insertSymbolInSymbolTable(symbolTable, node2);

    symbolTable = pushLevelToSymbolTable(symbolTable);
    insertSymbolInSymbolTable(symbolTable, createNode(ID, INTEGER, (void *) 2, "c", 1));
    printSymbolTable(symbolTable);

    printf("--------------\n");
    symbolTable = pushLevelToSymbolTable(symbolTable);
    insertSymbolInSymbolTable(symbolTable, createNode(ID, INTEGER, (void *) 2, "d", 1));
    printSymbolTable(symbolTable);

    printf("--------------\n");
    symbolTable = popLevelFromSymbolTable(symbolTable);
    printSymbolTable(symbolTable);

    printf("--------------\n");
    Node *foundNode = lookupInSymbolTable(symbolTable, "a");
    printf("foundNode is null? %d\n", foundNode == NULL);
    printf("founded node: %s\n", foundNode->name);*/

    ++argv,--argc;
    if (argc > 0)
        yyin = fopen(argv[0],"r");
    else
        yyin = stdin;

    yyparse();
}