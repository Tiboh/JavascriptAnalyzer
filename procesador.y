%{
	#include <stdio.h>
	
	extern const char* RED_TERM;
	extern const char* BLACK_TERM;
	
	extern int yylineno;
	
	void yyerror (char const *s) {
	   fprintf (stderr,  " %sERROR SINTÁCTICO: (Line:%d) %s%s\n" , RED_TERM, yylineno, s, BLACK_TERM);
	}
%}

%token ID
%token ABRPAR CERPAR ABRLLAVE CERLLAVE
%token COMA DOBLEPUNTOS
%token ENTERO CADENA INT CHARS BOOL LOGICO
%token OPRELIGUAL OPLOGCON OPINCR OPARSUMA OPAS OPASSUMA
%token VAR FUNCTION RETURN WRITE PROMPT IF SWITCH CASE BREAK DEFAULT

%right OPASSUMA OPAS
%left OPLOGCON
%left OPRELIGUAL
%left OPARSUMA
%right OPINCR 

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
	b
	| ABRLLAVE c CERLLAVE
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
	| WRITE ABRPAR l CERPAR
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
	| b j i 
	;	

g2:
	/* empty */
	| b 
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
	r e1
	;
	
e1:
	/* empty */
	| OPLOGCON r e1
	;
	
r:	
	u r1
	;

r1:
	/* empty */
	| OPRELIGUAL u r1
	;

u:
	v u1
	;

u1:
	/* empty */
	| OPARSUMA v u1
	;
	
v:
	ID v1
	| ABRPAR e CERPAR
	| ENTERO
	| CADENA
	| OPINCR ID
	| LOGICO
	;

v1:
	/* empty */
	| ABRPAR l CERPAR
	;

%%