#ifndef PRECOMPILER_TYPECHECKER_H
#define PRECOMPILER_TYPECHECKER_H

#include "tree.h"

void checkTypes(Tree *tree);
void checkParameters(Tree *method, Tree *formalParameters, Tree *actualParameters);

Type checkExpressionTypes(Tree *tree);

#endif //PRECOMPILER_TYPECHECKER_H