#ifndef PRECOMPILER_TYPECHECKER_H
#define PRECOMPILER_TYPECHECKER_H

#include "tree.h"

void checkTypes(Tree *tree);
void validateTypeCompatibility(Tree *lTree, Type *leftChildType, Type *rightChildType);

#endif //PRECOMPILER_TYPECHECKER_H