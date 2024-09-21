%{
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdbool.h>

    #include "../parser/preparser.tab.h"
%}

%option noyywrap
%option yylineno

digit [0-9]
alpha [a-zA-Z]
alpha_num [a-zA-Z0-9_]

%%

"int" {
    return TINTEGER;
}

"bool" {
    return TBOOL;
}

"void" {
    return TVOID;
}

"return" {
    return TRETURN;
}

"true"|"false" {
   return TBOOL_LITERAL;
}

"&&" {
    return TAND;
}

"||" {
    return TOR;
}

"!" {
    return TNOT;
}

"==" {
    return TEQUALS;
}

";" {
    return TCOLON;
}

">" {
    return TGRATERTHAN;
}

"<" {
    return TLESSTHAN;
}

"=" {
    return TASSIGN;
}

"+" {
    return TPLUS;
}

"-" {
    return TMINUS;
}

"*" {
    return TMULTIPLY;
}

"/" {
    return TDIVISION;
}

"%" {
    return TMOD;
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

"if" {
    return TIF;
}

"then" {
    return TTHEN;
}

"else" {
    return TELSE;
}

"while" {
    return TWHILE;
}

"progam" {
    return TPROGRAM;
}

"extern" {
    return TEXTERN;
}

"," {
    return TCOMMA;
}

{digit}+ {
    return TDIGIT;
}

{digit}{digit}* {
    return TINTEGER_LITERAL;
}

{alpha}+ {
    return TALPHA;
}

{alpha}{alpha_num}* {
    return TID;
}

[.|\n]  ; /* ignore all the rest */

[//]    ; /* comments */

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