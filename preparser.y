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
%token<vast> TPLUS
%token<vast> TMULTIPLY
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

prog: type TMAIN TLBRACKET TRBRACKET TLCURLY decls stmts TRCURLY { printf("PARSER OK\n"); }

decls:  decl {}
        | decl decls {}
        ;

decl:   type TID TCOLON   {}
        | type TID TASSIGN expr TCOLON {}
        ;

stmts:  stmt {}
        | stmt stmts {}
        ;

stmt:   TID TASSIGN expr TCOLON
        | TRETURN expr TCOLON
        | TRETURN TCOLON
        ;

expr:   expr TPLUS expr {Node *newNode = createNode(PLUS, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $3); printTree($$);}
        | expr TMULTIPLY expr    { Node *newNode = createNode(MULTIPLY, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $3); printTree($$); }
        | TLBRACKET expr TRBRACKET { Node *newNode = createNode(EXPR, NONTYPE, $2, NULL); $$ = createTree(newNode, NULL, NULL); }
        | TNUMBER   { Node *newNode = createNode(NUMBER, INTEGER, (void *) $1, NULL); $$ = createTree(newNode, NULL, NULL); printTree($$); }
        | TID { Node *newNode = createNode(ID, NONTYPE, NULL, $1); $$ = createTree(newNode, NULL, NULL); printTree($$); }
        ;

type:   TINTEGER { $$ = INTEGER; }
        | TBOOLEAN   { $$ = BOOLEAN; }
        | TVOID  { $$ = VOID; }
        ;

%%