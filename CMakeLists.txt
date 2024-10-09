cmake_minimum_required(VERSION 3.29)
project(precompiler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(binarytree)
include_directories(symboltable)

file(GLOB_RECURSE SRC_FILES "src/*.c" "src/*.h")

add_executable(precompiler
        ${SRC_FILES}
        build/parser.tab.c
        build/lex.yy.c
        main.c
)