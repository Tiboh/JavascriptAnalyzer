/* TEST 2 : FUNCIONES */

/* TEST DE PARAMETROS Y TIPO DE VUELTO DE FUNCION */

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
			return a
			}
	
}


