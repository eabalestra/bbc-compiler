%{
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdbool.h>

    #include "preparser.tab.h"
%}

%option noyywrap
%option yylineno

digit [0-9]

%%

"int" {
    return TINTEGER;
}

"boolean" {
    return TBOOLEAN;
}

"void" {
    return TVOID;
}

"return" {
    return TRETURN;
}

"main" {
    return TMAIN;
}

"true" {
   yylval.vbool = true;
   return TTRUE;
}

"false" {
   yylval.vbool = false;
   return TFALSE;
}

"and" {
    return TAND;
}

"or" {
    return TOR;
}

";" {
    return TCOLON;
}

"=" {
    return TASSIGN;
}

"+" {
    return TPLUS;
}

"*" {
    return TMULTIPLY;
}

"(" {
    return TLBRACKET;
}

")" {
    return TRBRACKET;
}

"{" {
    return TLCURLY;
}

"}" {
    return TRCURLY;
}

{digit}+    {
    yylval.vint = atoi(yytext);
    return TNUMBER;
}

[a-zA-Z][a-zA-Z0-9]* {
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
