#ifndef TAG_H
#define TAG_H

typedef enum Tag {
    PROG, BODY, EXPR, NUMBER, ID, PLUS, MULTIPLY,
    ASSIGN, COLON, DECLS, STMTS, RETURN, DECL, TYPE, VAR,
    BOOL, AND, OR, WHILE, IF, THEN, ELSE, LESSTHAN, GRATERTHAN,
    EQUALS, MINUS, NOT, METHODDECLS, VARDECL, METHODDECL, METHODEND, 
    EXPRLIST, MOD, EXTERN, BLOCK, METHODCALL, DIVISION,
    EMPTY, PARAM, CALL, LABEL, INITMETHOD, ENDMETHOD, GOTO, JMPF, GASSIGN,
    TEMP, WASSIGN, SUBTRACTION, INIT_EXTERNAL_METHOD, END_EXTERNAL_METHOD, EXTERNAL_CALL
} Tag;

#endif // TAG_H