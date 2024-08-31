%{
#include <stdlib.h>
#include <stdio.h>
#include "binarytree/Tree.h"

Tree *ast;
%}

%union{
    enum Type *vtype;
    struct Node *vnode;
    struct Tree *vast;
    int vint;
    char *vstring;
}

%token TRETURN
%token<vast> TCOLON
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
%type<vtype> type
%type<vast> prog
    
%left '+' TPLUS
%left '*' TMULTIPLY

%%

prog: type TMAIN TLBRACKET TRBRACKET TLCURLY decls stmts TRCURLY { printf("PARSER OK\n"); Node *newNode = createNode(PROG, NONTYPE, NULL, NULL); $$ = createTree(newNode, $6, $7); ast = $$;  printTree($$);}

decls:  decl { Node *newNode = createNode(COLON, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, NULL); }
        | decl decls { Node *newNode = createNode(DECLS, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $2);  }
        ;

decl:   type TID TCOLON { Node *newNode = createNode(ID, *$1, $2, NULL); $$ = createTree(createNode(DECL, NONTYPE, NULL, NULL), createTree(newNode, NULL, NULL), $3); }
        | type TID TASSIGN expr TCOLON { Node *newNode = createNode(ID, *$1, $2, NULL); $$ = createTree(createNode(DECL, NONTYPE, NULL, NULL), createTree(newNode, NULL, NULL), $4); }
        ;

stmts:  stmt { Node *newNode = createNode(COLON, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, NULL);}
        | stmt stmts { Node *newNode = createNode(STMTS, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $2);}
        ;
        
stmt:   TID TASSIGN expr TCOLON { Node *newNode = createNode(ASSIGN, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $3); }
        | TRETURN expr TCOLON { Node *newNode = createNode(RETURN, NONTYPE, NULL, NULL); $$ = createTree(newNode, $2, NULL); } 
        | TRETURN TCOLON { Node *newNode = createNode(RETURN, NONTYPE, NULL, NULL); $$ = createTree(newNode, NULL, NULL); }
        ;

expr:   expr TPLUS expr {Node *newNode = createNode(PLUS, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $3); }
        | expr TMULTIPLY expr    { Node *newNode = createNode(MULTIPLY, NONTYPE, NULL, NULL); $$ = createTree(newNode, $1, $3); }
        | TLBRACKET expr TRBRACKET { Node *newNode = createNode(EXPR, NONTYPE, $2, NULL); $$ = createTree(newNode, NULL, NULL); }
        | TNUMBER   { Node *newNode = createNode(NUMBER, INTEGER, (void *) $1, NULL); $$ = createTree(newNode, NULL, NULL); }
        | TID { Node *newNode = createNode(ID, NONTYPE, NULL, $1); $$ = createTree(newNode, NULL, NULL); }
        ;

type:   TINTEGER { $$ = INTEGER; }
        | TBOOLEAN   { $$ = BOOLEAN; }
        | TVOID  { $$ = VOID; }
        ;

%%