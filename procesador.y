%{
	#include <stdio.h>
	#include "global.h"
	#include <stdlib.h>
	
	extern int yylineno;
	extern FILE *errorFile;
	
	char* currentID;

	void yyerror (char const *s) {
	   fprintf (errorFile,  " %sERROR SINTÁCTICO: (Line:%d) %s%s\n" , RED_TERM, yylineno, s, BLACK_TERM);
	}
%}

%union{

struct{
	char* lexema;
	int valueInt;
	char* valueChar;
	char* valueBool;
	char* tipo;
	char* vuelta;
	int assign; /* 1 if assignation, 0 if else  */
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
	{writeParser(4);} VAR t ID {
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
	| {writeParser(5);} IF ABRPAR e { 
		if(strcmp($<p.tipo>4,"int") && strcmp($<p.tipo>4,"bool")){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Condicion del IF solo acepta tipo entero o booleano %s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	} CERPAR b1
	| {writeParser(6);} SWITCH ABRPAR ID {
		int tableID = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4);
		if (tableID == -1) 
		{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>4,BLACK_TERM);
			exit(-1);
		}else{
			if (existe_atributo(tableID,$<p.lexema>4,"value") != 0){
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' no inicializada %s\n",RED_TERM, yylineno, $<p.lexema>4, BLACK_TERM);
			}
		}
	} CERPAR ABRLLAVE g 
	{
		int currentTableID = pile_valeur(TSStack);
		char* idTipo = (char*) consultar_valor_atributo_cadena(currentTableID,$<p.lexema>4,"tipo");
		if(!strcmp(idTipo,$<p.tipo>8) && strcmp($<p.tipo>8,"error")){
		}else{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): El SWITCH y el CASE no tienen mismo tipo %s\n",RED_TERM, yylineno, BLACK_TERM);
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
		currentID = $<p.lexema>4; 
		int globalTable = pile_valeur(TSStack);
		crear_entrada(globalTable,$<p.lexema>4);
		asignar_tipo_entrada(globalTable, $<p.lexema>4, "funcion");
		crear_atributo_cadena(globalTable, $<p.lexema>4, "tipo", $<p.tipo>3);
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
	{writeParser(19);} RETURN e { $<p.tipo>$ = $<p.tipo>3;}
	| {writeParser(20);} WRITE ABRPAR e CERPAR {
		if (!strcmp($<p.tipo>4,"int")){
			fprintf(stdout," %s%d%s\n", GREEN_TERM, $<p.valueInt>4 ,BLACK_TERM);
		} else if (!strcmp($<p.tipo>4,"chars")){
			fprintf(stdout," %s%s%s\n", GREEN_TERM, $<p.valueChar>4,BLACK_TERM);
		} else{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Funcion WRITE solo acepta tipo entero y cadena %s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	}
	| {writeParser(21);} PROMPT ABRPAR ID CERPAR {
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>4);
		if(numTable == -1) {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>2,BLACK_TERM);
		}else{
			char* tipoID = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>4,"tipo");
			if (!strcmp(tipoID,"int")){
				int myvariable;
				printf(" Escribir valor de '%s' (entero): ",$<p.lexema>4);
				scanf("%d", &myvariable);
				if (existe_atributo(numTable,$<p.lexema>4,"value") == 0){ // exists
						asignar_valor_atributo_entero(numTable,$<p.lexema>4,"value",myvariable);
				}
				else {
					crear_atributo_entero(numTable,$<p.lexema>4,"value",myvariable);
				}
			}
			else if (!strcmp(tipoID,"chars")){
				char string[256];
				printf(" Escribir valor de '%s' (cadena): ",$<p.lexema>4);
				scanf("%s", &string);
				if (existe_atributo(numTable,$<p.lexema>4,"value") == 0){ // exists
						asignar_valor_atributo_cadena(numTable,$<p.lexema>4,"value",string);
				}
				else {
					crear_atributo_cadena(numTable,$<p.lexema>4,"value",string);
				}			
			}else{
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Funcion PROMPT solo acepta tipo entero y cadena %s\n",RED_TERM, yylineno,BLACK_TERM);
			}
		}
	}
	| {writeParser(18);} ID {
		{$<p>$ = $<p>2;}
		currentID = $<p.lexema>2;
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>2);
		if(numTable == -1) {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Identificador '%s' no declarado %s\n",RED_TERM, yylineno, $<p.lexema>2,BLACK_TERM);
			exit(-1);	
		}
	}
	s2 {
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>2);
		if($<p.assign>4){ // if it's an assignation
		char* tipoID = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>2,"tipo");
			if(!strcmp($<p.tipo>4,tipoID)){
				if (!strcmp($<p.tipo>4,"int")){
					if (existe_atributo(numTable,$<p.lexema>2,"value") == 0){ // exists
						asignar_valor_atributo_entero(numTable,$<p.lexema>2,"value",$<p.valueInt>4);
					}
					else {
						crear_atributo_entero(numTable,$<p.lexema>2,"value",$<p.valueInt>4);
					}
				}
				else if (!strcmp($<p.tipo>4,"chars")){
					if (existe_atributo(numTable,$<p.lexema>2,"value") == 0){ // exists
						asignar_valor_atributo_cadena(numTable,$<p.lexema>2,"value",$<p.valueChar>4);
					}
					else {
						crear_atributo_cadena(numTable,$<p.lexema>2,"value",$<p.valueChar>4);
					}
				}
				else if (!strcmp($<p.tipo>4,"bool")){
					if (existe_atributo(numTable,$<p.lexema>2,"value") == 0){ // exists
						asignar_valor_atributo_cadena(numTable,$<p.lexema>2,"value",$<p.valueBool>4);
					}
					else {
						crear_atributo_cadena(numTable,$<p.lexema>2,"value",$<p.valueBool>4);
					}
				}
			}else{
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' solo acepta tipo '%s' %s\n",RED_TERM, yylineno, $<p.lexema>2, tipoID, BLACK_TERM);
			}
		}
		else { //if not assignation, it's function
		
		}
	}

	;

