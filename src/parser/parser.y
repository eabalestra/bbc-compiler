%{
#include <stdlib.h>
#include <stdio.h>
%}

%token TINTEGER
%token TBOOL
%token TVOID

%token TRETURN
%token TIF;
%token TTHEN;
%token TELSE;
%token TWHILE;

%left TPLUS TDIVISION TMOV
%left TPLUS TMINUS
%left TLESSTHAN TGRATERTHAN
%left TEQUALS
%left TAND
%left TOR

%%

%%
