#ifndef PRECOMPILER_NODE_H
#define PRECOMPILER_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tag.h"
#include "type.h"

// Forward declaration of Tree structure
struct Tree;

typedef struct Node {
    Tag flag;
    Type type;
    void *value;
    char *name;
    int line_number;
    struct Tree *parameters;  // Use the forward-declared Tree structure here
} Node;

Node* createNonTerminalNode(Tag flag);
Node* createNode(Tag flag, Type type, void *value, char *name, int line_number);
char* nodeFlagToString(Tag flag);
char *nodeTypeToString(Type type); 

#endif //PRECOMPILER_NODE_H