s2:
	{writeParser(22);} OPAS e { 
		$<p.assign>$ = 1;
		if (!strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = $<p.tipo>3;
			$<p.valueInt>$ = $<p.valueInt>3;
		}
		else if (!strcmp($<p.tipo>3,"chars")){
			$<p.tipo>$ = $<p.tipo>3;
			$<p.valueChar>$ = $<p.valueChar>3;
		}	
		else if (!strcmp($<p.tipo>3,"bool")){
			$<p.tipo>$ = $<p.tipo>3;
			$<p.valueBool>$ = $<p.valueBool>3;
		}	
		else{
			$<p.tipo>$ = "error";
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Valor de tipo '%s' no se puede asignar %s\n",RED_TERM, yylineno, $<p.tipo>3, BLACK_TERM);
		}
		
	}
	| {writeParser(23);} ABRPAR {
		$<p.assign>$ = 0;
		$<p.tipo>$="funcion";
	}
	l CERPAR 
	| {writeParser(24);} OPASSUMA e{
		$<p.assign>$ = 1;
		if (!strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = $<p.tipo>3;
			$<p.valueInt>$ += $<p.valueInt>3;
		}
		else {
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variables deben ser de tipo entero %s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	}
	| {writeParser(25);} OPINCR {
		$<p.assign>$ = 1;
		int tableID = existe_entrada_tablas_anteriores(TSStack,currentID);
		if (!strcmp(consultar_valor_atributo_cadena(tableID, currentID, "tipo"),"int")){
			if (existe_atributo(tableID,currentID,"value") == 0){ // exists
				int idValue = consultar_valor_atributo_entero(tableID, currentID, "value");
				$<p.valueInt>$ = idValue++;
				$<p.tipo>$ = "int";
			}
			else {
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' no inicializada %s\n",RED_TERM, yylineno, currentID, BLACK_TERM);
			}
		}
		else {
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
			if(!strcmp($<p.tipo>3,$<p.tipo>6)){
				$<p.tipo>$ = $<p.tipo>3;
			}
			else{
				$<p.tipo>$ = "error";
				fprintf(errorFile," %sERROR SINTACTICO (Line:%d): El CASE y su cuerpo no tienen mismo tipo %s\n",RED_TERM, yylineno, BLACK_TERM);
			}
		}else if (!strcmp($<p.tipo>6,"empty") && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
		}else if(strcmp($<p.tipo>3,"error")){
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Error en la variable del case %s\n",RED_TERM, yylineno, BLACK_TERM);
		}else{
			fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Error en el cuerpo del case %s\n",RED_TERM, yylineno, BLACK_TERM);
		}
	}
	| {writeParser(27);} DEFAULT DOBLEPUNTOS g2
	;
	
g1:
	{writeParser(29);} i {$<p.tipo>$ = $<p.tipo>2;}
	| {writeParser(28);} b j i {$<p.tipo>$ = $<p.tipo>4;}
	;	

g2:
	/* empty */ {writeParser(31);}
	| {writeParser(30);} b {$<p>$ = $<p>2;}
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
	/* empty */ {writeParser(37); $<p.tipo>$ = "empty";}
	| {writeParser(36);} b c { $<p.tipo>$ = $<p.tipo>3;}
	;

h:
	/* empty */ {writeParser(39); $<p.tipo>$ = "empty";}
	| {writeParser(38);} t { $<p.tipo>$ = $<p.lexema>2;}
	;

l:
	/* empty */ {writeParser(41);}
	|{writeParser(41);} e q {$<p>$ = $<p>2;}
	;

q:
	/* empty */ {writeParser(43);}
	| {writeParser(42);} COMA e q 
	;

e:
	{writeParser(44);} r e1 {
		if(!strcmp($<p.tipo>3,"empty")){
			$<p.tipo>$ = $<p.tipo>2;
			if(strcmp($<p.tipo>2,"error")){
				if(!strcmp($<p.tipo>2,"int")) $<p.valueInt>$ = $<p.valueInt>2;
				else if(!strcmp($<p.tipo>2,"chars")) $<p.valueChar>$ = $<p.valueChar>2;
				else if(!strcmp($<p.tipo>2,"bool")) $<p.valueBool>$ = $<p.valueBool>2;
			}
		}else{
			if(!strcmp($<p.tipo>2,$<p.tipo>3) && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
				if(!strcmp($<p.valueBool>2,"true") && !strcmp("true",$<p.valueBool>3)) $<p.valueBool>$ = "true";
				else $<p.valueBool>$ = "false";
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
			if(!strcmp($<p.tipo>4,"empty")){
					$<p.valueBool>$ = $<p.valueBool>3;
			}else{
				if(!strcmp($<p.valueBool>3,"true") && !strcmp("true",$<p.valueBool>4)) $<p.valueBool>$ = "true";
				else $<p.valueBool>$ = "false";
			}
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
			if(strcmp($<p.tipo>2,"error")){
				if(!strcmp($<p.tipo>2,"int")) $<p.valueInt>$ = $<p.valueInt>2;
				else if(!strcmp($<p.tipo>2,"chars")) $<p.valueChar>$ = $<p.valueChar>2;
				else if(!strcmp($<p.tipo>2,"bool")) $<p.valueBool>$ = $<p.valueBool>2;
			}
		}else{
			if(!strcmp($<p.tipo>2,$<p.tipo>3) && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
				if(!strcmp($<p.valueBool>2,$<p.valueBool>3)) $<p.valueBool>$ = "true";
				else $<p.valueBool>$ = "false";
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
				$<p.tipo>$ = "bool";
				if(!strcmp($<p.tipo>4,"empty")){
					if($<p.valueInt>3 != 0) $<p.valueBool>$ = "true";
					else $<p.valueBool>$ = "false";
				}else{
					char* myR1;
					if($<p.valueInt>3 != 0) myR1 = "true";
					else myR1 = "false";
					if(!strcmp(myR1,$<p.valueBool>4)) $<p.valueBool>$ = "true";
					else $<p.valueBool>$ = "false";
				}
			}else if(!strcmp($<p.tipo>3,"bool")){
				$<p.tipo>$ = "bool";
				if(!strcmp($<p.tipo>4,"empty")){
					$<p.valueBool>$ = $<p.valueBool>3;
				}else{
					if(!strcmp($<p.valueBool>3,$<p.valueBool>4)) $<p.valueBool>$ = "true";
					else $<p.valueBool>$ = "false";
				}
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
			if(strcmp($<p.tipo>2,"error")){
				if(!strcmp($<p.tipo>2,"int")) $<p.valueInt>$ = $<p.valueInt>2;
				else if(!strcmp($<p.tipo>2,"chars")) $<p.valueChar>$ = $<p.valueChar>2;
				else if(!strcmp($<p.tipo>2,"bool")) $<p.valueBool>$ = $<p.valueBool>2;
			}
		}else{
			if(!strcmp($<p.tipo>2,$<p.tipo>3) && strcmp($<p.tipo>3,"error")){
				$<p.tipo>$ = $<p.tipo>3;
				$<p.valueInt>$ = $<p.valueInt>2 + $<p.valueInt>3;
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
			$<p.valueInt>$ = $<p.valueInt>3 + $<p.valueInt>4;
		}else if(!strcmp($<p.tipo>4,"empty") && !strcmp($<p.tipo>3,"int")){
			$<p.tipo>$ = "int";
			$<p.valueInt>$ = $<p.valueInt>3;
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
			exit(-1);
		}else{
			char* tipo = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>2,"tipo");
			$<p.tipo>$ = tipo;
			if(!strcmp($<p.tipo>3, "empty")){
				if (existe_atributo(tableID,$<p.lexema>2,"value") == 0){ // exists
					if (!strcmp(tipo,"int")){
						$<p.valueInt>$ = consultar_valor_atributo_entero(tableID,$<p.lexema>2,"value");
					}
					else if (!strcmp(tipo,"chars")){
						$<p.valueChar>$ = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>2,"value");
					}	
					else if (!strcmp(tipo,"bool")){
						$<p.valueBool>$ = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>2,"value");
					}else{
					}
				}
			}else{ // Function ou incrément
				if(!strcmp($<p.tipo>3,"incr")){
					if (!strcmp(consultar_valor_atributo_cadena(tableID, $<p.lexema>2, "tipo"),"int")){
						if (existe_atributo(tableID,$<p.lexema>2,"value") == 0){ // exists
							int idValue = consultar_valor_atributo_entero(tableID, $<p.lexema>2, "value");
							idValue++;
							asignar_valor_atributo_entero(tableID, $<p.lexema>2,"value", idValue);
							$<p.tipo>$ = "int"; 
							$<p.valueInt>$ = idValue;
						}
						else {
							fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' no inicializada %s\n",RED_TERM, yylineno, $<p.lexema>2, BLACK_TERM);
						}
					}
					else {
						fprintf(errorFile," %sERROR SINTACTICO (Line:%d): Variable '%s' deben ser de tipo entero %s\n",RED_TERM, yylineno, $<p.lexema>2, BLACK_TERM);
					}	
				}else{
					if (!strcmp(tipo,"int")){
						$<p.valueInt>$ = $<p.valueInt>3;
					}
					else if (!strcmp(tipo,"chars")){
						$<p.valueChar>$ = $<p.valueChar>3;
					}	
					else if (!strcmp(tipo,"bool")){
						$<p.valueBool>$ = $<p.valueBool>3;
					}	
				}
			}
		}
	}
	| {writeParser(54);} ABRPAR e CERPAR {
		$<p.tipo>$ = $<p.tipo>3;
		if(strcmp($<p.tipo>2,"error")){
			if(!strcmp($<p.tipo>3,"int")) $<p.valueInt>$ = $<p.valueInt>3;
			else if(!strcmp($<p.tipo>3,"chars")) $<p.valueChar>$ = $<p.valueChar>3;
			else if(!strcmp($<p.tipo>3,"bool")) $<p.valueBool>$ = $<p.valueBool>3;
		} 
	}
	| {writeParser(55);} ENTERO { $<p.tipo>$ = "int"; $<p.valueInt>$ = $<p.valueInt>2;}
	| {writeParser(56);} CADENA { $<p.tipo>$ = "chars"; $<p.valueChar>$ = $<p.valueChar>2;}
	| {writeParser(57);} LOGICO { $<p.tipo>$ = "bool"; $<p.valueBool>$ = $<p.valueBool>2;}
	;

v1:
	/* empty */ {writeParser(58); $<p.tipo>$ = "empty";}
	| {writeParser(59);} ABRPAR l CERPAR { $<p.tipo>$ = "param";}
	| {writeParser(60);} OPINCR { $<p.tipo>$ = "incr";}
	;

%%

