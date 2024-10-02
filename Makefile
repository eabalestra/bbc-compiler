parse:
	@mkdir -p build
	@gcc -c include/tag.h -o build/tag.c.o
	@gcc -c include/type.h -o build/type.c.o 
	@gcc -c src/tree/tree.c -o build/tree.o 
	@flex -o build/lex.yy.c src/scanner/lexer.l 
	@bison -d -v -o build/parser.tab.c src/parser/parser.y 
	@gcc -o compiler build/parser.tab.c build/lex.yy.c build/tree.o

c-tds:
	@echo "Compiling $(word 2, $(MAKECMDGOALS))"
	@./compiler $(word 2, $(MAKECMDGOALS))
	@echo "Compilation finished"

.PHONY: parse c-tds

# 2>/dev/null