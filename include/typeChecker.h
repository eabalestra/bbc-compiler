#ifndef PRECOMPILER_TYPECHECKER_H
#define PRECOMPILER_TYPECHECKER_H

#include "tree.h"

Type checkTypes(Tree *tree);
void checkParameters(Tree *method, Tree *formalParameters, Tree *actualParameters);
Type getExprType(Tree *tree);

#endif //PRECOMPILER_TYPECHECKER_H