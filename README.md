# TDS24/BBC Compiler

## Overview
The **TDS24/BBC Compiler** translates `.ctds` source files into assembly code. This guide will help you set up, build, and run the compiler, as well as execute test cases.

---

## Getting Started

### Prerequisites
Ensure the following tools are installed on your system:
- **Git**: For cloning the repository.
- **Make**: To build and manage the project.

---

### Cloning the Repository
Clone the repository to your local machine with:

```bash
  git clone https://github.com/eabalestra/bbc-compiler.git 
  cd bbc-compiler
```

### Building the Project
To build the compiler, use the following command:
```bash
  make build
```
This generates the required files to execute the compiler.

### Running the Compiler
To process a .ctds file and generate assembly code:

```bash
  make c-tds inputs/input.ctds
```
This will produce an assembly file named `output.s`.

### Compiling the Assembly Code
You can compile the generated assembly code using gcc like a typical C file:
```bash
  gcc output.s
```
If you need to link it with additional functions, you can do so:
```bash
  gcc output.s functions.c
```
This will create an executable named a.out. Run it with:
```bash
  ./a.out
```

### Running Test Cases
The repository includes multiple .ctds input files under the inputs/ directory. 
To test a specific scenario, modify the input file path in the Makefile and run:
```bash
  make test
```


