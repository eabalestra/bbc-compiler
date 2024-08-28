%{
#include <stdlib.h>
#include <stdio.h>
#include "binarytree/Tree.h"
%}

%union{
    enum Type *vtype;
    struct Node *vnode;
    struct Tree *vast;
    int vint;
    char *vstring;
}

%token TRETURN
%token TCOLON
%token TPLUS
%token TMULTIPLY
%token TLBRACKET
%token TRBRACKET
%token TLCURLY
%token TRCURLY
%token TMAIN
%token<vast> TASSIGN
%token<vtype> TVOID
%token<vtype> TBOOLEAN
%token<vtype> TINTEGER
%token<vint> TNUMBER
%token<vstring> TID

%type<vast> decls
%type<vast> decl
%type<vast> stmts
%type<vast> stmt
%type<vast> expr
%type<vast> type
    
%left '+' TPLUS
%left '*' TMULTIPLY

%%

prog: type TMAIN TLBRACKET TRBRACKET TLCURLY decls stmts TRCURLY

decls:  decl
        | decl decls
        ;

decl:   type TID TCOLON   {}
        | type TID TASSIGN expr TCOLON {}
        ;

stmts:  stmt
        | stmt stmts
        ;

stmt:   TID TASSIGN expr TCOLON
        | TRETURN expr TCOLON
        | TRETURN TCOLON
        ;

expr:   expr TPLUS expr
        | expr TMULTIPLY expr    {}
        | TLBRACKET expr TRBRACKET    {}
        | TNUMBER   {}
        | TID {}
        ;

type:   TINTEGER {}
        | TBOOLEAN   {}
        | TVOID  {}
        ;

%%
