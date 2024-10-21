#ifndef PRECOMPILER_TYPECHECKER_H
#define PRECOMPILER_TYPECHECKER_H

#include "tree.h"

void checkTypes(Tree *tree);
Type checkExpressionTypes(Tree *rightChild);

#endif //PRECOMPILER_TYPECHECKER_H