%{
#include <stdlib.h>
#include <stdio.h>
%}

%token TINTEGER
%token TBOOL
%token TVOID
%token TBOOL_LITERAL
%token TINTEGER_LITERAL
%token TID
%token TDIGIT
%token TALPHA

%token TRETURN
%token TIF
%token TTHEN
%token TELSE
%token TWHILE

%token TAND
%token TOR
%token TNOT
%token TEQUALS
%token TGRATERTHAN
%token TLESSTHAN
%token TASSIGN
%token TPLUS
%token TMINUS
%token TMULTIPLY
%token TDIVISION
%token TMOD

%token TCOLON
%token TLBRACKET
%token TRBRACKET
%token TLCURLY
%token TRCURLY
%token TPROGRAM
%token TEXTERN
%token TCOMMA

%left TOR
%left TAND
%nonassoc TEQUALS TLESSTHAN TGRATERTHAN
%left TPLUS TMINUS
%left TMULTIPLY TDIVISION TMOD
%left TUNARY

%%

program:    TPROGRAM TLCURLY var_decls method_decls TRCURLY { printf("PARSER OK\n"); }
            | TPROGRAM TLCURLY method_decls TRCURLY { printf("PARSER OK\n"); }
            ;

var_decls:  var_decls var_decl
            | var_decl
            ;

var_decl:   type TID TASSIGN expr TCOLON
            | type TID TCOLON
            ;

method_decls:   method_decls method_decl
                | method_decl
                ;

method_decl:    type TID TLBRACKET params TRBRACKET method_end
                | TVOID TID TLBRACKET params TRBRACKET method_end
                ;

params:     params_list
            | %empty
            ;

params_list:    type TID
                | type TID TCOMMA params_list
                ;

method_end:     block
                | TEXTERN TCOLON
                ;

block:  TLCURLY var_decls statements TRCURLY
        | TLCURLY statements TRCURLY
        | TLCURLY var_decls TRCURLY
        | TLCURLY TRCURLY
        ;

type:   TINTEGER  
        | TBOOL
        ;

statements:     statements statement
                | statement
                ;

statement:  TID TASSIGN expr TCOLON
            | method_call TCOLON
            | if_block
            | while_block
            | return_block
            | TCOLON
            | block
            ;

return_block:   TRETURN expr TCOLON
                | TRETURN TCOLON
                ;

method_call:    TID TLBRACKET expr_list TRBRACKET
                ;

expr:   TID
        | method_call
        | literal
        | expr TAND expr
        | expr TOR expr
        | expr TPLUS expr
        | expr TMINUS expr
        | expr TMULTIPLY expr
        | expr TDIVISION expr
        | expr TMOD expr
        | expr TLESSTHAN expr
        | expr TGRATERTHAN expr
        | expr TEQUALS expr
        | TMINUS expr %prec TUNARY
        | TNOT expr %prec TUNARY
        | TLBRACKET expr TRBRACKET
        ;

expr_list:  expr TCOMMA expr_list
            | expr
            | %empty
            ;

if_block:   TIF TLBRACKET expr TRBRACKET TTHEN block else_block
            ;

else_block:     TELSE block
                | %empty
                ;

while_block:    TWHILE TLBRACKET expr TRBRACKET block
                ;

literal:    integer_literal
            | bool_literal
            ;

integer_literal : TINTEGER_LITERAL
                ;

bool_literal : TBOOL_LITERAL
             ;

%%