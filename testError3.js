/* ERROR PRUEBA 3 : SENTENCIAS */

/* CASOS DE ERROR DE SENTENCIA CONDITIONAL SIMPLE "IF" */

	/** Condicion de tipo incorrecto **/
		var chars var1
		if(var1){
		}
		
/* CASOS DE ERROR DE SENTENCIA SELECCION MULTIPLE "SWITCH-CASE" */

	/** Variable del switch no declarada **/
		switch(num){
			case 1 :
			case 2 :		
		}
		
	/** Valor de case de tipo incorrecto **/
		var chars var2 
		switch(var2){
			case 1 :
			case 2 :		
		}
		
	/** Valor de case de tipos distintos **/	
		var chars var3 
		switch(var3){
			case "a" : 
			case 2 :		
		}
	
	/** Syntax error : Default no puede contenir break **/	
		var int var4
		switch(var4){
			case 1 : write(var4) break
			case 2 : write(var4) break
			default : write(var4) break
		}
	