/* TEST 1 : VARIABLES */


/* TEST DE VARIABLES GLOBALES Y LOCALES */

var int a
a = 1

function int TestGlobal ( int a)
{
	a = 2
	write ("La volor de 'a' local es = ")
	write (a)
	return a
}

write ("La volor de 'a' global es =  ")
write (a)

/* TEST DE TIPOS Y DECLARACION DE VARIABLES */

var int variable_1
var bool variable_2
var chars variable_3

variable_1 = 5
variable_2 = true
variable_3 = "mivariablestring"
