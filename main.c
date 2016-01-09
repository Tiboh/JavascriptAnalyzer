#include <stdio.h>
#include <stdlib.h>
#include "global.h"

extern int  yyparse();
extern FILE *yyin;

FILE *tokenFile;
FILE *symbolTableFile;
FILE *parserFile;
FILE *errorFile;

int main(int argc,char *argv[])
{
	RED_TERM = "\033[31;40m";
	BLACK_TERM = "\033[37;40m";
		
	if(argc<6)
	{
		printf("More arguments needed. (ex: myAnalyzer.exe input.txt token.txt symbolTable.txt parser.txt error.txt)\n");
		exit(0);
	}
	
	/* Init input and output files */
	FILE *inputFile=fopen(argv[1],"r");
	if(!inputFile)
	{
		printf("Couldn't open %s file for reading\n", argv[1]);
		exit(0);
	}
	tokenFile=fopen(argv[2],"w");
	if(!tokenFile)
	{
		printf("Couldn't open %s file for writting\n", argv[2]);
		exit(0);
	}
	symbolTableFile=fopen(argv[3],"w");
	if(!tokenFile)
	{
		printf("Couldn't open %s file for writting\n", argv[3]);
		exit(0);
	}
	parserFile=fopen(argv[4],"w");
	if(!tokenFile)
	{
		printf("Couldn't open %s file for writting\n", argv[4]);
		exit(0);
	}
	errorFile=fopen(argv[5],"w");
	if(!tokenFile)
	{
		printf("Couldn't open %s file for writting\n", argv[5]);
		exit(0);
	}
	yyin=inputFile;
	
	/* Create symbol table stack and the global symbol table*/
	TSStack = pile_creer(); // Create stack of current symbol table
	allTable = pile_creer(); // Create stack of all symbol table create

	int globalSymbolTable = crear_tabla();
	pile_empile(TSStack, globalSymbolTable);
	
	yyparse();
	
	fclose(inputFile);
	fclose(tokenFile);
	fclose(symbolTableFile);
	fclose(parserFile);
	fclose(errorFile);
	
	popAndPushToStacks(TSStack,allTable); // Pop and push the global table
	
	writeAndDestroySymbolTable(allTable, argv[3]);
	
	/* Destroy stacks */
	pile_detruire(TSStack);
	pile_detruire(allTable);

	return 0;
}

void writeToken(const char *codigo, const char *attributo, const char* commentario){
	fprintf(tokenFile, "<%s,%s> // token %s\n", codigo, attributo, commentario);
}

void popAndPushToStacks(const pile stackToPop, const pile stackToPush){
	pile_empile(stackToPush,pile_depile(stackToPop));
}

void writeAndDestroySymbolTable(const pile stack, const char* symbolTableFile){
	int tableId;
	while(!pile_vide(stack)){
		tableId = pile_depile(stack);
		escribir_tabla(tableId,symbolTableFile);
		destruir_tabla(tableId);
	}
}

int existe_entrada_tablas_anteriores(const pile stack, char *lexema){
	int retour;
	if(pile_vide(stack)){
		retour = -1;
	}else{
		retour = 0;
	}
	int incr;
	for (incr = 0; incr < pile_taille(stack) ; incr++){
		if(existe_entrada(pile_valeur_position(stack, incr),lexema) == 0){
			retour = 1;
		}
	}
	return retour;
}