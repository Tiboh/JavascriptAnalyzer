#include "pile.h"

/* For terminal color purpose */
const char* RED_TERM;
const char* BLACK_TERM;
const char* GREEN_TERM;

pile TSStack; // stack of current symbol table
pile allTable; // stack of all symbol table create

void popAndPushToStacks(const pile stackToPop, const pile stackToPush);
void writeToken(const char *codigo, const char *attributo, const char* commentario);
void writeParser(const int regla);
void writeAndDestroySymbolTable(const pile stack, const char* symbolTableFile);
int existe_entrada_tablas_anteriores(const pile stack, char *lexema);
char* concatStringInt(char* str1, int num);
void readFile(char* filePath);