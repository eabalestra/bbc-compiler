%{
#include <stdlib.h>
#include <stdio.h>
#include "binarytree/binarytree.h"
%}

%union{
    struct Node *vnode;
    struct int vint;
    struct *char vstring;
}

%token TYPE
%token RETURN
%token COLON
%token ASSIGN
%token PLUS
%token MULTIPLY
%token LBRACKET
%token RBRACKET
%token LCURLY
%token RCURLY
%token MAIN
%token<vint> INT
%token<vstring> ID

%type body
%type sentence
%type asgmt
%type stmt
%type<vnode> expr
%type<vint> value
    
%left '+' PLUS
%left '*' MULTIPLY

%%

prog: TYPE MAIN LBRACKET RBRACKET LCURLY body RCURLY    { printf("No hay errores \n"); }
    ;

body: sentence body
    | %empty
    ;

sentence:   stmt COLON
            | asgmt COLON
            ;

asgmt:  ID ASSIGN expr 
        | RETURN expr
        | RETURN 
        ;

stmt:   TYPE ID
        |   TYPE ID ASSIGN expr
        ;

expr:   value   { $$ = $1; }
        | expr PLUS expr
        | expr MULTIPLY expr
        | LBRACKET expr RBRACKET
        | ID { $$ = createNodeWithSymbol($1); }
        ;

value:  INT { $$ = $1; }
        ;

%%
