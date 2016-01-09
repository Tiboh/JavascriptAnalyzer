%{
	#include <stdio.h>
	#include "global.h"
	#include <stdlib.h>
	
	extern int yylineno;
	
	char* currentID;

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
		if (existe_entrada_tablas_anteriores(TSStack,$<p.lexema>3) == -1) 
		{
			int currentTable = pile_valeur(TSStack);
			crear_entrada(currentTable,$<p.lexema>3);
			asignar_tipo_entrada(currentTable, $<p.lexema>3, "variable");
			crear_atributo_cadena(currentTable, $<p.lexema>3, "tipo", $<p.lexema>2);
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
		if (existe_entrada_tablas_anteriores(TSStack,$<p.lexema>3) == -1) 
		{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator %s no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>3,BLACK_TERM);
			exit(-1);
		}
	} CERPAR ABRLLAVE g 
	{
		int currentTableID = pile_valeur(TSStack);
		char* idTipo = (char*) consultar_valor_atributo_cadena(currentTableID,$<p.lexema>3,"tipo");
		if(!strcmp(idTipo,$<p.tipo>7) && strcmp($<p.tipo>7,"error")){
		}else{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): the variable of the switch should be compared to variables of the same type only %s/%s%s\n",RED_TERM, yylineno, idTipo, $<p.tipo>7, BLACK_TERM);
		}
	}
	CERLLAVE
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
	FUNCTION h ID { 
		currentID = $<p.lexema>3; 
		int globalTable = pile_valeur(TSStack);
		crear_entrada(globalTable,$<p.lexema>3);
		asignar_tipo_entrada(globalTable, $<p.lexema>3, "funcion");
		crear_atributo_cadena(globalTable, $<p.lexema>3, "tipo", $<p.tipo>2);
	}
	ABRPAR a CERPAR {
		int functionTable = crear_tabla();
		int globalTable = pile_valeur(TSStack);
		crear_atributo_entero(globalTable, $<p.lexema>3, "idtabla", functionTable);
		pile_empile(TSStack, functionTable);
		
		int parametros = consultar_valor_atributo_entero(globalTable,$<p.lexema>3,"parametros");
		if(parametros!=0){
			int i;
			for(i = 1 ; i <= parametros ; i++){
				char* currentParameterType = concatStringInt("tipoparam", i);
				char* currentParameterLex = concatStringInt("lexemaparam", i);
				char* lexCurrent = (char*) consultar_valor_atributo_cadena(globalTable,$<p.lexema>3,currentParameterLex);
				char* typeCurrent = (char*) consultar_valor_atributo_cadena(globalTable,$<p.lexema>3,currentParameterType);
				crear_entrada(functionTable,lexCurrent);
				asignar_tipo_entrada(functionTable, lexCurrent, "parametro");
				crear_atributo_cadena(functionTable, lexCurrent, "tipo", typeCurrent);
			}
		}else{
			crear_atributo_entero(globalTable, $<p.lexema>3, "parametros", parametros);
		}
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
	/* empty */ { $<p.tipo>$ = "empty";}
	| t ID {
		int currentTable = pile_valeur(TSStack);
		int parametros = 1;
		char* tipoParamNum = concatStringInt("tipoparam", parametros);
		char* lexemaParamNum = concatStringInt("lexemaparam", parametros);
		crear_atributo_cadena(currentTable, currentID, tipoParamNum, $<p.lexema>1);
		crear_atributo_cadena(currentTable, currentID, lexemaParamNum, $<p.lexema>2);
		crear_atributo_entero(currentTable, currentID, "parametros", parametros);
	} k
	;

k:
	/* empty */ { $<p.tipo>$ = "empty";}
	| COMA t ID {
		int currentTable = pile_valeur(TSStack);
		int parametros = consultar_valor_atributo_entero(currentTable,currentID,"parametros");
		parametros++;
		char* tipoParamNum = concatStringInt("tipoparam", parametros);
		char* lexemaParamNum = concatStringInt("lexemaparam", parametros);
		crear_atributo_cadena(currentTable, currentID, tipoParamNum, $<p.lexema>2);
		crear_atributo_cadena(currentTable, currentID, lexemaParamNum, $<p.lexema>3);
		asignar_valor_atributo_entero(currentTable,currentID,"parametros", parametros);
	} k
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
	| OPINCR
	;

g:
	CASE e {
		
		$<p.tipo>$ = $<p.tipo>2;
	}
		
	DOBLEPUNTOS g1
	{	
		if(strcmp($<p.tipo>2,"error") && strcmp($<p.tipo>5,"empty") && strcmp($<p.tipo>5,"error"))
		{		
			if(!strcmp($<p.tipo>2,$<p.tipo>5)){
				$<p.tipo>$ = $<p.tipo>2;
			}
			else{
				$<p.tipo>$ = "error";
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): the variable of case should be  of the same type %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>5, BLACK_TERM);
			}
		}
		else if (!strcmp($<p.tipo>5,"empty")){
				$<p.tipo>$ = $<p.tipo>2;
		}else{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): definir erreur %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>5, BLACK_TERM);
		}
	}
	| DEFAULT DOBLEPUNTOS g2
	;
	
g1:
	i {$<p.tipo>$ = $<p.tipo>1;}
	| b j i {$<p.tipo>$ = $<p.tipo>3;}
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
	/* empty */ {$<p.tipo>$ = "empty";}
	| g {$<p.tipo>$ = $<p.tipo>1;}
	;

c:
	/* empty */ {$<p.tipo>$ = "empty";}
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
		if((!strcmp($<p.tipo>3,$<p.tipo>2) || !strcmp($<p.tipo>3,"empty")) && !strcmp($<p.tipo>2,"bool")){
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
		if(!strcmp($<p.tipo>3,$<p.tipo>2) || !strcmp($<p.tipo>3,"empty")){
			if(!strcmp($<p.tipo>2,"int")){
				$<p.tipo>$ = "int";
			}else if(!strcmp($<p.tipo>2,"bool")){
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
		if((!strcmp($<p.tipo>3,$<p.tipo>2)  || !strcmp($<p.tipo>3,"empty")) && !strcmp($<p.tipo>2,"int")){
			$<p.tipo>$ = "int";
		}else{
			$<p.tipo>$ = "error";
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): exprecion suma solo acepta enteros %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>3, BLACK_TERM);
		}
	}
	;
	
v:
	ID v1 {
		if (existe_entrada_tablas_anteriores(TSStack,$<p.lexema>1) == -1) 
		{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator %s no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>1,BLACK_TERM);
			exit(-1);
		}else{
			int currentTableID = pile_valeur(TSStack);
			$<p.tipo>$ = (char*) consultar_valor_atributo_cadena(currentTableID,$<p.lexema>1,"tipo");
		}
	}
	| ABRPAR e CERPAR
	| ENTERO { $<p.tipo>$ = "int"; $<p.valueInt>$ = $<p.valueInt>1;}
	| CADENA { $<p.tipo>$ = "chars"; $<p.valueChar>$ = $<p.valueChar>1;}
	| LOGICO { $<p.tipo>$ = "bool"; $<p.valueChar>$ = $<p.valueChar>1;}
	;

v1:
	/* empty */ { $<p.tipo>$ = "empty";}
	| ABRPAR l CERPAR
	| OPINCR
	;

%%