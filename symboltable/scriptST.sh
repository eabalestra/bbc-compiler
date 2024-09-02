#!/bin/bash
gcc -c SymbolTable.c
gcc -o main main.c Tree.o SymbolTable.o
./main