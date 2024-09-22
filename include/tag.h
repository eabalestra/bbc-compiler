#ifndef TAG_H
#define TAG_H

typedef enum Tag {
    PROG, BODY, EXPR, NUMBER, ID, PLUS, MULTIPLY,
    ASSIGN, COLON, DECLS, STMTS, RETURN, DECL, TYPE, VAR,
    BOOL, AND, OR
} Tag;

#endif // TAG_H