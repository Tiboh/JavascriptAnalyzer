%{
	#include <stdio.h>
	#include "global.h"
	
	extern int yylineno;
	
	char* currentID;
	int contador;

	void yyerror (char const *s) {
	   fprintf (stderr,  " %sERROR SINTÁCTICO: (Line:%d) %s%s\n" , RED_TERM, yylineno, s, BLACK_TERM);
	}
%}

%union{

struct{
	char* lexema;
	int valueInt;
	char* valueChar;
	char* tipo;
	char* vuelta;
	}p;
}

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
	VAR t ID {
		if (!existe_entrada_tablas_anteriores(TSStack,$<p.lexema>3)) 
		{
			int currentTable = pile_valeur(TSStack);
			crear_entrada(currentTable,$<p.lexema>3);
			asignar_tipo_entrada(currentTable, $<p.lexema>3, "variable");
		}
		else
		{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator %s ya existe%s\n",RED_TERM, yylineno, $<p.lexema>3,BLACK_TERM);
		}
	}
	| IF ABRPAR e { 
		if(strcmp($<p.tipo>3,"int") && strcmp($<p.tipo>3,"bool")){
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): condicion del IF debe ser de tipo int o bool%s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	} CERPAR b1
	| SWITCH ABRPAR ID {
		if (!existe_entrada_tablas_anteriores(TSStack,$<p.lexema>3)) 
		{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator %s no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>3,BLACK_TERM);
		}
	} CERPAR ABRLLAVE g CERLLAVE
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
	FUNCTION h ID { currentID = $<p.lexema>3; }
	ABRPAR a CERPAR {
		int globalTable = pile_valeur(TSStack);
		crear_entrada(globalTable,$<p.lexema>3);
		asignar_tipo_entrada(globalTable, $<p.lexema>3, "funcion");
		crear_atributo_cadena(globalTable, $<p.lexema>3, "tipo", $<p.tipo>2);
		int functionTable = crear_tabla();
		crear_atributo_entero(globalTable, $<p.lexema>3, "idtabla", functionTable);
		pile_empile(TSStack, functionTable);
	} 
	ABRLLAVE c {
			fprintf(stderr," %s HEREE %s\n",RED_TERM, BLACK_TERM);
			/*if(strcmp($<p.tipo>2,$<p.tipo>10)){
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): tipo de funcion %s y su valor de vuelta no corresponde %s/%s%s\n",RED_TERM, yylineno, $<p.lexema>3, $<p.tipo>2, $<p.tipo>10, BLACK_TERM);
			}*/
	} 
	CERLLAVE {
		popAndPushToStacks(TSStack,allTable); // Pop and push the function table
	}
	;

a:
	/* empty */
	| t ID k {
		/// TO DO MAKE A FUNCTION WITH THAT CODE
		int globalTable = pile_valeur(TSStack);
		contador = 1;
		char* paramNum = concatStringInt("tipoparam", contador);
		crear_atributo_cadena(globalTable, currentID, paramNum, $<p.lexema>1);
	}
	;

k:
	/* empty */
	| COMA t ID k {
		/// TO DO MAKE A FUNCTION WITH THAT CODE
		int globalTable = pile_valeur(TSStack);
		contador++;
		char* paramNum = concatStringInt("tipoparam", contador);
		crear_atributo_cadena(globalTable, currentID, paramNum, $<p.lexema>2);
	}
	;

s:
	RETURN e { $<p.tipo>$ = $<p.tipo>2;}
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
	| b c { $<p.tipo>$ = $<p.tipo>2;}
	;

h:
	/* empty */ { $<p.tipo>$ = "empty";}
	| t { $<p.tipo>$ = $<p.lexema>1;}
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
	r e1 {
		if(!strcmp($<p.tipo>2,"empty")){
			$<p.tipo>$ = $<p.tipo>1;
		}else{
			if(!strcmp($<p.tipo>1,$<p.tipo>2) && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;
	
e1:
	/* empty */ { $<p.tipo>$ = "empty";}
	| OPLOGCON r e1 { 
		if(!strcmp($<p.tipo>3,$<p.tipo>2) && !strcmp($<p.tipo>3,"bool")){
			$<p.tipo>$ = "bool";
		}else{
			$<p.tipo>$ = "error";
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): tipos debe ser bool %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>3, BLACK_TERM);
		}
	}
	;
	
r:	
	u r1 {
		if(!strcmp($<p.tipo>2,"empty")){
			$<p.tipo>$ = $<p.tipo>1;
		}else{
			if(!strcmp($<p.tipo>1,$<p.tipo>2) && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;

r1:
	/* empty */ { $<p.tipo>$ = "empty";}
	| OPRELIGUAL u r1 { 
		fprintf(stderr," %s r %s\n",RED_TERM, BLACK_TERM);
		if(!strcmp($<p.tipo>3,$<p.tipo>2)){
			if(!strcmp($<p.tipo>3,"int")){
				$<p.tipo>$ = "int";
			}else if(!strcmp($<p.tipo>3,"bool")){
				$<p.tipo>$ = "bool";
			}else{
				$<p.tipo>$ = "error";
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): exprecion == solo acepta enteros o boolean %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>3, BLACK_TERM);
			}
		}else{
			$<p.tipo>$ = "error";
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): no mismo tipos %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>3, BLACK_TERM);
		}
	}
	;

u:
	v u1 {
		if(!strcmp($<p.tipo>2,"empty")){
			$<p.tipo>$ = $<p.tipo>1;
		}else{
			if(!strcmp($<p.tipo>1,$<p.tipo>2) && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;

u1:
	/* empty */ { $<p.tipo>$ = "empty";}
	| OPARSUMA v u1 { 
		if(!strcmp($<p.tipo>3,$<p.tipo>2) && !strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = "int";
		}else{
			$<p.tipo>$ = "error";
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): exprecion suma solo acepta enteros %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>3, BLACK_TERM);
		}
	}
	;
	
v:
	ID v1 {/*$<p.tipo>$ = serach_tipo_in_tabla */}
	| ABRPAR e CERPAR
	| ENTERO { $<p.tipo>$ = "int"; $<p.valueInt>$ = $<p.valueInt>1;}
	| CADENA { $<p.tipo>$ = "chars"; $<p.valueChar>$ = $<p.valueChar>1;}
	| OPINCR ID
	| LOGICO { $<p.tipo>$ = "bool"; $<p.valueChar>$ = $<p.valueChar>1;}
	;

v1:
	/* empty */
	| ABRPAR l CERPAR
	;

%%