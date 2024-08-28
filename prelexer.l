%{
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>

    #include "preparser.tab.h"
%}

%option noyywrap
%option yylineno

digit [0-9]

%%

"int" {
    printf("INT : %s\n", yytext);
    return INT;
}

"bool" {
    return BOOL;
}

"void" {
    printf("VOID : %s\n", yytext);
    return VOID;
}

"return" {
    printf("RETURN : %s\n", yytext);
    return RETURN;
}

"main" {
    printf("MAIN : %s\n", yytext);
    return MAIN;
}

";" {
    printf("COLON : %s\n", yytext);
    return COLON;
}

"=" {
    printf("ASSIGN : %s\n", yytext);
    return ASSIGN;
}

"+" {
    return PLUS;
}

"*" {
    return MULTIPLY;
}

"(" {
    printf("LBRACKET : %s\n", yytext);
    return LBRACKET;
}

")" {
    printf("RBRACKET : %s\n", yytext);
    return RBRACKET;
}

"{" {
    printf("LCURLY : %s\n", yytext);
    return LCURLY;
}

"}" {
    printf("RCURLY : %s\n", yytext);
    return RCURLY;
}

{digit}+    {
    printf("TNUMBER : %s\n", yytext);
    yylval.vint = atoi(yytext);
    return TNUMBER;
}

[a-zA-Z][a-zA-Z0-9]* {
    printf("ID : %s\n", yytext);
    yylval.vstring = strdup(yytext);
    return ID;
}

[.|\n]  ; /* ignore all the rest */

%%

void yyerror(){
    printf("Syntax ERROR at line: %d, near '%s'\n", yylineno, yytext);
    exit(1);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}
