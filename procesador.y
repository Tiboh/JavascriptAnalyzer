%{
	#include <stdio.h>
	
	extern int yylineno;
	
	void yyerror (char const *s) {
	   fprintf (stderr,  " (Line:%d) %s\n" , yylineno, s);
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
	| IF ABRPAR e CERPAR b1
	| SWITCH ABRPAR ID CERPAR ABRLLAVE g CERLLAVE
	| s
	;
	
b1:
	s
	| ABRLLAVE s CERLLAVE
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
	RETURN e
	| WRITE ABRPAR e CERPAR
	| PROMPT ABRPAR ID CERPAR
	| ID s2
	;

s2:
	OPAS e
	| ABRPAR l CERPAR
	| OPASSUMA e
	;

g:
	CASE e DOBLEPUNTOS g1
	| DEFAULT DOBLEPUNTOS g2
	;
	
g1:
	i
	| s j i
	| IF ABRPAR e CERPAR s
	;	

g2:
	/* empty */
	| s j
	| IF ABRPAR e CERPAR s j
	;		

j:
	/* empty */
	| PUNTOCOMA BREAK
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
	r e1
	;
	
e1:
	/* empty */
	| OPRELIGUAL r e1
	;
	
r:	
	/* empty */
	| u r1
	;

r1:
	/* empty */
	| OPARSUMA u r1
	;

u:
	v u1
	;

u1:
	/* empty */
	| OPLOGNEG v u1
	;
	
v:
	ID v1
	| ABRPAR e CERPAR
	| ENTERO
	| CADENA
	| OPINCR ID
	;

v1:
	/* empty */
	| ABRPAR l CERPAR
	;

%%