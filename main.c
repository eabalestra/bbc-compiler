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
    ++argv,--argc;
    if (argc > 0)
        yyin = fopen(argv[0],"r");
    else
        yyin = stdin;

    yyparse();
}