/* PRUEBA 1 : VARIABLES */

/* PRUEBA LEXICAL */

	/** Identificador **/
		var int test1
		var int test1_
		var int Atest1_IDENTIFICADOR

	/** Entero **/
		test1 = 1234

	/** Cadena **/
		var chars testCadena
		testCadena=""
		testCadena="Prueba de tipo cadena"
		testCadena="Prueba 124 de tipo cadena"

	/** Logico **/
		var bool bool1
		var bool bool2
		bool1 = true
		bool2 = false

/* PRUEBA DE VARIABLES GLOBALES Y LOCALES */

	var int global1
	global1 = 1000
	var int a
	a = 1

	function int TestGlobal ( int a)
	{
		a = 2
		write ("La volor de 'a' local es = ")
		write (a)
		write (global1 + a)
		return a
	}

	write ("La volor de 'a' global es =  ")
	write (a)

/* PRUEBA DE TIPOS Y DECLARACION DE VARIABLES */

	var int variable_1
	var bool variable_2
	var chars variable_3

	variable_1 = 5
	variable_2 = true
	variable_3 = "mivariablestring"
