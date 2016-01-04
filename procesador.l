%option noyywrap
%option yylineno
%{
	#include <stdio.h>
	#include "y.tab.h"
	
	extern FILE *outFile_p;
	extern void writeToken(const char *codigo, const char *attributo, const char* commentario);
%}

ID [A-Za-z][A-Za-z0-9_]*
ENTERO [0-9]+
CADENA \"(\\.|[^"])*\"

%x C_COMMENT

%%


"/*"            {ECHO; BEGIN(C_COMMENT); }
<C_COMMENT>"*/" {ECHO; BEGIN(INITIAL); }
<C_COMMENT>.    {ECHO; }

[[:blank:]]+ {ECHO;/* Do nothing */}

"\n" {ECHO;/* Do nothing */}


"\(" {ECHO;writeToken("ABRPAR", "", "Abrir paréntesis");return(ABRPAR);}

"\)" {ECHO;writeToken("CERPAR", "", "Cerrar paréntesis");return(CERPAR);}

"\{" {ECHO;writeToken("ABRLLAVE", "", "Abrir llave");return(ABRLLAVE);}

"\}" {ECHO;writeToken("CERLLAVE", "", "Cerrar llave");return(CERLLAVE);}

"," {ECHO;writeToken("COMA", "", "Comma");return(COMA);}

{ENTERO} {ECHO;writeToken("ENTERO", yytext, "Entero");return(ENTERO);}

{CADENA} {ECHO;writeToken("CADENA", yytext, "Cadena");return(CADENA);}

int {ECHO;writeToken("INT", yytext, "Palabra reservada");return(INT);}

chars {ECHO;writeToken("CHARS", yytext, "Palabra reservada");return(CHARS);}

bool {ECHO;writeToken("BOOL", yytext, "Palabra reservada");return(BOOL);}

"==" {ECHO;writeToken("OPRELIGUAL", "", "Operator relational igual");return(OPRELIGUAL);}

"&&" {ECHO;writeToken("OPLOGCON", "", "Operator logico conjucion");return(OPLOGCON);}

"++" {ECHO;writeToken("OPINCR", "", "Operator de incremento");return(OPINCR);}

"\+" {ECHO;writeToken("OPARSUMA", "", "Operator aritmetico suma");return(OPARSUMA);}

"=" {ECHO;writeToken("OPAS", "", "Operator asignacion");return(OPAS);}

"+=" {ECHO;writeToken("OPASSUMA", yytext, "Operator asignacion con suma");return(OPASSUMA);}

"var" {ECHO;writeToken("VAR", yytext, "Palabra reservada");return(VAR);}

"function" {ECHO;writeToken("FUNCTION", yytext, "Palabra reservada");return(FUNCTION);}

"return" {ECHO;writeToken("RETURN", yytext, "Palabra reservada");return(RETURN);}

"write" {ECHO;writeToken("WRITE", yytext, "Palabra reservada");return(WRITE);}

"prompt" {ECHO;writeToken("PROMPT", yytext, "Palabra reservada");return(PROMPT);}

"if" {ECHO;writeToken("IF", yytext, "Palabra reservada");return(IF);}

"switch" {ECHO;writeToken("SWITCH", yytext, "Palabra reservada");return(SWITCH);}

"case" {ECHO;writeToken("CASE", yytext, "Palabra reservada");return(CASE);}

"default" {ECHO;writeToken("DEFAULT", yytext, "Palabra reservada");return(DEFAULT);}

":" {ECHO;writeToken("DOBLEPUNTOS", "", "Doble puntos");return(DOBLEPUNTOS);}

"break" {ECHO;writeToken("BREAK", yytext, "Palabra reservada");return(BREAK);}

{ID} {ECHO;writeToken("ID", yytext, "Identificador");return(ID);}

%%