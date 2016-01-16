/* PRUEBA 4 : OPERADORES */

/* Arithmetico '+' */
	var int a 
	var int b
	var int sum1
	var int sum2

	sum1=a+b
	sum2=(a+b)+(sum1+a)+b

/* Logico '&&' */
	var bool b1
	var bool b2
	var bool b3
	b1 = true
	b2 = false

	b3 = b1 && b2

/* Relational '==' */
	/* Comparacion enteros */
		if(sum1==3)
			write("int Succeed")
	
	/* Comparacion booleanos */
		if(b1==true)
			write("bool Succeed")

	/* Comparacion de mas de dos variables de mismo tipo */
		var bool comp1
		var bool comp2
		if(comp1==comp2==true)	
			write("Multiple variables Succeed")
		
/* Post Auto Incrementacion '++' as sufijo */
	a++ 
	a = sum1++

/* Asignacion '=' */
	a = b

/* Asignacion con operacion '+=' */
	a+= b
	a+= sum1 + sum2
