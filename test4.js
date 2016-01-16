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
	if(sum1==3)
		write("int Succeed")

	if(b1==true)
		write("bool Succeed")

/* Post Auto Incrementacion '++' as sufijo */
	a++ 
	a = sum1++

/* Asignacion '=' */
	a = b

/* Asignacion con operacion '+=' */
	a+= b
	a+= sum1 + sum2
