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
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator '%s' ya existe%s\n",RED_TERM, yylineno, $<p.lexema>3,BLACK_TERM);
		}
	}
	| IF ABRPAR e { 
		if(strcmp($<p.tipo>3,"int") && strcmp($<p.tipo>3,"bool")){
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): condicion del IF debe ser de tipo int o bool%s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	} CERPAR b1
	| SWITCH ABRPAR ID {
		int tableID = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>3);
		if (tableID == -1) 
		{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator '%s' no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>3,BLACK_TERM);
			exit(-1);
		}else{
			if (existe_atributo(tableID,$<p.lexema>3,"value") != 0){
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): the variable '%s' hasn't been initialized %s\n",RED_TERM, yylineno, $<p.lexema>3, BLACK_TERM);	
			}
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
	| WRITE ABRPAR e CERPAR {
		if (!strcmp($<p.tipo>3,"int")){
			fprintf(stdout," %s%d%s\n", GREEN_TERM, $<p.valueInt>3 ,BLACK_TERM);
		} else if (!strcmp($<p.tipo>3,"chars")){
			fprintf(stdout," %s%s%s\n", GREEN_TERM, $<p.valueChar>3,BLACK_TERM);
		} else{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): funcion write solo admite tipo int y chars%s\n",RED_TERM, yylineno,BLACK_TERM);
		}
	}
	| PROMPT ABRPAR ID CERPAR {
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>3);
		if(numTable == -1) {
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator '%s' no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>1,BLACK_TERM);
		}else{
			char* tipoID = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>3,"tipo");
			if (!strcmp(tipoID,"int")){
				int myvariable;
				printf(" Escribir valor de '%s' (entero): ",$<p.lexema>3);
				scanf("%d", &myvariable);
				if (existe_atributo(numTable,$<p.lexema>3,"value") == 0){ // exists
						asignar_valor_atributo_entero(numTable,$<p.lexema>3,"value",myvariable);
				}
				else {
					crear_atributo_entero(numTable,$<p.lexema>3,"value",myvariable);
				}
			}
			else if (!strcmp(tipoID,"chars")){
				char string[256];
				printf(" Escribir valor de '%s' (cadena): ",$<p.lexema>3);
				scanf("%s", &string);
				if (existe_atributo(numTable,$<p.lexema>3,"value") == 0){ // exists
						asignar_valor_atributo_cadena(numTable,$<p.lexema>3,"value",string);
				}
				else {
					crear_atributo_cadena(numTable,$<p.lexema>3,"value",string);
				}			
			}else{
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): funcion prompt solo admite tipo int y chars%s\n",RED_TERM, yylineno,BLACK_TERM);
			}
		}
	}
	| ID {
		currentID = $<p.lexema>1;
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>1);
		if(numTable == -1) {
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator '%s' no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>1,BLACK_TERM);
			exit(-1);	
		}
	}
	
	s2 {
		int numTable = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>1);
		if($<p.assign>3){ // if it's an assignation
		char* tipoID = (char*) consultar_valor_atributo_cadena(numTable,$<p.lexema>1,"tipo");
			if(!strcmp($<p.tipo>3,tipoID)){
				if (!strcmp($<p.tipo>3,"int")){
					if (existe_atributo(numTable,$<p.lexema>1,"value") == 0){ // exists
						asignar_valor_atributo_entero(numTable,$<p.lexema>1,"value",$<p.valueInt>3);
					}
					else {
						crear_atributo_entero(numTable,$<p.lexema>1,"value",$<p.valueInt>3);
					}
				}
				else if (!strcmp($<p.tipo>3,"chars")){
					if (existe_atributo(numTable,$<p.lexema>1,"value") == 0){ // exists
						asignar_valor_atributo_cadena(numTable,$<p.lexema>1,"value",$<p.valueChar>3);
					}
					else {
						crear_atributo_cadena(numTable,$<p.lexema>1,"value",$<p.valueChar>3);
					}
				}
				else if (!strcmp($<p.tipo>3,"bool")){
					if (existe_atributo(numTable,$<p.lexema>1,"value") == 0){ // exists
						asignar_valor_atributo_cadena(numTable,$<p.lexema>1,"value",$<p.valueBool>3);
					}
					else {
						crear_atributo_cadena(numTable,$<p.lexema>1,"value",$<p.valueBool>3);
					}
				}
			}else{
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): La variable '%s' solo acepte valor de tipo %s%s\n",RED_TERM, yylineno, $<p.lexema>1, tipoID, BLACK_TERM);
			}
		}
		else { //if not assignation, it's function
		
		}
	}

	;

