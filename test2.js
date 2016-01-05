
var int x 
function int factorial (int x)
/* se define la función recursiva con un parámetro,
que oculta a la variable global de igual nombre */
{
if (x == 1)
{
return x + factorial (x + 1)
}
return 1
} /* la función devuelve un entero */


function Imprime (int a)
{
write (a)
return a+2 /* esta instrucción se podría omitir */
} 
Imprime (factorial (Suma (5, 3))) /* se llama a las tres funciones */