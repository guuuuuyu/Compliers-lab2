main: syntax.tab.c lex.yy.c syntax.tab.c node.c symboltable.c
	gcc main.c syntax.tab.c node.c symboltable.c -lfl -ly -o main

syntax.tab.c: syntax.y
	bison -d syntax.y

lex.yy.c: lexical.l
	flex lexical.l


clean:
	rm -rf main
	rm -rf syntax.tab.h
	rm -rf syntax.tab.c
	rm -rf lex.yy.c