s2:
	OPAS e { 
		$<p.assign>$ = 1;
		if (!strcmp($<p.tipo>2,"int")){
			$<p.tipo>$ = $<p.tipo>2;
			$<p.valueInt>$ = $<p.valueInt>2;
		}
		else if (!strcmp($<p.tipo>2,"chars")){
			$<p.tipo>$ = $<p.tipo>2;
			$<p.valueChar>$ = $<p.valueChar>2;
		}	
		else if (!strcmp($<p.tipo>2,"bool")){
			$<p.tipo>$ = $<p.tipo>2;
			$<p.valueBool>$ = $<p.valueBool>2;
		}	
		else{
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): Value of type '%s' can't be assign%s\n",RED_TERM, yylineno, $<p.tipo>2, BLACK_TERM);	
		}
		
	}
	| ABRPAR {
		$<p.assign>$ = 0;
		$<p.tipo>$="funcion";
	}
	l CERPAR 
	| OPASSUMA e{
		$<p.assign>$ = 1;
		if (!strcmp($<p.tipo>2,"int")){
			$<p.tipo>$ = $<p.tipo>2;
			$<p.valueInt>$ += $<p.valueInt>2;
		}
		else {
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): Los variables debe ser de tipo entero %s/%s %s\n",RED_TERM, yylineno, $<p.tipo>$, $<p.tipo>2,BLACK_TERM);
		}
	}
	| OPINCR {
		$<p.assign>$ = 1;
		int tableID = existe_entrada_tablas_anteriores(TSStack,currentID);
		if (!strcmp(consultar_valor_atributo_cadena(tableID, currentID, "tipo"),"int")){
			if (existe_atributo(tableID,currentID,"value") == 0){ // exists
				int idValue = consultar_valor_atributo_entero(tableID, currentID, "value");
				$<p.valueInt>$ = idValue++;
				$<p.tipo>$ = "int";
			}
			else {
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): the variable '%s' hasn't been initialized %s\n",RED_TERM, yylineno, currentID, BLACK_TERM);	
			}
		}
		else {
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): la variable '%s' debe ser de tipo int %s\n",RED_TERM, yylineno, currentID, BLACK_TERM);	
		}	
	}
	;

