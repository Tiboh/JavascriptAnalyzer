/* TEST 2 : FUNCIONES */

/* TEST INPUT OUTPUT TERMINAL */

function TestTerminal () 
{
	var int test
	prompt(test)
	write(test)
}

/* TEST DE PARAMETROS DE FUNCION */

/** sin parametro **/
function Nothing () 
{
	write("test function without parameters")
}

/** con un parametro **/
function Imprime (int a)  
{
	write(a)
}

/** con dos parametros **/
function int Suma ( int a,  int b)  
{
	var int c
	c = a + b
	return c
}

/** con parametros de tipos distintos **/
function int TestTiposDistintos (int a, bool b, chars c) 
{
	if (b==true)
	{
		write(c)
		return a
	}
	return a
}

/* TEST DE TIPO DE VUELTA DE FUNCION */

/** sin return**/
function TestWithoutReturn(int a, int b) 
{	
	var int c
	c = a + b
}

/** return variable **/
function int TestReturnVariable (int a, int b) 
{
	return a
}

/** return expresion **/
function int Suma2 (int a, int b) 
{
	return a + b
}

/* TEST LLAMADA DE FUNCION */

function Main()
{
	var int variable1
	var int variable2
	var int variable3
	var int variable4
	
	variable1 = 10
	variable2 = 20
	
	variable3 = Suma(variable1,variable2)
	variable4 = Suma2(variable1,variable3)
	Imprime(variable4)
}


/* RECURSIVIDAD */

function int TestRecursif (int a)
{	
	a = 5
	if (a==5) {
			return a
			}
	TestRecursif(a+1)		
	return a
}