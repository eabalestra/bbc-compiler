cmake_minimum_required(VERSION 3.29)
project(precompiler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(binarytree)
include_directories(symboltable)

add_executable(precompiler
        src/symboltable/symbolTable.c
        src/tree/tree.c
        src/symboltable/main2.c
        src/node/node.c
)