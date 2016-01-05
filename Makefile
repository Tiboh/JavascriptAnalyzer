LEX = lex
YACC = yacc
CC = gcc

EXEC = procesador

all: $(EXEC)

$(EXEC): $(EXEC).tab.o lex.yy.o main.o ts2006.o pile.o
	$(CC) -o $@ $^
	
$(EXEC).tab.o: $(EXEC).tab.c
	$(CC) -c $^ -o $@

lex.yy.o: lex.yy.c y.tab.h
	$(CC) -c $< -o $@
	
main.o: main.c
	$(CC) -c $< -o $@
	
ts2006.o: ts2006.c ts2006.h
	$(CC) -c $< -o $@
	
pile.o: pile.c pile.h
	$(CC) -c $< -o $@

y.tab.h: $(EXEC).tab.h
	ren $^ $@

$(EXEC).tab.c $(EXEC).tab.h: $(EXEC).y
	$(YACC) -d $^

lex.yy.c: $(EXEC).l
	$(LEX) $^

.PHONY: clean

clean:
	-rm -f *.o lex.yy.c *.tab.*  $(EXEC).exe *.txt