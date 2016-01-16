/* ERROR PRUEBA 4 : OPERADORES */

/* Arithmetico '+' */
	/** Error en tipo de variable **/
		var int ar1 
		var chars ar2
		var int sum1
		sum1 = ar1+ar2

/* Logico '&&' */	
	/** Error en tipo de variable **/
		var bool log1
		var bool log2
		var int log3
		log2 = log1 && log3
	
/* Relational '==' */
	/** No se puede comparar cadenas **/
		var chars rel1
		var chars rel2
		if(rel1 == rel2) write(rel1)
		
	/** Comparar tipos distintos **/
		var int rel3
		if(rel3 == true) write(rel3)

	/** Comparar mas de dos variables distintos **/
		var int rel4
		var int rel5
		if(rel4 == rel5 == true) write(rel4)	
		
/* Post Auto Incrementacion '++' as sufijo */

	/** Error en tipo de variable **/
		var chars incr
		incr++

/* Asignacion '=' */

	/** Variables de tipos distintos **/		
		var int asi1
		var chars asi2
		asi1 = asi2
	
/* Asignacion con operacion '+=' */

	/** Variables de tipos distintos **/		
		var int asi5
		var chars asi6
		asi5 += asi6	
	
	/** Variables de tipo cadena **/		
		var chars asi7
		var chars asi8
		asi7 += asi8
	
	/** Variables de tipo booleano **/		
		var bool var3
		var bool var4
		var3 += var4	
	
		
		
		
		
		