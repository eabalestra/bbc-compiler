%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>

    #include "../include/tree.h"
    #include "../include/type.h"
    
    Tree *ast;
%}

%union{
    enum Type vtype;
    struct Tree *vast;
    char *vstring;
    int vint;
    int line_number;
    int vbool;
}

%token<vtype> TINTEGER
%token<vtype> TBOOL
%token<vtype> TVOID
%token<vbool> TBOOL_LITERAL
%token<vint> TINTEGER_LITERAL
%token<vstring> TID
%token<vint> TDIGIT
%token<vstring> TALPHA

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

%type<vtype> type

%type<vast> program
%type<vast> var_decls
%type<vast> var_decl
%type<vast> method_decls
%type<vast> method_decl
%type<vast> params
%type<vast> params_list
%type<vast> method_end
%type<vast> block
%type<vast> statements
%type<vast> statement
%type<vast> return_block
%type<vast> method_call
%type<vast> expr
%type<vast> expr_list
%type<vast> if_block
%type<vast> else_block
%type<vast> while_block
%type<vast> literal
%type<vast> integer_literal
%type<vast> bool_literal

%left TOR
%left TAND
%nonassoc TEQUALS TLESSTHAN TGRATERTHAN
%left TPLUS TMINUS
%left TMULTIPLY TDIVISION TMOD
%left TUNARY

%%

program:    TPROGRAM TLCURLY var_decls method_decls TRCURLY { 
                Node *newNode = createNonTerminalNode(PROG);        
                ast = createTree(newNode, $3, $4);
                printTree(ast);
                printf("PARSER OK\n"); 
            }
            | TPROGRAM TLCURLY method_decls TRCURLY {
                Node *newNode = createNonTerminalNode(PROG);        
                ast = createTree(newNode, $3, NULL);
                printTree(ast);
                printf("PARSER OK\n"); 
            }
            ;

var_decls:  var_decls var_decl {
                Node *newNode = createNonTerminalNode(DECLS);
                $$ = createTree(newNode, $2, $1);
            }
            | var_decl {
                Node *newNode = createNonTerminalNode(COLON);
                $$ = createTree(newNode, $1, NULL);
            }
            ;

var_decl:   type TID TASSIGN expr TCOLON {
                Node *newNode = createNode(ID, $1, NULL, $2, yylval.line_number);
                Tree *idTree = createTree(newNode, NULL, NULL);
                $$ = createTree(createNonTerminalNode(VARDECL), idTree, $4);
            }
            | type TID TCOLON {
                Node *newNode = createNode(ID, $1, NULL, $2, yylval.line_number);
                Tree *idTree = createTree(newNode, NULL, NULL);
                $$ = createTree(createNonTerminalNode(VARDECL), idTree, NULL);
            }
            ;

method_decls:   method_decls method_decl {
                                Node *newNode = createNonTerminalNode(METHODDECLS);
                                $$ = createTree(newNode, $2, $1);
                }
                | method_decl { $$ = $1; }
                ;

method_decl:    type TID TLBRACKET params TRBRACKET method_end {
                    Node *newNode = createNonTerminalNode(METHODDECL);
                    Node *idNode = createNode(ID, $1, NULL, $2, yylval.line_number);  
                    Tree *idTree = createTree(idNode, NULL, NULL); 
                    Node *methodEndNode = createNonTerminalNode(METHODEND);
                    $$ = createTree(newNode, idTree, createTree(methodEndNode, $4, $6));
                }
                | TVOID TID TLBRACKET params TRBRACKET method_end {
                    Node *newNode = createNonTerminalNode(METHODDECL);
                    Node *idNode = createNode(ID, VOID, NULL, $2, yylval.line_number);
                    Tree *hi = createTree(idNode, NULL, NULL);
                    Tree *hd = createTree(createNonTerminalNode(METHODEND), $4, $6);
                    $$ = createTree(newNode, hi, hd);
                }
                ;

params:     params_list { $$ = $1; }
            | %empty { $$ = NULL; }
            ;

params_list:    type TID  {
                    Node *idNode = createNode(ID, $1, NULL, $2, yylval.line_number);
                    $$ = createTree(idNode, NULL, NULL);
                }
                | type TID TCOMMA params_list {
                    Node *newNode = createNonTerminalNode(PARAMSLIST);
                    Node *idNode = createNode(ID, $1, NULL, $2, yylval.line_number);
                    $$ = createTree(newNode, createTree(idNode, NULL, NULL), $4);
                }
                ;

method_end:     block {
                    $$ = $1;
                }
                | TEXTERN TCOLON {
                    Node *newNode = createNonTerminalNode(EXTERN);
                    $$ = createTree(newNode, NULL, NULL);
                }
                ;

block:  TLCURLY var_decls statements TRCURLY {
            Node *newNode = createNonTerminalNode(BLOCK);
            $$ = createTree(newNode, $2, $3);
        }
        | TLCURLY statements TRCURLY {
            Node *newNode = createNonTerminalNode(BLOCK);
            $$ = createTree(newNode, $2, NULL);
        }
        | TLCURLY var_decls TRCURLY {
            Node *newNode = createNonTerminalNode(BLOCK);
            $$ = createTree(newNode, $2, NULL);
        }
        | TLCURLY TRCURLY   {
            Node *newNode = createNonTerminalNode(BLOCK);
            $$ = createTree(newNode, NULL, NULL);
        }
        ;

