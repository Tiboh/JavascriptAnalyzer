%{
	#include <stdio.h>
	#include "global.h"
	#include <stdlib.h>
	
	extern int yylineno;
	extern FILE *errorFile;
	
	char* currentID;
	int nbParam;
	char* params[50];

	void yyerror (char const *s) {
	   fprintf (errorFile,  " %sERROR SINTACTICO: (Line:%d) %s%s\n" , RED_TERM, yylineno, s, BLACK_TERM);
	}
%}

%union{

struct{
	char* lexema;
	int valueInt;
	char* valueChar;
	char* valueBool;
	char* tipo;
	char* tipovuelta;
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
	/* empty */ {writeParser(3);}
	| {writeParser(1);} b p {$<p>$ = $<p>2;}
	| {writeParser(2);} f p {$<p>$ = $<p>2;}
	;

b:
	{writeParser(4);$<p.tipovuelta>$ = "empty";} VAR t ID {
		if (existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4) == -1) 
		{
			int currentTable = pile_valeur(TSStack);
			crear_entrada(currentTable,$<p.lexema>4);
			asignar_tipo_entrada(currentTable, $<p.lexema>4, "variable");
			crear_atributo_cadena(currentTable, $<p.lexema>4, "tipo", $<p.lexema>3);
		}
		else
		{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' ya existe %s\n",RED_TERM, yylineno, $<p.lexema>4,BLACK_TERM);
		}
	}
	| {writeParser(5);$<p.tipovuelta>$ = "empty";} IF ABRPAR e {
		if(strcmp($<p.tipo>4,"int") && strcmp($<p.tipo>4,"bool")){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Condicion del IF solo acepta tipo entero o booleano %s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	} CERPAR b1
	| {writeParser(6);$<p.tipovuelta>$ = "empty";} SWITCH ABRPAR ID {
		int tableID = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4);
		if (tableID == -1) 
		{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>4,BLACK_TERM);
			$<p.tipo>$ = "error";
		}else{

		}
	} CERPAR ABRLLAVE g 
	{
		int currentTableID = pile_valeur(TSStack);
		if(strcmp($<p.tipo>8, "error")){
			if(existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4) != -1){
				char* idTipo = (char*) consultar_valor_atributo_cadena(currentTableID,$<p.lexema>4,"tipo");
				if(strcmp(idTipo,$<p.tipo>8)){
					fprintf(errorFile," %sERROR SINTACTICO (Line:%d): El SWITCH y el CASE no tienen mismo tipo %s\n",RED_TERM, yylineno, BLACK_TERM);
				}
			}
		}
	}
	CERLLAVE
	| {writeParser(7);} s {$<p>$ = $<p>2;}
	;
	
b1:
	{writeParser(8);} b {$<p>$ = $<p>2;}
	| {writeParser(9);} ABRLLAVE c CERLLAVE
	;
		
t:
	{writeParser(11);} INT {$<p>$ = $<p>2;}
	| {writeParser(12);} BOOL {$<p>$ = $<p>2;}
	| {writeParser(13);} CHARS {$<p>$ = $<p>2;}
	;

f:
	{writeParser(10);} FUNCTION h ID {
		if (existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4) == -1) 
		{
			currentID = $<p.lexema>4;
			int globalTable = pile_valeur(TSStack);
			crear_entrada(globalTable,$<p.lexema>4);
			asignar_tipo_entrada(globalTable, $<p.lexema>4, "funcion");
			crear_atributo_cadena(globalTable, $<p.lexema>4, "tipo", $<p.tipo>3);
		} else {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' ya existe %s\n",RED_TERM, yylineno, $<p.lexema>4,BLACK_TERM);
		}
	}
	ABRPAR a CERPAR {
		int functionTable = crear_tabla();
		int globalTable = pile_valeur(TSStack);
		crear_atributo_entero(globalTable, $<p.lexema>4, "idtabla", functionTable);
		pile_empile(TSStack, functionTable);
		
		int parametros = consultar_valor_atributo_entero(globalTable,$<p.lexema>4,"parametros");
		if(parametros!=0){
			int i;
			for(i = 1 ; i <= parametros ; i++){
				char* currentParameterType = concatStringInt("tipoparam", i);
				char* currentParameterLex = concatStringInt("lexemaparam", i);
				char* lexCurrent = (char*) consultar_valor_atributo_cadena(globalTable,$<p.lexema>4,currentParameterLex);
				char* typeCurrent = (char*) consultar_valor_atributo_cadena(globalTable,$<p.lexema>4,currentParameterType);
				crear_entrada(functionTable,lexCurrent);
				asignar_tipo_entrada(functionTable, lexCurrent, "parametro");
				crear_atributo_cadena(functionTable, lexCurrent, "tipo", typeCurrent);
			}
		}else{
			crear_atributo_entero(globalTable, $<p.lexema>4, "parametros", parametros);
		}
	}
	ABRLLAVE c {
		if(!strcmp($<p.tipo>3,"empty") && strcmp("empty", $<p.tipovuelta>11)){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): La funcion %s no necesite un RETURN %s\n",RED_TERM, yylineno, $<p.lexema>4, BLACK_TERM);
		}else if(strcmp($<p.tipo>3,"empty") && !strcmp("empty", $<p.tipovuelta>11)){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): La funcion %s necesite un RETURN de tipo %s %s\n",RED_TERM, yylineno, $<p.lexema>4, $<p.tipo>3, BLACK_TERM);
		}else if(strcmp($<p.tipo>3, $<p.tipovuelta>11)){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): El RETURN no corresponde al tipo de la funcion %s %s\n",RED_TERM, yylineno, $<p.lexema>4, BLACK_TERM);
		}
	} 
	CERLLAVE {
		popAndPushToStacks(TSStack,allTable); // Pop and push the function table
	}
	;

