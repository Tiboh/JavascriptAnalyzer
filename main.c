#include <stdio.h>
#include <stdlib.h>
extern int  yyparse();
extern FILE *yyin;
FILE *outFile_p;

main(int argc,char *argv[])
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
}

void writeToken(const char *codigo, const char *attributo, const char* commentario){
	fprintf(outFile_p, "<%s,%s> // token %s\n", codigo, attributo, commentario);
}