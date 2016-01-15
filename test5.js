/* TEST 3 : SENTENCIAS */

/* SENTENCIA CONDITIONAL SIMPLE "IF" */






/* SENTENCIA SELECCION MULTIPLE "SWITCH-CASE" */
/
var int a 
var bool test
switch (num)
{
	case 1: write("lunes") break /* break in the same line */
	case 2: write("martes")
			break	/* break in other line*/
	case 3: write("miércoles")/* without break */
	case 4:  write ("El dia numero ", num, " es Jueves .\n")  /* write with parameters*/
	
	case 8: if (a == 0) /*if without instructions */
		{ 					
		}
	
	case 9: if (test == true ) /*if with bool parameter */
		{ 			
			write ("case 9 ")
		}
	
	default: /* default is optional */
		if (num == 0 )/* if with instructions */
		{ 
			For = FactorialFor (num)
			While = FactorialWhile ()
			Do = FactorialDo (num)
			imprime (cadena (false), "recursivo es: ", FactorialRecursivo (num))
			imprime (s, "con do-while es: ", Do)
			imprime (s, "con while es: ", While)
			imprime (cadena (false), "con for es: ", For)		
		}
}

