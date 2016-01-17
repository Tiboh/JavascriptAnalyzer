%option noyywrap
%option yylineno
%{
	#include <stdio.h>
	#include <string.h>
	#include "y.tab.h"
	#include "global.h"
	
	extern const char* RED_TERM;
	extern const char* BLACK_TERM;
	
	extern FILE *tokenFile;
	extern FILE *errorFile;
%}

ID [A-Za-z][A-Za-z0-9_]*
ENTERO [0-9]+
CADENA \"(\\.|[^"])*\"
LOGICO ("true"|"false")

%x C_COMMENT

%%


"/*"            {ECHO; BEGIN(C_COMMENT); }
<C_COMMENT>"*/" {ECHO; BEGIN(INITIAL); }
<C_COMMENT>.    {ECHO; }

"\(" {ECHO;writeToken("ABRPAR", "", "Abrir paréntesis");yylval.p.lexema = strdup(yytext);return(ABRPAR);}

"\)" {ECHO;writeToken("CERPAR", "", "Cerrar paréntesis");yylval.p.lexema = strdup(yytext);return(CERPAR);}

"\{" {ECHO;writeToken("ABRLLAVE", "", "Abrir llave");yylval.p.lexema = strdup(yytext);return(ABRLLAVE);}

"\}" {ECHO;writeToken("CERLLAVE", "", "Cerrar llave");yylval.p.lexema = strdup(yytext);return(CERLLAVE);}

"," {ECHO;writeToken("COMA", "", "Comma");yylval.p.lexema = strdup(yytext);return(COMA);}

{ENTERO} {ECHO;writeToken("ENTERO", yytext, "Entero");return(ENTERO);}

{LOGICO} {ECHO;writeToken("LOGICO", yytext, "Logico");return(LOGICO);}

{CADENA} {ECHO;writeToken("CADENA", yytext, "Cadena");return(CADENA);}

int {ECHO;writeToken("INT", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(INT);}

chars {ECHO;writeToken("CHARS", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(CHARS);}

bool {ECHO;writeToken("BOOL", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(BOOL);}

"==" {ECHO;writeToken("OPRELIGUAL", "", "Operator relational igual");yylval.p.lexema = strdup(yytext);return(OPRELIGUAL);}

"&&" {ECHO;writeToken("OPLOGCON", "", "Operator logico conjucion");yylval.p.lexema = strdup(yytext);return(OPLOGCON);}

"++" {ECHO;writeToken("OPINCR", "", "Operator de incremento");yylval.p.lexema = strdup(yytext);return(OPINCR);}

"\+" {ECHO;writeToken("OPARSUMA", "", "Operator aritmetico suma");yylval.p.lexema = strdup(yytext);return(OPARSUMA);}

"=" {ECHO;writeToken("OPAS", "", "Operator asignacion");yylval.p.lexema = strdup(yytext);return(OPAS);}

"+=" {ECHO;writeToken("OPASSUMA", yytext, "Operator asignacion con suma");yylval.p.lexema = strdup(yytext);return(OPASSUMA);}

"var" {ECHO;writeToken("VAR", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(VAR);}

"function" {ECHO;writeToken("FUNCTION", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(FUNCTION);}

"return" {ECHO;writeToken("RETURN", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(RETURN);}

"write" {ECHO;writeToken("WRITE", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(WRITE);}

"prompt" {ECHO;writeToken("PROMPT", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(PROMPT);}

"if" {ECHO;writeToken("IF", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(IF);}

"switch" {ECHO;writeToken("SWITCH", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(SWITCH);}

"case" {ECHO;writeToken("CASE", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(CASE);}

"default" {ECHO;writeToken("DEFAULT", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(DEFAULT);}

":" {ECHO;writeToken("DOBLEPUNTOS", "", "Doble puntos");yylval.p.lexema = strdup(yytext);return(DOBLEPUNTOS);}

"break" {ECHO;writeToken("BREAK", yytext, "Palabra reservada");yylval.p.lexema = strdup(yytext);return(BREAK);}

{ID} {ECHO;writeToken("ID", yytext, "Identificador");yylval.p.lexema = strdup(yytext);return(ID);}

[ \t\n]+	{ECHO;/* ignorar blancos, tabuladores y saltos de línea */}

. {
  fprintf(errorFile, " %sERROR LEXICO: Caracter no valido: %s%s\n", RED_TERM, yytext, BLACK_TERM);
}

%%