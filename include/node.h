#ifndef PRECOMPILER_NODE_H
#define PRECOMPILER_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tag.h"
#include "type.h"

typedef struct Node {
    Tag flag;
    Type type;
    void *value;
    char *name;
    int line_number;
} Node;

Node* createNonTerminalNode(Tag flag);
Node* createNode(Tag flag, Type type, void *value, char *name, int line_number);
char* nodeFlagToString(Tag flag);

#endif //PRECOMPILER_NODE_H