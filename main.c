#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

extern int  yyparse();
extern FILE *yyin;

FILE *tokenFile;
FILE *symbolTableFile;
FILE *parserFile;
FILE *errorFile;

int main(int argc,char *argv[])
{
	RED_TERM = "";
	BLACK_TERM = "";
	GREEN_TERM = "";
		
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
	errorFile=fopen(argv[5],"a");
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
	
	fprintf(parserFile, "Descendente");
	
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

void writeParser(const int regla){
	fprintf(parserFile, " %d", regla);
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
	int retour = -1;
	int incr;
	for (incr = 0; incr < pile_taille(stack) ; incr++){
		int tableID = pile_valeur_position(stack, incr);
		if(existe_entrada(tableID,lexema) == 0){
			retour = tableID;
		}
	}
	return retour;
}

char* concatStringInt(char* str1, int num){
	char str2[255];
	sprintf(str2, "%d", num);
	char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2));
	strcpy(str3, str1);
	strcat(str3, str2);
	return str3;
}