%{
	#include <stdio.h>
	
	#define KRED  "\x1B[31m" // RED COLOR CODE
	#define RESET "\033[0m" // RESET COLOR CODE
	
	extern int yylineno;
	
	void yyerror (char const *s) {
	   fprintf (stderr, KRED " (Line:%d) %s\n" RESET, yylineno, s);
	}
%}

%token ID
%token ABRPAR CERPAR ABRLLAVE CERLLAVE
%token COMA DOBLEPUNTOS PUNTOCOMA
%token ENTERO CADENA INT CHARS BOOL
%token OPRELIGUAL OPLOGNEG OPINCR OPARSUMA OPAS OPASSUMA
%token VAR FUNCTION RETURN WRITE PROMPT IF SWITCH CASE BREAK DEFAULT

%right OPASSUMA OPAS
%left OPRELIGUAL
%left OPARSUMA
%right OPINCR OPLOGNEG

%% 

p:
	/* empty */
	| b p
	| f p
	;

b:
	VAR t ID
	| IF ABRPAR e CERPAR s
	| SWITCH ABRPAR ID CERPAR ABRLLAVE g CERLLAVE
	| s
	;

t:
	INT
	| BOOL
	| CHARS
	;

f:
	FUNCTION h ID ABRPAR a CERPAR ABRLLAVE c CERLLAVE
	;

a:
	/* empty */
	| t ID k
	;

k:
	/* empty */
	| COMA t ID k
	;

s:
	ID s2
	| RETURN e
	| WRITE ABRPAR e CERPAR
	| PROMPT ABRPAR ID CERPAR
	;

s2:
	OPAS e
	| ABRPAR l CERPAR
	| OPASSUMA e
	;

g:
	CASE e DOBLEPUNTOS s j i
	;

j:
	/* empty */
	| BREAK
	;

i: 
	/* empty */
	| g
	;

c:
	/* empty */
	| b c
	;

h:
	/* empty */
	| t
	;

l:
	/* empty */
	| e q
	;

q:
	/* empty */
	| COMA e q
	;

e:
	OPRELIGUAL u
	| u
	;

u:
	OPARSUMA v
	| v
	;

v:
	ID v2
	| ABRPAR e CERPAR
	| ENTERO
	| CADENA
	| OPINCR ID
	| OPLOGNEG e
	;

v2:
	/* empty */
	| OPINCR
	| ABRPAR l CERPAR
	;

%%