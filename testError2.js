/* ERROR PRUEBA 2 : FUNCIONES */

/* DECLARACION DE FUNCION */
	/** Declaracion de dos funciones con el mismo nombre **/
		function int F1(int a)  
			{
				return a
			}
		function F1(int a)  
			{
				write(a)
			}	

/* CASOS DE ERROR TIPO DE VUELTA DE FUNCION */
	
	/** Tipo de vuelta incorrecto **/
		function int Test1 (chars a)  
		{
			return a
		}
		
	/** Return necessario **/
		function int Test2 (chars a)  
		{
			write(a)
		}
		
	/** no hay que tiene un return **/
		function Test3 (int a)  
		{
			write(a)
			return a
		}

/* LLAMADA DE FUNCION */

	/** Llamada de funcion con parametros de tipo incorrecto **/
		
		function Func1(chars a, int b){
			write (a)
		}
		var int c
		var int d
		Func1(c,d)
	
	/** Llamada de funcion con nombre de parametros incorrecto **/
		var chars varChars
		Func1(varChars)
		
	/** Llamada de funcion no declarada **/
		Func2(c)
		 