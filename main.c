#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

/* For terminal color purpose */
const char* RED_TERM = "\033[31;40m";
const char* BLACK_TERM = "\033[37;40m";

extern int  yyparse();
extern FILE *yyin;
FILE *outFile_p;

int main(int argc,char *argv[])
{
		
	if(argc<3)
	{
		printf("Please specify the input file & output file\n");
		exit(0);
	}
	FILE *fp=fopen(argv[1],"r");
	if(!fp)
	{
		printf("Couldn't open file for reading\n");
		exit(0);
	}
	outFile_p=fopen(argv[2],"w");
	if(!outFile_p)
	{
		printf("Couldn't open output file for writting\n");
		exit(0);
	}
	yyin=fp;
	yyparse();
	fclose(fp);
	fclose(outFile_p);
	return 0;
}

void writeToken(const char *codigo, const char *attributo, const char* commentario){
	fprintf(outFile_p, "<%s,%s> // token %s\n", codigo, attributo, commentario);
}