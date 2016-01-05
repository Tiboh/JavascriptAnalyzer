
function int Suma ( int a,  int b) /* function with  return one variable, with 2 parameters */
{
	var int c
	c = a + b
	return c
}

function int Suma2 (int a, int b) /* operation in return */
{
	return a + b
}

function Imprime (int a)  /* function without return and with one parameter */
{
	write(a)
}

function int Nothing () /* function without parameters */
{
	write("test function without parameters")
}

function bool Logico (bool a, bool b) /* parametro logico */
{
	if (a==true && b==true)
	return true
}

/* RECURSIVIDAD */

function int TestRecursif (int a)
{	
	a = 5
	if (a==5) {
			return a + TestRecursif (a + 1)
			}
	return a
}

/*VARIABLES LOCALES ET GLOBALES */
var int global1
global1 = 1

function TestGlobal ( int global1)
{
	global1 = 2
}

function WriteGlobal ()
{	
	write(global1)
}

/* CASOS DE ERRORES */

function int Logico (bool a, bool b) /* function with non valid return type */
{
	if (a==true && b==true)
	return a
}

function Logico (bool a, bool b) /* this function shouldn't have a return  */
{
	if (a==true && b==true)
	write ("this function shouldn't have a return")
	return true
}


/*** no se permitten funciones anidadas 

function func1 (int a) 
{
	function func2 (int b)
	{
		write("can't declare function in an other")
	}
}
***/