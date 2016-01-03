LEX = lex
YACC = yacc
CC = gcc

EXEC = procesador

all: $(EXEC)

procesador: procesador.tab.o lex.yy.o main.o
	$(CC) -o $@ $^
	
procesador.tab.o: procesador.tab.c
	$(CC) -c $^ -o $@

lex.yy.o: lex.yy.c y.tab.h
	$(CC) -c $< -o $@
	
main.o: main.c
	$(CC) -c $< -o $@

y.tab.h: procesador.tab.h
	ren $^ $@

procesador.tab.c procesador.tab.h: procesador.y
	$(YACC) -d $^

lex.yy.c: procesador.l
	$(LEX) $^

.PHONY: clean

clean:
	-rm -f *.o lex.yy.c *.tab.*  procesador.exe *.output