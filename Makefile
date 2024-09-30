parse:
	@mkdir -p build
	@flex -o build/lex.yy.c src/scanner/lexer.l 2>/dev/null
	@bison -d -v -o build/parser.tab.c src/parser/parser.y 2>/dev/null
	@gcc -o compiler build/parser.tab.c build/lex.yy.c 2>/dev/null

c-tds:
	@echo "Compiling $(word 2, $(MAKECMDGOALS))"
	@./compiler $(word 2, $(MAKECMDGOALS)) 2>/dev/null
	@echo "Compilation finished"

.PHONY: parse c-tds
