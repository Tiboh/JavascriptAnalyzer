/* TEST 3 : SENTENCIAS */

/* SENTENCIA CONDITIONAL SIMPLE "IF" */






/* SENTENCIA SELECCION MULTIPLE "SWITCH-CASE" */

function Imprime (chars a)  
{
	write(a)
}
var int a 
var bool test
test = true 

switch (num)
{
	/*** case con break en la misma linea */
	case 1: write("lunes") break 
	
	/*** case con break en un otra linea */
	case 2: write("martes")
			break	
			
	/*** case sin break */		
	case 3: write("miercoles")
	
	/*** case con sentencia conditional simple if break */
	case 4: if (test == true) 
		{ 	write("Jueves")				
		}
	/*** case con llamada funcion ***/
	case 5 : Imprime("Viernes")
	
	
	/* DEFAULT IS OPTIONAL */
	/*** default ***/
	default: 
		if (test == true )
		{ 
			Imprime("Sabado")	
		}
		if (test == false )
		{ 
			write("Domingo")	
		}
}