type:   TINTEGER { $$ = INTEGER; }
        | TBOOL { $$ = BOOLEAN; }
        ;

statements:     statements statement {
                                    Node *newNode = createNonTerminalNode(STMTS);
                                    $$ = createTree(newNode, $1, $2);
                }
                | statement {
                            Node *newNode = createNonTerminalNode(COLON);
                            $$ = createTree(newNode, $1, NULL); 
                }
                ;

statement:  TID TASSIGN expr TCOLON {
                                    Node *newNode = createNonTerminalNode(ASSIGN);
                                    Node *newId = createNode(ID, NONTYPE, NULL, $1, yylval.line_number);   
                                    Tree *idTree = createTree(newId, NULL, NULL);
                                    $$ = createTree(newNode, idTree, $3);
            }
            | method_call TCOLON {
                $$ = $1;
            }
            | if_block {
                $$ = $1;
            }
            | while_block {
                $$ = $1;
            }
            | return_block {
                $$ = $1;
            }
            | TCOLON {
                Node *newNode = createNonTerminalNode(COLON);
                $$ = createTree(newNode, NULL, NULL);
            }
            | block {
                $$ = $1;
            }
            ;

return_block:   TRETURN expr TCOLON { 
                    Node *newNode = createNonTerminalNode(RETURN);
                    $$ = createTree(newNode, $2, NULL);
                }
                | TRETURN TCOLON    {
                    Node *newNode = createNode(RETURN, NONTYPE, NULL, NULL, yylval.line_number);
                    $$ = createTree(newNode, NULL, NULL);
                }
                ;

method_call:    TID TLBRACKET expr_list TRBRACKET   {
                                                        Node *newNode = createNonTerminalNode(METHODCALL);
                                                        $$ = createTree(newNode, $1, $3);   
                                                    }
                ;

expr:   TID {
            Node *newNode = createNode(ID, NONTYPE, $1, NULL, yylval.line_number);
            $$ = createTree(newNode, NULL, NULL);
        }
        | method_call   {
                            $$ = $1;
                        }
        | literal   { 
                        $$ = $1; 
                    }
        | expr TAND expr {
                            Node *newNode = createNode(AND, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TOR expr {
                            Node *newNode = createNode(OR, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TPLUS expr {
                            Node *newNode = createNode(PLUS, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TMINUS expr {
                            Node *newNode = createNode(MINUS, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TMULTIPLY expr {
                            Node *newNode = createNode(MULTIPLY, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TDIVISION expr {
                            Node *newNode = createNode(DIVISION, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TMOD expr {
                            Node *newNode = createNode(MOD, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TLESSTHAN expr {
                            Node *newNode = createNode(LESSTHAN, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TGRATERTHAN expr {
                            Node *newNode = createNode(GRATERTHAN, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TEQUALS expr {
                            Node *newNode = createNode(EQUALS, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $1, $3);
                        }
        | TMINUS expr %prec TUNARY {
                            Node *newNode = createNode(MINUS, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $2, NULL);
                        }
        | TNOT expr %prec TUNARY {
                            Node *newNode = createNode(NOT, NONTYPE, NULL, NULL, yylval.line_number);
                            $$ = createTree(newNode, $2, NULL);
                        }
        | TLBRACKET expr TRBRACKET {
                            $$ = $2;
                        }
        ;

expr_list:  expr TCOMMA expr_list {
                Node *newNode = createNonTerminalNode(EXPRLIST);
                $$ = createTree(newNode, $1, $3);
            }
            | expr {
                Node *newNode = createNonTerminalNode(EXPRLIST);
                $$ = createTree(newNode, $1, NULL);
            }
            | %empty { $$ = NULL; }
            ;

if_block:   TIF TLBRACKET expr TRBRACKET TTHEN block else_block {
                Node *ifNode = createNonTerminalNode(IF);
                Node *thenNode = createNonTerminalNode(THEN);
                Tree *thenTree = createTree(thenNode, $6, $7);
                $$ = createTree(ifNode, $3, thenTree);
            }
            ;

else_block:     TELSE block {
                    Node *newNode = createNonTerminalNode(ELSE);
                    $$ = createTree(newNode, $2, NULL);
                }
                | %empty {
                    $$ = NULL;
                }
                ; 

while_block:    TWHILE TLBRACKET expr TRBRACKET block {
                    Node *newNode = createNonTerminalNode(WHILE); 
                    $$ = createTree(newNode, $3, $5);
                }
                ;   

literal:    integer_literal { 
                $$ = $1;
            }
            | bool_literal {
                $$ = $1; 
            }
            ;

integer_literal : TINTEGER_LITERAL {
                    Node *newNode = createNode(NUMBER, INTEGER, (void *) $1, NULL, yylval.line_number);
                    $$ = createTree(newNode, NULL, NULL);
                }
                ;

bool_literal : TBOOL_LITERAL {
                    Node *newNode = createNode(BOOL, BOOLEAN, (void *) $1, NULL, yylval.line_number);
                    $$ = createTree(newNode, NULL, NULL);
                }
            ;

%%