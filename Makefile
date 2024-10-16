parse:
	@mkdir -p build
	@gcc -c include/tag.h -o build/tag.c.o
	@gcc -c include/type.h -o build/type.c.o
	@gcc -c src/node/node.c -o build/node.o
	@gcc -c src/symbollist/symbolList.c -o build/symbolList.o
	@gcc -c src/tree/tree.c -o build/tree.o
	@gcc -c src/utils/typeChecker.c -o build/typeChecker.o
	@gcc -c src/symboltable/symbolTable.c -o build/symboltable.o
	@flex -o build/lex.yy.c src/scanner/lexer.l
	@bison -d -v -o build/parser.tab.c src/parser/parser.y 
	@gcc -o compiler main.c build/parser.tab.c build/lex.yy.c build/tree.o build/symboltable.o build/node.o build/symbolList.o build/typeChecker.o

c-tds:
	@echo "Compiling $(word 2, $(MAKECMDGOALS))"
	@./compiler $(word 2, $(MAKECMDGOALS))
	@echo "Compilation finished"

.PHONY: parse c-tds

# 2>/dev/null