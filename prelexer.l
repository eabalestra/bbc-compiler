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
    return TINTEGER;
}

"bool" {
    return TBOOLEAN;
}

"void" {
    printf("VOID : %s\n", yytext);
    return TVOID;
}

"return" {
    printf("RETURN : %s\n", yytext);
    return TRETURN;
}

"main" {
    printf("MAIN : %s\n", yytext);
    return TMAIN;
}

";" {
    printf("COLON : %s\n", yytext);
    return TCOLON;
}

"=" {
    printf("ASSIGN : %s\n", yytext);
    return TASSIGN;
}

"+" {
    return TPLUS;
}

"*" {
    return TMULTIPLY;
}

"(" {
    printf("LBRACKET : %s\n", yytext);
    return TLBRACKET;
}

")" {
    printf("RBRACKET : %s\n", yytext);
    return TRBRACKET;
}

"{" {
    printf("LCURLY : %s\n", yytext);
    return TLCURLY;
}

"}" {
    printf("RCURLY : %s\n", yytext);
    return TRCURLY;
}

{digit}+    {
    printf("TNUMBER : %s\n", yytext);
    yylval.vint = atoi(yytext);
    return TNUMBER;
}

[a-zA-Z][a-zA-Z0-9]* {
    printf("ID : %s\n", yytext);
    yylval.vstring = strdup(yytext);
    return TID;
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
