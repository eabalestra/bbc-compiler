%{
#include <stdlib.h>
#include <stdio.h>
#include "binarytree/Tree.h"
%}

%union{
    struct Node *vnode;
    int vint;
    char *vstring;
}

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
%token VOID
%token BOOL
%token INT
%token<vint> TNUMBER
%token<vstring> ID

%type<vnode> decls
%type<vnode> decl
%type<vnode> stmts
%type<vnode> stmt
%type<vnode> expr
%type type
    
%left '+' PLUS
%left '*' MULTIPLY

%%

prog: type MAIN LBRACKET RBRACKET LCURLY decls stmts RCURLY

decls:  decl
        | decl decls
        ;

decl:   type ID COLON
        | type ID ASSIGN expr COLON
        ;

stmts:  stmt
        | stmt stmts
        ;

stmt:   ID ASSIGN expr COLON
        | RETURN expr COLON
        | RETURN COLON
        ;

expr:   expr PLUS expr
        | expr MULTIPLY expr
        | LBRACKET expr RBRACKET
        | TNUMBER
        | ID
        ;

type:   INT
        | BOOL
        | VOID
        ;

%%
