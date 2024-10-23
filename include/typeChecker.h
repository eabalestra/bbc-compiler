#ifndef PRECOMPILER_TYPECHECKER_H
#define PRECOMPILER_TYPECHECKER_H

#include "tree.h"

void checkTypes(Tree *tree);
Type checkExpressionTypes(Tree *rightChild);
void checkParameters(Tree *method, Tree *formalParameters, Tree *actualParameters);

#endif //PRECOMPILER_TYPECHECKER_H