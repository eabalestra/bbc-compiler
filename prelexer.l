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
    return TYPE;
}

"bool" {
    return TYPE;
}

"void" {
    return TYPE;
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
    return COLON;
}

"=" {
    return ASSIGN;
}

"+" {
    return PLUS;
}

"*" {
    return MULTIPLY;
}

"(" {
    return LBRACKET;
}

")" {
    return RBRACKET;
}

"{" {
    return LCURLY;
}

"}" {
    return RCURLY;
}

{digit}+    {
    yylval = atoi(yytext);
    return INT;
}

[a-zA-Z][a-zA-Z0-9]* {
    yylval = strdup(yytext);
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
