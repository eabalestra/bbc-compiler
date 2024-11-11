#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "include/tree.h"
#include "include/symbolTable.h"
#include "include/quadrupleLinkedList.h"
#include "include/assemblyCodeGenerator.h"
#include "include/threeAddressGenerator.h"
#include <bits/getopt_core.h>

extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;
extern char *yytext;
extern int yyparse(void);

Tree *ast = NULL;
SymbolTable *symbolTable = NULL;
QuadrupleLinkedList *qll = NULL;

void yyerror()
{
    printf("Syntax ERROR at line: %d, near '%s'\n", yylineno, yytext);
    exit(1);
}

int hasCtdsFileExtension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return 0;

    return strcmp(dot, ".ctds") == 0;
}

int main(int argc, char *argv[])
{
    if (optind >= argc)
    {
        fprintf(stderr, "Expected file name after options\n");
        exit(EXIT_FAILURE);
    }

    char *inputFile = argv[optind];

    if (!hasCtdsFileExtension(inputFile))
    {
        printf("Invalid file extension. Please provide a file with a .ctds extension.\n");
        exit(1);
    }

    yyin = fopen(inputFile, "r");
    if (!yyin)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Parse the input file and build the AST
    yyparse();

    // Perform semantic checks
    symbolTable = createSymbolTable();
    symbolTable = semanticCheck(symbolTable, ast);

    // Generate three address code
    generateThreeAddressCode(ast);

    // Generate assembly code
    qll = getQuadrupleList();
    generateAssemblyCode(qll);

    // Print debugging information if requested
    if (1)
    {
        printTree(ast);
        printQuadrupleLinkedList(qll);
    }

    fclose(yyin);
    return 0;
}