g:
	CASE e {
		$<p.tipo>$ = $<p.tipo>2;
	}
	DOBLEPUNTOS g1
	{	
		if(strcmp($<p.tipo>2,"error") && strcmp($<p.tipo>5,"error"))
		{		
			if(!strcmp($<p.tipo>2,$<p.tipo>5)){
				$<p.tipo>$ = $<p.tipo>2;
			}
			else{
				$<p.tipo>$ = "error";
				fprintf(stderr," %sERROR SINTACTICO (Line:%d): the variable of case should be  of the same type %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>5, BLACK_TERM);
			}
		}else if (!strcmp($<p.tipo>5,"empty") && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
		}else if(strcmp($<p.tipo>2,"error")){
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): Error en la variable del case %s\n",RED_TERM, yylineno, BLACK_TERM);
		}else{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): Error en el cuerpo del case %s\n",RED_TERM, yylineno, BLACK_TERM);
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
			if(strcmp($<p.tipo>1,"error")){
				if(!strcmp($<p.tipo>1,"int")) $<p.valueInt>$ = $<p.valueInt>1;
				else if(!strcmp($<p.tipo>1,"chars")) $<p.valueChar>$ = $<p.valueChar>1;
				else if(!strcmp($<p.tipo>1,"bool")) $<p.valueBool>$ = $<p.valueBool>1;
			}
		}else{
			if(!strcmp($<p.tipo>1,$<p.tipo>2) && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
				if(!strcmp($<p.valueBool>1,"true") && !strcmp("true",$<p.valueBool>2)) $<p.valueBool>$ = "true";
				else $<p.valueBool>$ = "false";
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
			if(!strcmp($<p.tipo>3,"empty")){
					$<p.valueBool>$ = $<p.valueBool>2;
			}else{
				if(!strcmp($<p.valueBool>2,"true") && !strcmp("true",$<p.valueBool>3)) $<p.valueBool>$ = "true";
				else $<p.valueBool>$ = "false";
			}
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
			if(strcmp($<p.tipo>1,"error")){
				if(!strcmp($<p.tipo>1,"int")) $<p.valueInt>$ = $<p.valueInt>1;
				else if(!strcmp($<p.tipo>1,"chars")) $<p.valueChar>$ = $<p.valueChar>1;
				else if(!strcmp($<p.tipo>1,"bool")) $<p.valueBool>$ = $<p.valueBool>1;
			}
		}else{
			printf("tipo u %s, tipo r1 %s \n",$<p.tipo>1,$<p.tipo>2 );
			if(!strcmp($<p.tipo>1,$<p.tipo>2) && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
				if(!strcmp($<p.valueBool>1,$<p.valueBool>2)) $<p.valueBool>$ = "true";
				else $<p.valueBool>$ = "false";
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
				$<p.tipo>$ = "bool";
				if(!strcmp($<p.tipo>3,"empty")){
					if($<p.valueInt>2 != 0) $<p.valueBool>$ = "true";
					else $<p.valueBool>$ = "false";
				}else{
					char* myR1;
					if($<p.valueInt>2 != 0) myR1 = "true";
					else myR1 = "false";
					if(!strcmp(myR1,$<p.valueBool>3)) $<p.valueBool>$ = "true";
					else $<p.valueBool>$ = "false";
				}
			}else if(!strcmp($<p.tipo>2,"bool")){
				$<p.tipo>$ = "bool";
				if(!strcmp($<p.tipo>3,"empty")){
					$<p.valueBool>$ = $<p.valueBool>2;
				}else{
					if(!strcmp($<p.valueBool>2,$<p.valueBool>3)) $<p.valueBool>$ = "true";
					else $<p.valueBool>$ = "false";
				}
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
			if(strcmp($<p.tipo>1,"error")){
				if(!strcmp($<p.tipo>1,"int")) $<p.valueInt>$ = $<p.valueInt>1;
				else if(!strcmp($<p.tipo>1,"chars")) $<p.valueChar>$ = $<p.valueChar>1;
				else if(!strcmp($<p.tipo>1,"bool")) $<p.valueBool>$ = $<p.valueBool>1;
			}
		}else{
			if(!strcmp($<p.tipo>1,$<p.tipo>2) && strcmp($<p.tipo>2,"error")){
				$<p.tipo>$ = $<p.tipo>2;
				$<p.valueInt>$ = $<p.valueInt>1 + $<p.valueInt>2;
			}else{
				$<p.tipo>$ = "error";
			}
		}
	}
	;

u1:
	/* empty */ { $<p.tipo>$ = "empty";}
	| OPARSUMA v u1 { 
		if(!strcmp($<p.tipo>3,$<p.tipo>2) && !strcmp($<p.tipo>2,"int")){
			$<p.tipo>$ = "int";
			$<p.valueInt>$ = $<p.valueInt>2 + $<p.valueInt>3;
		}else if(!strcmp($<p.tipo>3,"empty") && !strcmp($<p.tipo>2,"int")){
			$<p.tipo>$ = "int";
			$<p.valueInt>$ = $<p.valueInt>2;
		}else{
			$<p.tipo>$ = "error";
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): exprecion suma solo acepta enteros %s/%s%s\n",RED_TERM, yylineno, $<p.tipo>2, $<p.tipo>3, BLACK_TERM);
		}
	}
	;
	
