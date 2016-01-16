/* TEST 3 : SENTENCIAS */

/* SENTENCIA CONDITIONAL SIMPLE "IF" */

/** sentencias if anidadas **/
var int var1 
var int var2
var1 = 1
if  (var1==1){
	prompt(var2)
	if (var2==1)
		write("test sentencias if anidadas")
}

/* SENTENCIA SELECCION MULTIPLE "SWITCH-CASE" */

function Imprime (int a)  
{
	write(a)
}

function Main ()

{
	var int a 
	var bool test
	test = true 

	var int num
	num = 5
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
		case 5 : 
				Imprime(num)

		/* DEFAULT IS OPTIONAL */
		/*** default ***/
		default: 
			if (test == true )
			{ 
				write("Sabado")	
			}	
			
	}
}
