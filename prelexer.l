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
    printf("TYPE : %s\n", yytext);
    return TYPE;
}

"bool" {
    printf("TYPE : %s\n", yytext);
    return TYPE;
}

"void" {
    printf("TYPE : %s\n", yytext);
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
    return SEMICOLON;
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
    return OPEN_BRACKET;
}

")" {
    return CLOSE_BRACKET;
}

"{" {
    return OPEN_CURLY_BRACKET;
}

"}" {
    return CLOSE_CURLY_BRACKET;
}

{digit}+    {
    printf("INT : %d\n",atoi(yytext));
    return INT;
}

[a-zA-Z][a-zA-Z0-9]* {
    printf("ID : %s\n", yytext);
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
