#ifndef THREE_ADDRESS_GENERATOR_H
#define THREE_ADDRESS_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "quadruple.h"
#include "quadrupleLinkedList.h"

Node *generateThreeAddressCode(Tree *tree);
QuadrupleLinkedList *getQuadrupleList();

#endif // THREE_ADDRESS_GENERATOR_H