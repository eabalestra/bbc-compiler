build:
	@mkdir -p build
	@gcc -c include/tag.h -o build/tag.c.o 2>/dev/null
	@gcc -c include/type.h -o build/type.c.o 2>/dev/null
	@gcc -c src/tree/tree.c -o build/tree.o 2>/dev/null
	@gcc -c src/node/node.c -o build/node.o 2>/dev/null
	@gcc -c src/symbollist/symbolList.c -o build/symbolList.o 2>/dev/null
	@gcc -c src/utils/typeChecker.c -o build/typeChecker.o 2>/dev/null
	@gcc -c src/symboltable/symbolTable.c -o build/symboltable.o 2>/dev/null
	@gcc -c src/quadruple/quadruple.c -o build/quadruple.o 2>/dev/null
	@gcc -c src/quadrupleLinkedList/quadrupleLinkedList.c -o build/quadrupleLinkedList.o 2>/dev/null
	@gcc -c src/threeaddresscodegenerator/threeAddressGenerator.c -o build/threeAddressGenerator.o 2>/dev/null
	@gcc -c src/assemblycodegenerator/assemblyCodeGenerator.c -o build/assemblyCodeGenerator.o 2>/dev/null
	@flex -o build/lex.yy.c src/scanner/lexer.l 2>/dev/null
	@bison -d -v -o build/parser.tab.c src/parser/parser.y 2>/dev/null
	@gcc -o compiler main.c build/parser.tab.c build/lex.yy.c build/tree.o build/symboltable.o build/node.o build/symbolList.o build/typeChecker.o build/quadruple.o build/quadrupleLinkedList.o build/threeAddressGenerator.o build/assemblyCodeGenerator.o 2>/dev/null

c-tds:
	@echo "Compiling $(word 2, $(MAKECMDGOALS))"
	@./compiler $(word 2, $(MAKECMDGOALS))
	@echo "Compilation finished"

clean:
	@rm -rf build
	@rm -f compiler
	@rm -f output.s
	@rm -f output.out

.PHONY: build c-tds clean

# 2>/dev/null