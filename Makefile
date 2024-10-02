parse:
	@mkdir -p build
	@gcc -c include/tag.h -o build/tag.c.o 2>/dev/null
	@gcc -c include/type.h -o build/type.c.o 2>/dev/null
	@gcc -c src/tree/tree.c -o build/tree.o 2>/dev/null
	@flex -o build/lex.yy.c src/scanner/lexer.l 2>/dev/null
	@bison -d -v -o build/parser.tab.c src/parser/parser.y 2>/dev/null
	@gcc -o compiler build/parser.tab.c build/lex.yy.c build/tree.o 2>/dev/null

c-tds:
	@echo "Compiling $(word 2, $(MAKECMDGOALS))"
	@./compiler $(word 2, $(MAKECMDGOALS))
	@echo "Compilation finished"

.PHONY: parse c-tds
