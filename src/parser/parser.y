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

program : TPROGRAM TLCURLY var_decls method_decls TRCURLY

var_decls:  var_decl var_decls
            | var_decl
            ;

var_decl : type TID TASSIGN expr TCOLON
         ;

method_decls:   method_decl method_decls
                | method_decl
                ;

method_decl:    method_type TID TLBRACKET params TRBRACKET TRBRACKET method_end
                ;

method_type:    TVOID
                | type
                ;

params:     params_list
            | %empty
            ;

method_end:     block
                | TEXTERN TCOLON
                ;

params_list:    type TID TCOMMA params_list
                | type TID
                ;

block:  TLCURLY var_decls statements TRCURLY
        ;

statements:     statement statements
                | statement
                ;

statement:  TID TASSIGN expr TCOLON
            | method_call TCOLON
            | if_block
            | while_block
            | return expr TCOLON
            | //punto y coma
            | block
            ;

expr    : TID
        | method_call
        | literal
        | expr bin_op expr
        | TMINUS expr
        | TNOT expr
        | TLBRACKET expr TRBRACKET
        ;

bin_op: arith_op
        | rel_op
        | cond_op
        ;

arith_op:   TPLUS
            | TMINUS
            | TMULTIPLY
            |
            ;

rel_op:     TLESSTHAN
            | TGRATERTHAN
            | TEQUALS
            ;

cond_op:    TAND
            | TOR
            ;

literal:    integer_literal
            | bool_literal
            ;

integer_literal : TINTEGER_LITERAL
                ;

bool_literal : TBOOL_LITERAL
             ;

%%