v:
	ID v1 {
		int tableID = existe_entrada_tablas_anteriores(TSStack,$<p.lexema>1);
		if (tableID == -1) 
		{
			fprintf(stderr," %sERROR SINTACTICO (Line:%d): identificator '%s' no esta declarado%s\n",RED_TERM, yylineno, $<p.lexema>1,BLACK_TERM);
			exit(-1);
		}else{
			char* tipo = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>1,"tipo");
			$<p.tipo>$ = tipo;
			if(!strcmp($<p.tipo>2, "empty")){
				if (existe_atributo(tableID,$<p.lexema>1,"value") == 0){ // exists
					if (!strcmp(tipo,"int")){
						$<p.valueInt>$ = consultar_valor_atributo_entero(tableID,$<p.lexema>1,"value");
					}
					else if (!strcmp(tipo,"chars")){
						$<p.valueChar>$ = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>1,"value");
					}	
					else if (!strcmp(tipo,"bool")){
						$<p.valueBool>$ = (char*) consultar_valor_atributo_cadena(tableID,$<p.lexema>1,"value");
					}else{
					}
				}
			}else{ // Function ou incrément
				if(!strcmp($<p.tipo>2,"incr")){
					if (!strcmp(consultar_valor_atributo_cadena(tableID, $<p.lexema>1, "tipo"),"int")){
						if (existe_atributo(tableID,$<p.lexema>1,"value") == 0){ // exists
							int idValue = consultar_valor_atributo_entero(tableID, $<p.lexema>1, "value");
							idValue++;
							asignar_valor_atributo_entero(tableID, $<p.lexema>1,"value", idValue);
							$<p.tipo>$ = "int"; 
							$<p.valueInt>$ = idValue;
						}
						else {
							fprintf(stderr," %sERROR SINTACTICO (Line:%d): the variable '%s' hasn't been initialized %s\n",RED_TERM, yylineno, $<p.lexema>1, BLACK_TERM);	
						}
					}
					else {
						fprintf(stderr," %sERROR SINTACTICO (Line:%d): la variable '%s' debe ser de tipo int %s\n",RED_TERM, yylineno, $<p.lexema>1, BLACK_TERM);	
					}	
				}else{
					if (!strcmp(tipo,"int")){
						$<p.valueInt>$ = $<p.valueInt>2;
					}
					else if (!strcmp(tipo,"chars")){
						$<p.valueChar>$ = $<p.valueChar>2;
					}	
					else if (!strcmp(tipo,"bool")){
						$<p.valueBool>$ = $<p.valueBool>2;
					}	
				}
			}
		}
	}
	| ABRPAR e CERPAR {
		$<p.tipo>$ = $<p.tipo>2;
		if(strcmp($<p.tipo>1,"error")){
			if(!strcmp($<p.tipo>2,"int")) $<p.valueInt>$ = $<p.valueInt>2;
			else if(!strcmp($<p.tipo>2,"chars")) $<p.valueChar>$ = $<p.valueChar>2;
			else if(!strcmp($<p.tipo>2,"bool")) $<p.valueBool>$ = $<p.valueBool>2;
		} 
	}
	| ENTERO { $<p.tipo>$ = "int"; $<p.valueInt>$ = $<p.valueInt>1;}
	| CADENA { $<p.tipo>$ = "chars"; $<p.valueChar>$ = $<p.valueChar>1;}
	| LOGICO { $<p.tipo>$ = "bool"; $<p.valueBool>$ = $<p.valueBool>1;}
	;

v1:
	/* empty */ { $<p.tipo>$ = "empty";}
	| ABRPAR l CERPAR { $<p.tipo>$ = "param";}
	| OPINCR { $<p.tipo>$ = "incr";}
	;

%%