a:
	/* empty */ { writeParser(15); $<p.tipo>$ = "empty";}
	| {writeParser(14);} t ID {
		{$<p>$ = $<p>2;}
		int currentTable = pile_valeur(TSStack);
		int parametros = 1;
		char* tipoParamNum = concatStringInt("tipoparam", parametros);
		char* lexemaParamNum = concatStringInt("lexemaparam", parametros);
		crear_atributo_cadena(currentTable, currentID, tipoParamNum, $<p.lexema>2);
		crear_atributo_cadena(currentTable, currentID, lexemaParamNum, $<p.lexema>3);
		crear_atributo_entero(currentTable, currentID, "parametros", parametros);
	} k
	;

k:
	/* empty */ { writeParser(17); $<p.tipo>$ = "empty";}
	| {writeParser(16);} COMA t ID {
		int currentTable = pile_valeur(TSStack);
		int parametros = consultar_valor_atributo_entero(currentTable,currentID,"parametros");
		parametros++;
		char* tipoParamNum = concatStringInt("tipoparam", parametros);
		char* lexemaParamNum = concatStringInt("lexemaparam", parametros);
		crear_atributo_cadena(currentTable, currentID, tipoParamNum, $<p.lexema>3);
		crear_atributo_cadena(currentTable, currentID, lexemaParamNum, $<p.lexema>4);
		asignar_valor_atributo_entero(currentTable,currentID,"parametros", parametros);
	} k
	;

s:
	{writeParser(19);} RETURN e {$<p.tipovuelta>$ = $<p.tipo>3;}
	| {writeParser(20); $<p.tipovuelta>$ = "empty";} WRITE ABRPAR e CERPAR {
		if (!strcmp($<p.tipo>4,"int") || !strcmp($<p.tipo>4,"chars")){
		} else{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Funcion WRITE solo acepta tipo entero y cadena %s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	}
	| {writeParser(21);$<p.tipovuelta>$ = "empty";} PROMPT ABRPAR ID CERPAR {
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4);
		if(numTable == -1) {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>2,BLACK_TERM);
		}else{
			char* tipoID = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>4,"tipo");
			if (!strcmp(tipoID,"int") || !strcmp(tipoID,"chars")){
			}else{
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Funcion PROMPT solo acepta tipo entero y cadena %s\n",RED_TERM, yylineno,BLACK_TERM);
			}
		}
	}
	| {writeParser(18);$<p.tipovuelta>$ = "empty";} ID {
		{$<p>$ = $<p>2;}
		currentID = $<p.lexema>2;
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>2);
		if(numTable == -1) {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>2,BLACK_TERM);
		}
		nbParam = 0;
	}
	s2 {	
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>2);
		if(!strcmp($<p.tipo>4, "int") || !strcmp($<p.tipo>4, "bool") || !strcmp($<p.tipo>4, "chars")){ // if it's an assignation
		char* tipoID = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>2,"tipo");
			if(!strcmp($<p.tipo>4,tipoID)){
			}else{
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' solo acepta tipo '%s' %s\n",RED_TERM, yylineno, $<p.lexema>2, tipoID, BLACK_TERM);
			}
		} else if(!strcmp($<p.tipo>4, "funcion")){ // it's function
			int parametros = consultar_valor_atributo_entero(numTable,$<p.lexema>2,"parametros");
			if(parametros != nbParam){
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Se falta %d parametros en el llamamiento de la funcion %s %s\n",RED_TERM, yylineno, parametros-nbParam, $<p.lexema>2, BLACK_TERM);
			}
			int i;
			for(i = 0 ; i < nbParam ; i++){
				char * attr = concatStringInt("tipoparam", i+1);
				char * tipoParam = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>2,attr);
				if(strcmp(tipoParam, params[i])){
					fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Parametro %d en el llamamiento de la funcion %s debe ser de tipo %s %s\n",RED_TERM, yylineno, i+1, $<p.lexema>2, tipoParam, BLACK_TERM);
				}
			}
		}else{
			
		}
	}
	;

