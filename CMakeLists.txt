cmake_minimum_required(VERSION 3.29)
project(precompiler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(binarytree)
include_directories(symboltable)

add_executable(precompiler
        main.c
        binarytree/Tree.c
        binarytree/Tree.h
        symboltable/SymbolTable.c
        symboltable/SymbolTable.h
)