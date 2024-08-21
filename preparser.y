%{
#include <stdlib.h>
#include <stdio.h>
%}

%token TYPE
%token RETURN
%token SEMICOLON
%token ASSIGN
%token PLUS
%token MULTIPLY
%token OPEN_BRACKET
%token CLOSE_BRACKET
%token OPEN_CURLY_BRACKET
%token CLOSE_CURLY_BRACKET
%token MAIN
%token INT
%token ID

%type body
%type sentence
%type assignment
%type statement
%type returns
%type expr
%type value
    
%left '+' PLUS
%left '*' MULTIPLY

%%

prog: TYPE MAIN OPEN_BRACKET CLOSE_BRACKET OPEN_CURLY_BRACKET body CLOSE_CURLY_BRACKET    { printf("No hay errores \n"); }
    ;

body: sentence body
    | %empty
    ;

sentence:   assignment SEMICOLON
            | statement SEMICOLON
            | returns SEMICOLON
            ;

assignment:     TYPE ID ASSIGN expr
                | ID ASSIGN expr
                ;

statement:      TYPE ID
                ;

returns:        RETURN expr
                | RETURN
                ;

expr:           value
                | expr PLUS expr
                | expr MULTIPLY expr
                | OPEN_BRACKET expr CLOSE_BRACKET
                | ID
                ;

value:  INT
        ;

%%
