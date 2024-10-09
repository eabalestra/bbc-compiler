%{
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdbool.h>

    #include "../include/type.h"
    #include "../build/parser.tab.h"
%}

%option noyywrap
%option yylineno

digit [0-9]
alpha [a-zA-Z]
alpha_num [a-zA-Z0-9_]

%%

"integer" {
    yylval.vtype = INTEGER;
    return TINTEGER;
}

"bool" {
    yylval.vtype = BOOLEAN;
    return TBOOL;
}

"void" {
    return TVOID;
}

"return" {
    yylval.line_number = yylineno;
    return TRETURN;
}

"true"|"false" {
    if (strcmp(yytext, "true") == 0) {
        yylval.vbool = true;
    } else {
        yylval.vbool = false;
    }
    yylval.line_number = yylineno;
    return TBOOL_LITERAL;
}

"&&" {
    yylval.line_number = yylineno;
    return TAND;
}

"||" {
    yylval.line_number = yylineno;
    return TOR;
}

"!" {
    yylval.line_number = yylineno;
    return TNOT;
}

"==" {
    yylval.line_number = yylineno;
    return TEQUALS;
}

";" {
    return TCOLON;
}

">" {
    yylval.line_number = yylineno;
    return TGRATERTHAN;
}

"<" {
    yylval.line_number = yylineno;
    return TLESSTHAN;
}

"=" {
    yylval.line_number = yylineno;
    return TASSIGN;
}

"+" {
    yylval.line_number = yylineno;
    return TPLUS;
}

"-" {
    yylval.line_number = yylineno;
    return TMINUS;
}

"*" {
    yylval.line_number = yylineno;
    return TMULTIPLY;
}

"/" {
    yylval.line_number = yylineno;
    return TDIVISION;
}

"%" {
    yylval.line_number = yylineno;
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

"Program" {
    return TPROGRAM;
}

"extern" {
    return TEXTERN;
}

"," {
    return TCOMMA;
}

{digit}{digit}* {
    yylval.vint = atoi(yytext);
    yylval.line_number = yylineno;
    return TINTEGER_LITERAL;
}

{alpha}{alpha_num}* {
    yylval.vstring = strdup(yytext);
    yylval.line_number = yylineno;
    return TID;
}

[.|\n]  ; /* ignore all the rest */

"//"(alpha_num|[+*;()-{},!=|<>%&]|" ")* ; /* single-line comments */

"/*"([^*]|\*+[^*/])*\*+"/" ; /* multi line comments */


%%