s2:
	{writeParser(22);} OPAS e { 
		if (!strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = $<p.tipo>3;
		}
		else if (!strcmp($<p.tipo>3,"chars")){
			$<p.tipo>$ = $<p.tipo>3;
		}	
		else if (!strcmp($<p.tipo>3,"bool")){
			$<p.tipo>$ = $<p.tipo>3;
		}	
		else{
			$<p.tipo>$ = "error";
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Valor de tipo '%s' no se puede asignar %s\n",RED_TERM, yylineno, $<p.tipo>3, BLACK_TERM);
		}
		
	}
	| {writeParser(23);} ABRPAR l CERPAR {$<p.tipo>$="funcion";}
	| {writeParser(24);} OPASSUMA e{
		if (!strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = $<p.tipo>3;
		}
		else {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variables deben ser de tipo entero %s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	}
	| {writeParser(25);} OPINCR {
		int tableID = existe_entrada_tablas_anteriores(TSStack,currentID);
		if (!strcmp(consultar_valor_atributo_cadena(tableID, currentID, "tipo"),"int")){
			$<p.tipo>$ = "int";
		}
		else {
			$<p.tipo>$ = "error";
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' deben ser de tipo entero %s\n",RED_TERM, yylineno, currentID, BLACK_TERM);
		}	
	}
	;

g:
	{writeParser(26);} CASE e {
		$<p.tipo>$ = $<p.tipo>3;
	}
	DOBLEPUNTOS g1
	{
		if(strcmp($<p.tipo>3,"error") && strcmp($<p.tipo>6,"error"))
		{		
			if(!strcmp($<p.tipo>3,$<p.tipo>6) || !strcmp("default",$<p.tipo>6) || !strcmp("empty",$<p.tipo>6)){
				$<p.tipo>$ = $<p.tipo>3;
			}
			else{
				$<p.tipo>$ = "error";
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Los CASE no tienen todos el mismo tipo %s\n",RED_TERM, yylineno, BLACK_TERM);
			}
		}else if (!strcmp($<p.tipo>6,"empty") && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
		}else if(strcmp($<p.tipo>3,"error")){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Error en la variable del case %s\n",RED_TERM, yylineno, BLACK_TERM);
		}else{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Error en el cuerpo del case %s\n",RED_TERM, yylineno, BLACK_TERM);
		}
	}
	| {writeParser(27);} DEFAULT DOBLEPUNTOS g2 {$<p>$ = $<p>4; $<p.tipo>$ = "default";}
	;
	
g1:
	{writeParser(29);} i {$<p.tipo>$ = $<p.tipo>2;}
	| {writeParser(28);} b j i {$<p.tipo>$ = $<p.tipo>4;}
	;	

g2:
	/* empty */ {writeParser(31);}
	| {writeParser(30);} b g2 {$<p>$ = $<p>2;}
	;		

j:
	/* empty */ {writeParser(33);}
	| {writeParser(32);} BREAK 
	;

i: 
	/* empty */ {writeParser(35); $<p.tipo>$ = "empty";}
	| {writeParser(34);} g {$<p.tipo>$ = $<p.tipo>2;}
	;

c:
	/* empty */ {writeParser(37); $<p.tipovuelta>$ = "empty";}
	| {writeParser(36);} b c {
		if(!strcmp($<p.tipovuelta>3,"empty")){
			$<p.tipovuelta>$ = $<p.tipovuelta>2;
		}else{
			$<p.tipovuelta>$ = $<p.tipovuelta>3;
		}
	}
	;

h:
	/* empty */ {writeParser(39); $<p.tipo>$ = "empty";}
	| {writeParser(38);} t { $<p.tipo>$ = $<p.lexema>2;}
	;

l:
	/* empty */ {writeParser(41);}
	|{writeParser(41);} e {nbParam = 1; params[nbParam-1] = $<p.tipo>2;} q {$<p>$ = $<p>2;}
	;
	
q:
	/* empty */ {writeParser(43);}
	| {writeParser(42);} COMA e {nbParam++; params[nbParam-1] = $<p.tipo>3;} q
	;

e:
	{writeParser(44);} r e1 {
		if(!strcmp($<p.tipo>3,"empty")){
			$<p.tipo>$ = $<p.tipo>2;
		}else{
			if(!strcmp($<p.tipo>2,$<p.tipo>3) && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;
	
e1:
	/* empty */ {writeParser(46); $<p.tipo>$ = "empty";}
	| {writeParser(45);} OPLOGCON r e1 {
		if((!strcmp($<p.tipo>4,$<p.tipo>3) || !strcmp($<p.tipo>4,"empty")) && !strcmp($<p.tipo>3,"bool")){
			$<p.tipo>$ = "bool";
		}else{
			$<p.tipo>$ = "error";
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Solo acepta tipo booleano %s\n",RED_TERM, yylineno, BLACK_TERM);
		}
	}
	;
	
r:	
	{writeParser(47);} u r1 {
		if(!strcmp($<p.tipo>3,"empty")){
			$<p.tipo>$ = $<p.tipo>2;
		}else{
			if(!strcmp($<p.tipo>2,$<p.tipo>3) && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = "bool";
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;

r1:
	/* empty */ {writeParser(49); $<p.tipo>$ = "empty";}
	| {writeParser(48);} OPRELIGUAL u r1 {
		if(!strcmp($<p.tipo>4,$<p.tipo>3) || !strcmp($<p.tipo>4,"empty")){
			if(!strcmp($<p.tipo>3,"int")){
				$<p.tipo>$ = "int";
			}else if(!strcmp($<p.tipo>3,"bool")){
				$<p.tipo>$ = "bool";
			}else{
				$<p.tipo>$ = "error";
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Operator '==' solo acepta tipo entero o booleano %s\n",RED_TERM, yylineno, BLACK_TERM);
			}
		}else{
			$<p.tipo>$ = "error";
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): No mismo tipos %s\n",RED_TERM, yylineno, BLACK_TERM);
		}
	}
	;

u:
	{writeParser(50);} v u1 {
		if(!strcmp($<p.tipo>3,"empty")){
			$<p.tipo>$ = $<p.tipo>2;
		}else{
			if(!strcmp($<p.tipo>2,$<p.tipo>3) && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;

u1:
	/* empty */ {writeParser(52); $<p.tipo>$ = "empty";}
	| {writeParser(51);} OPARSUMA v u1 { 
		if(!strcmp($<p.tipo>4,$<p.tipo>3) && !strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = "int";
		}else if(!strcmp($<p.tipo>4,"empty") && !strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = "int";
		}else{
			$<p.tipo>$ = "error";
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Operator '+' solo acepta tipo entero %s\n",RED_TERM, yylineno, BLACK_TERM);
		}
	}
	;
	
v:
	{writeParser(53);} ID v1 {
		int tableID = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>2);
		if (tableID == -1)
		{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>2,BLACK_TERM);
			$<p.tipo>$ = "error";
			// exit(-1);
		}else{
			char* tipo = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>2,"tipo");
			$<p.tipo>$ = tipo;
			if(!strcmp($<p.tipo>3,"incr")){
				if (!strcmp(consultar_valor_atributo_cadena(tableID, $<p.lexema>2, "tipo"),"int")){
						$<p.tipo>$ = "int"; 
				} else {
					fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' deben ser de tipo entero %s\n",RED_TERM, yylineno, $<p.lexema>2, BLACK_TERM);
				}	
			}else{ // Function

			}
		}
	}
	| {writeParser(54);} ABRPAR e CERPAR {
		$<p.tipo>$ = $<p.tipo>3;
	}
	| {writeParser(55);} ENTERO { $<p.tipo>$ = "int"; }
	| {writeParser(56);} CADENA { $<p.tipo>$ = "chars";}
	| {writeParser(57);} LOGICO { $<p.tipo>$ = "bool";}
	;

v1:
	/* empty */ {writeParser(58); $<p.tipo>$ = "empty";}
	| {writeParser(59);} ABRPAR l CERPAR {$<p.tipo>$ = "param"; $<p>$ = $<p>3;}
	| {writeParser(60);} OPINCR { $<p.tipo>$ = "incr";}
	;

%%

