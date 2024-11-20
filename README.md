# TDS24/BBC Compiler
## Getting Started
### Prerequisites

Make sure you have `git` and `make` installed on your system.

### Cloning the Repository

To get a copy of the project, clone the repository using the following command:

```
git clone https://github.com/eabalestra/bbc-compiler.git 
cd bbc-compiler
```

### Building the Project
To compile the lexer and parser, run:
```
make build
```

### Running the Project
To execute the project, run:
```
make c-tds inputs/input.ctds
```

### Compile, link and execute the program
clear && make clean && make build && make c-tds inputs/input5.ctds && gcc output.s functions.c && ./a.out

### Running the project's tests
Each test with different scenarios are included in the repository ```inputs```. You can choose to runa specific one by changing the name of the input file in the command above.