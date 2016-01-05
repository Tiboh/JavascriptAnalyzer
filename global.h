#include "pile.h"

void popAndPushToStacks(const pile stackToPop, const pile stackToPush);
void writeToken(const char *codigo, const char *attributo, const char* commentario);
void writeAndDestroySymbolTable(const pile stack, const char* symbolTableFile);