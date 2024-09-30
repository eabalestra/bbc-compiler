%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>

    #include "../include/tree.h"
    #include "../include/type.h"
    
    Tree *ast;
%}

%union{
    enum Type *vtype;
    struct Tree *vast;
    char *vstring;
    int vint;
    int line_number;
    bool vbool;
}

%type<vtype> type

%type<vast> program
%type<vast> var_decls
%type<vast> var_decl
%type<vast> method_decls
%type<vast> method_decl


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

%type<vast> program
%type<vast> var_decls
%type<vast> var_decl
%type<vast> method_decls
%type<vast> method_decl
%type<vast> params
%type<vast> params_list
%type<vast> method_end
%type<vast> block
%type<vast> type
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

program:    TPROGRAM TLCURLY var_decls method_decls TRCURLY { printf("PARSER OK\n"); 
                Node *newNode = createNode(PROG, NONTYPE, NULL, NULL, NULL);        
                $$ = createTree(node, $3, $4);
            }
            | TPROGRAM TLCURLY method_decls TRCURLY { printf("PARSER OK\n"); 
                Node *newNode = createNode(PROG, NONTYPE, NULL, NULL, NULL);        
                $$ = createTree(node, $3, NULL);
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
                Node *newNode = createNode(ID, *$1, NULL, $2, yylval.lyylineno);
                Tree *idTree = createTree(newNode, NULL, NULL);
                $$ = createTree(createNode(VARDECL, NONTYPE, NULL, NULL), idTree, $4);
            }
            | type TID TCOLON {
                Node *newNode = createNode(ID, *$1, NULL, $2, yylval.lyylineno);
                Tree *idTree = createTree(newNode, NULL, NULL);
                $$ = createTree(createNode(VARDECL, NONTYPE, NULL, NULL), idTree, NULL);
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
                    $$ = createTree(newNode, $1, createTree(createNonTerminalNode(METHODDECL),));
                }
                | TVOID TID TLBRACKET params TRBRACKET method_end
                ;

params:     params_list
            | %empty { $$ = NULL; }
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

type:   TINTEGER { $$ = $1; }
        | TBOOL { $$ = $1; }
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
                                    Node *newId = createNode(ID, NONTYPE, NULL, $1, yylval.lyylineno);   
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
                    Node *newNode = createNonTerminalNode(RETURN);
                    $$ = createTree(newNode, NULL, NULL);
                }
                ;

method_call:    TID TLBRACKET expr_list TRBRACKET  {
                                                        Node *newNode = createNonTerminalNode(METHODCALL);
                                                        $$ = createTree(newNode, $1, $3);   
                                                    }
                ;

expr:   TID {
            $$ = $1;
        }
        | method_call   {
                            $$ = $1;
                        }
        | literal   { 
                        $$ = $1; 
                    }
        | expr TAND expr {
                            Node *newNode = createNonTerminalNode(AND);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TOR expr {
                            Node *newNode = createNonTerminalNode(OR);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TPLUS expr {
                            Node *newNode = createNonTerminalNode(PLUS);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TMINUS expr {
                            Node *newNode = createNonTerminalNode(MINUS);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TMULTIPLY expr {
                            Node *newNode = createNonTerminalNode(MULTIPLY);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TDIVISION expr {
                            Node *newNode = createNonTerminalNode(DIVISION);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TMOD expr {
                            Node *newNode = createNonTerminalNode(MOD);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TLESSTHAN expr {
                            Node *newNode = createNonTerminalNode(LESSTHAN);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TGRATERTHAN expr {
                            Node *newNode = createNonTerminalNode(GRATERTHAN);
                            $$ = createTree(newNode, $1, $3);
                        }
        | expr TEQUALS expr {
                            Node *newNode = createNonTerminalNode(EQUALS);
                            $$ = createTree(newNode, $1, $3);
                        }
        | TMINUS expr %prec TUNARY {
                            Node *newNode = createNonTerminalNode(MINUS);
                            $$ = createTree(newNode, $2, NULL);
                        }
        | TNOT expr %prec TUNARY {
                            Node *newNode = createNonTerminalNode(NOT);
                            $$ = createTree(newNode, $2, NULL);
                        }
        | TLBRACKET expr TRBRACKET
        ;

expr_list:  expr TCOMMA expr_list
            | expr 
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
                    $$ = createTree(newNode, $3, $4);
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
                    Node *newNode = createNode(NUMBER, INTEGER, $1, NULL, yylval.lyylineno);
                    $$ = createTree(newNode, NULL, NULL);
                }
                ;

bool_literal : TBOOL_LITERAL {
                    Node *newNode = createNode(BOOL, BOOLEAN, $1, NULL, yylval.lyylineno);
                    $$ = createTree(newNode, NULL, NULL);
                }
            ;

%%