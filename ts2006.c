// Tabla de Símbolos 2006
// Creada por Daniel Peña Vázquez
// Supervisada por José Luis Fuertes Castro

// Este código es C estándar.
// Gracias a ello, se puede compilar correctamente y sin warnings 
// al menos en los siguientes compiladores:
// - GCC  ( probado en la version DJGPP 2.30 )
// - BCC32 ( probado en la versión Borland C++ 5.5.1 )
// - Microsoft Visual C++ ( probado en la versión 6.0 )

// Nota: las cadenas que maneja esta tabla de simbolos son case-sensitive
// (no es lo mismo mayusculas que minusculas)

// impedir desastres si se incluye este codigo varias veces :
#ifndef _ts2006_c_incluido_
#define _ts2006_c_incluido_


#include "ts2006.h" // cabeceras de las funciones

// definiciones de constantes
#define cte_espacio_hash                    53
#define cte_max_tablas              2147483647
#define cte_error_no_existe_tabla           -1
#define cte_error_ya_existe_entrada         -2
#define cte_error_no_existe_entrada         -3
#define cte_error_ya_existe_atributo        -4
#define cte_error_no_existe_atributo        -5
#define cte_error_lexema_es_null            -6
#define cte_error_alias_es_null             -7
#define cte_error_fichero_es_null           -8
#define cte_error_tipo_es_null              -9
#define cte_error_creacion_fichero         -10
#define cte_error_tipo_no_definido         -11
#define cte_error_atributo_no_entero       -12
#define cte_error_atributo_no_cadena       -13
#define cte_error_tipo_entrada_no_definido -14
#define cte_error_demasiadas_tablas        -15
#define cte_error_sin_memoria              -16
#define cte_error_atributo_cadena_es_null  -17
#define cte_error_valor_es_null            -18
#define cte_error_posicion_es_null         -19

// definiciones de conjuntos
enum conj_tipo_atributo {tipo_entero,tipo_cadena};

// definiciones de uniones
union enteroocadena {
 int entero; 
 char *cadena;
 } ; // almacena [entero o cadena]


// definiciones de tipos
typedef struct nodo_atributo
{
	char *nombre_atributo; // nombre del atributo
	enum conj_tipo_atributo tipo_atributo; // tipo que almacena de cara al usuario
        union enteroocadena valor; // almacena [entero o cadena]
	struct nodo_atributo *sig_atributo; // siguiente atributo en la lista
}tipo_atributo; // Tipo Atributo de una entrada de una tabla de simbolos

typedef struct nodo_entrada
{
	char *lexema; // lexema que determina la entrada
	char *tipo_entrada; // tipo de entrada
	struct nodo_atributo *primer_atributo; // lista de atributos
	struct nodo_entrada *sig_hash; // para colisiones en la funcion hash
}tipo_entrada; // Tipo Entrada de una tabla de simbolos

typedef struct nodo_tabla_de_simbolos
{
	int id_tabla; // identificador
	struct nodo_entrada *entrada[cte_espacio_hash]; // array de entradas
	struct nodo_tabla_de_simbolos *sig_tabla; // siguiente tabla en la lista
}tipo_tabla_de_simbolos; // Tipo Tabla de simbolos

// variables globales (pero en principio el usuario no deberia tocarlas)
tipo_tabla_de_simbolos *tablas_de_simbolos=NULL; // Lista de tablas de simbolos
int ultimo_id_error = 0; // ID del último error que se ha producido
int contador_tablas = 0; // Contador de las tablas creadas

// FUNCIONES PRIVADAS :

int funcion_hash(char *cadena)
{  // Devuelve numero entre 
   // 0 y cte_espacio_hash - 1
   // calculado a partir de la cadena indicada.
  int suma_ascii = 0;
  int lon;
  int i;
  int resultado;
  char letra;
  int ascii_letra;
  // no debería ocurrir que la cadena fuera NULL ,
  if (cadena == NULL)
  {
    // pero en ese caso se devolverá 0
    resultado = 0;  
  }
  else
  {
    lon = strlen(cadena);
    for (i=0; i<lon; i++ )
	{
	  letra = cadena[i];
	  // Se convertirá cada letra a su valor entero ascii.
	  ascii_letra = (int) letra;
	  // Se sumarán todos esos valores.
      suma_ascii = suma_ascii+ascii_letra; 
	}
	// Se hace modulo con cte_espacio_hash
    resultado = suma_ascii % cte_espacio_hash; 
  }
  return resultado;
}

// FUNCIONES PUBLICAS :


// ************** Crear una tabla de símbolos ******************************************
// Proceso DFD  : 1.1
// ID Requisito : REQ04
// Entrada      : <nada>
//
// Salida       : - Identificador de la tabla creada (número positivo) si se creó.
//                - Devuelve -1 si se produce un error.
// Descripción  : Crea una nueva tabla de símbolos y
//                 devuelve el identificador que se le ha asignado.
// *************************************************************************************
int crear_tabla()
{
   int i;
   tipo_tabla_de_simbolos *nueva_tabla;
   tipo_tabla_de_simbolos *puntero;
   if (contador_tablas == cte_max_tablas)
   {
	 ultimo_id_error = cte_error_demasiadas_tablas; // Error: se han creado demasiadas tablas 
     return -1;
   }
   contador_tablas = contador_tablas+1; // Incrementamos el contador de tablas creadas hasta el momento
   // Creamos la nueva tabla (vacia)
   nueva_tabla = (tipo_tabla_de_simbolos *) malloc(sizeof(tipo_tabla_de_simbolos));
   if (nueva_tabla == NULL) // sin memoria para mas
   {
	 ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
     return -1;
   }
   nueva_tabla->id_tabla = contador_tablas;
   nueva_tabla->sig_tabla = NULL;
   for (i=0; i<cte_espacio_hash; i++ ) // todas sus entradas estaran vacias
   {
     nueva_tabla->entrada[i] = NULL;
   }
   // Si la lista es vacia, la tabla va al principio de la lista.
   if (tablas_de_simbolos == NULL)
   {
     tablas_de_simbolos = nueva_tabla;
   }
   else // Si no, enlazaremos la nueva tabla al final de la lista de tablas de simbolos
   {
     puntero = tablas_de_simbolos;
     while (puntero->sig_tabla != NULL) // Buscamos el final de la lista
	 {
       puntero = puntero->sig_tabla;
	 }
     puntero->sig_tabla = nueva_tabla; // ponemos la tabla a continuacion
   }
   return contador_tablas; // devolvemos al usuario el ID de la tabla que acabamos de crear
}


// ************** Destruir una tabla de símbolos ***************************************
// Proceso DFD  : 1.2
// ID Requisito : REQ05
// Cabecera     : int destruir_tabla(int id_tabla);
// Entrada      : - Identificador de la tabla que se quiere destruir.
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Se destruye la tabla indicada.
// *************************************************************************************
int destruir_tabla(int id_tabla) 
{  
   int resultado = -1; // resultado de la operacion.
   int i;
   tipo_tabla_de_simbolos *a_destruir; // tabla que vamos a destruir
   tipo_tabla_de_simbolos *previo = NULL; // tabla previa a la que vamos a destruir
   tipo_tabla_de_simbolos *siguiente = NULL; // tabla siguiente a la que vamos a destruir
   // algunos punteros auxiliares para recorrer el contenido de la tabla y liberar toda su memoria :
   tipo_entrada *p_entrada; 
   tipo_entrada *p_entrada_a_destruir; 
   tipo_atributo *p_atributo;
   tipo_atributo *p_atributo_a_destruir;
   // Calculamos a_destruir y previo, a la vez que vemos si la tabla existe o no : 
   if (tablas_de_simbolos != NULL)
   {
	 a_destruir=tablas_de_simbolos; 
     do
	 {
	   if ( a_destruir->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	   {
         resultado = 0; // .. entonces es que existe
	   }
	   else
	   {
		 previo = a_destruir;
         a_destruir = a_destruir->sig_tabla;
	   }
	 }
	 while ((a_destruir != NULL) && (resultado == -1));
   }
   if (resultado == 0)
   {
     siguiente = a_destruir->sig_tabla; // calculamos siguiente
	 // Ya podemos destruir la tabla actual (a_destruir) ...
	 // Primero liberamos la memoria de todas sus entradas
	 for (i=0; i<cte_espacio_hash; i++ ) 
	 {
       if (a_destruir->entrada[i] != NULL)
	   {
		 p_entrada=a_destruir->entrada[i];
		 do // recorremos todas las entradas enlazadas con esta entrada, destruyendolas
		 {
		   if (p_entrada->primer_atributo!=NULL )
		   {
             p_atributo=p_entrada->primer_atributo;
			 do // Destruimos memoria de todos sus atributos
			 {
               p_atributo_a_destruir=p_atributo;
			   p_atributo=p_atributo->sig_atributo;
			   if (p_atributo_a_destruir->nombre_atributo != NULL)
			   {
			     free(p_atributo_a_destruir->nombre_atributo); // liberamos memoria de la cadena nombre_atributo
			   }
			   if (p_atributo_a_destruir->tipo_atributo == tipo_cadena) // si el atributo es de tipo cadena ...
			   {
				 if (p_atributo_a_destruir->valor.cadena != NULL)
				 {
                   free(p_atributo_a_destruir->valor.cadena); // ... liberamos memoria de la cadena
				 }
			   }
			   free(p_atributo_a_destruir);
			 }
             while (p_atributo !=NULL);
		   }
		   p_entrada_a_destruir=p_entrada;
           p_entrada=p_entrada->sig_hash; // avanzamos en la lista de las colisiones hash
		   if (p_entrada_a_destruir->tipo_entrada != NULL)
		   {
		     free(p_entrada_a_destruir->tipo_entrada); // liberamos memoria de la cadena tipo_entrada
		   }
		   free(p_entrada_a_destruir); // liberamos entrada en si
		 }
		 while (p_entrada != NULL);
	   }
	 }
	 // liberamos la memoria de la tabla en si
	 free(a_destruir); 

	 if (previo == NULL) // Si hemos destruido el primer elemento...
	 {
       tablas_de_simbolos = siguiente; // enlazar la raiz de la lista con el siguiente
	 }
	 else  // si no ...
	 {
       previo->sig_tabla = siguiente; // enlazar el previo con el siguiente
	 }
   }
   else
   {
     ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
   }
   return resultado; // Devolvemos el resultado
}


// ************** Determinar la existencia de una tabla de símbolos ********************
// Proceso DFD  : 1.3
// ID Requisito : REQ15
// Entrada      : - Identificador de la tabla que se quiere consultar.
//
// Salida       : - Devuelve 0 si existe.
//                - Devuelve 1 si no existe. 
//                - Devuelve -1 si se produce un error.
// Descripción  : Determina si una tabla existe 
//                 (ya fue creada y aun no ha sido destruida)
// *************************************************************************************
int existe_tabla(int id_tabla)
{  // Determina si actualmente existe la tabla con el ID especificado
   // 0 = existe  ; 1 = no existe
   int resultado = 1; // no existe por defecto
   tipo_tabla_de_simbolos *puntero;
   puntero = tablas_de_simbolos;
   if (puntero != NULL)
   {
     do
	 {
	   if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	   {
         resultado = 0; // .. entonces es que existe
	   }
       puntero = puntero->sig_tabla;
	 }
	 while ((puntero != NULL) && (resultado == 1));
   }
   return resultado; // Devolvemos el resultado
} 


// ************** Crear una nueva entrada **********************************************
// Proceso DFD  : 2.1
// ID Requisito : REQ06
// Entrada      : - Identificador de la tabla donde se quiere crear la entrada.
//                - Lexema que se quiere insertar en la nueva entrada.
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Crea una nueva entrada en la tabla indicada
//                 con el lexema indicado. 
// *************************************************************************************
int crear_entrada(int id_tabla, char *lexema)
{
  int resultado = -1; // no existe por defecto
  int posicion;
  tipo_entrada *nueva_entrada;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  int bytes_cadena;
  char* cadena_tmp;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // insertamos directamente la entrada 
      nueva_entrada = (tipo_entrada *) malloc(sizeof(tipo_entrada));
	  if (nueva_entrada == NULL) // sin memoria para mas
	  {
	    ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada
        return -1;
	  }
	  bytes_cadena = strlen(lexema)+1;
	  cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	  if (cadena_tmp == NULL) // sin memoria para mas
	  {
	    ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada
        return -1;
	  }
	  strcpy(cadena_tmp,lexema);
	  nueva_entrada->lexema=cadena_tmp;
      nueva_entrada->primer_atributo=NULL;
      // inicializamos el tipo de la entrada con NULL
	  nueva_entrada->tipo_entrada=NULL;
	  nueva_entrada->sig_hash=NULL;  
	  puntero->entrada[posicion]=nueva_entrada;
	}
	else // si ya existe alguno -> colision . Primero ver si ya existe ese lexema :
	{
	  p_entrada=puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{
          resultado = -1; // .. devolvemos error
		  ultimo_id_error = cte_error_ya_existe_entrada; // Ya existe una entrada con ese lexema en esa tabla
		}
		else
		{
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == 0));
	  if (resultado == 0) 
	  { // si no existia el lexema ya, añadirlo a la lista de colisiones para esa entrada
        nueva_entrada = (tipo_entrada *) malloc(sizeof(tipo_entrada));
		if (nueva_entrada == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
          return -1;
		}
	    bytes_cadena = strlen(lexema)+1;
	    cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	    if (cadena_tmp == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada
          return -1;
		}
	    strcpy(cadena_tmp,lexema);
	    nueva_entrada->lexema=cadena_tmp;
        nueva_entrada->primer_atributo=NULL;
        // inicializamos el tipo de la entrada con NULL
	    nueva_entrada->tipo_entrada=NULL;
	    nueva_entrada->sig_hash=NULL;     
        p_entrada_previa->sig_hash=nueva_entrada;
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado; // Devolvemos el resultado
}


// ************** Asignar el tipo de entrada a una entrada *****************************
// Proceso DFD  : 2.2
// ID Requisito : REQ07
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Tipo que se le quiere dar a la entrada.
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Asigna un tipo a una entrada existente.
// *************************************************************************************
int asignar_tipo_entrada(int id_tabla, char *lexema, char *tipo)
{ // Asigna tipo de entrada a una entrada
  int resultado = -1; // no existe por defecto
  int posicion;
  char* cadena_tmp; // cadena temporal
  int bytes_cadena = 0; // para la longitud del lexema en bytes
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL) && (tipo != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
      resultado=-1;
	}
	else // vemos si existe ese lexema :
	{
	  resultado=-1;
	  p_entrada=puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{
          // liberamos la cadena anterior
          if (p_entrada->tipo_entrada != NULL)
		  {
            free(p_entrada->tipo_entrada);
		  }
		  // escribimos la nueva cadena
	      if (tipo == NULL) 
		  {
            p_entrada->tipo_entrada = NULL; // no deberia consultarse antes de ser definido
			ultimo_id_error = cte_error_tipo_entrada_no_definido; // Error: no se ha definido_aun_tipo_de_entrada
		  }
		  else
		  {
		    bytes_cadena = strlen(tipo)+1;
	        cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	    	if (cadena_tmp == NULL) // sin memoria para mas
			{
	          ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
              return -1;
			}
	        strcpy(cadena_tmp,tipo);
	        p_entrada->tipo_entrada=cadena_tmp;
		  }
          resultado = 0; // operacion realizada correctamente
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1));
	  if (resultado == -1) 
	  { // no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
        resultado=-1;
	  }
	}
  }
  else
  {
	if (lexema == NULL )
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (tipo == NULL)
	{
      ultimo_id_error = cte_error_tipo_es_null; // Error: se intento asignar un tipo nulo
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado; // Devolvemos el resultado
}


// ************** Consultar el tipo de entrada de una entrada **************************
// Proceso DFD  : 2.3
// ID Requisito : REQ08
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//
// Salida       : - Devuelve tipo de la entrada si se ha definido uno y no hay errores.
//                - Devuelve NULL si se produce un error.
// Descripción  : Consulta el tipo de una entrada.
// *************************************************************************************
char* consultar_tipo_entrada(int id_tabla, char *lexema)
{ // Consulta tipo de entrada de una entrada
  int resultado = -1; // no existe por defecto
  char* cadena_resultado = NULL; // devolver error por defecto
  int posicion;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
      resultado=-1;
	}
	else // vemos si existe ese lexema :
	{
	  resultado=-1;
	  p_entrada=puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{
		  if (p_entrada->tipo_entrada == NULL)
		  {
            cadena_resultado = NULL;
		  }
		  else
		  {
		    cadena_resultado = p_entrada->tipo_entrada;
		  }
		  resultado = 0; // salimos del bucle
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1));

	  if (resultado == -1) 
	  { // no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
        resultado=-1;
	  }
	  else if (cadena_resultado == NULL) // encontrado pero nulo .... 
	  {
	    ultimo_id_error = cte_error_tipo_no_definido; // Error: tipo de entrada no definido
        resultado = -1; // (lo consideraremos un error)
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return cadena_resultado; // Devolvemos el resultado ( o NULL si algo ha fallado )
}


// ************** Determinar la existencia de una entrada ******************************
// Proceso DFD  : 2.4
// ID Requisito : REQ16
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//
// Salida       : - Devuelve 0 si existe.
//                - Devuelve 1 si no existe.
//                - Devuelve -1 si se produce un error.
// Descripción  : Determina si una entrada existe 
//                 ( fue ya creada y aun no se ha destruido su tabla )
// *************************************************************************************
int existe_entrada(int id_tabla, char *lexema)
{ // busca un lexema en una tabla
  // -1 = error ; 0 = existe ; 1 = no existe
  int resultado = 1;  
  int posicion;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  puntero = tablas_de_simbolos;
  // primero ver si la tabla existe
  if ((puntero != NULL) && (lexema != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  
	  {
        resultado = 0; // existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla; // seguir buscando
	  }
	}
	while ((puntero != NULL) && (resultado == 1));
  }
  // si la tabla existe, buscar lexema
  if (resultado == 0 )
  {
	resultado = 1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  
      resultado = 1; // no existe, salir
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
	      resultado = 0; // salir
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == 1));
	}
  }
  else
  {
	if (lexema == NULL)
	{
      resultado = -1 ; // no se permiten lexemas NULL
	}
	else
	{
      // Ni siquiera existe la tabla indicada
	  resultado = 1 ; // lo consideraremos como -no existe-
	}
  }
  return resultado; // Devolvemos el resultado de la operacion
}


// ************** Crear un atributo entero en una entrada ******************************
// Proceso DFD  : 3.1
// ID Requisito : REQ09
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo (ejemplos: "desplazamiento", "ancho","tabla")
//                - Valor inicial para el atributo que estamos 
//                   creando (ejemplos: 16, 48, 3)
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Crea un nuevo atributo en la entrada, con el alias indicado. 
//                 Este atributo solo podrá almacenar valores enteros (tipo int de C)
// *************************************************************************************
int crear_atributo_entero(int id_tabla, char *lexema, char *alias_at, int valor)
{ // Crea un nuevo atributo entero en la tabla indicada, en la entrada de lexema indicado
  // inicializandolo con el valor indicado
  int resultado = -1; // error por defecto
  int atributo_encontrado = -1; // lo pondremos a 0 si el atributo existe
  char* cadena_tmp; // cadena temporal
  int posicion;
  int bytes_cadena = 0; // para la longitud del lexema en bytes
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  tipo_atributo *nuevo_atributo;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
      resultado=-1;
	}
	else // vemos si existe ese lexema :
	{
	  resultado=-1;
	  p_entrada=puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ninguno creado aun, creamos el primer atributo:
            nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
			if (nuevo_atributo == NULL) // sin memoria para mas
			{
	          ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
              return -1;
			}
			nuevo_atributo->sig_atributo = NULL;
			nuevo_atributo->tipo_atributo = tipo_entero;
			nuevo_atributo->valor.entero = valor;
			bytes_cadena = strlen(alias_at)+1;
	        cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
			if (cadena_tmp == NULL) // sin memoria para mas
			{
	          ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
              return -1;
			}
	        strcpy(cadena_tmp,alias_at);
			nuevo_atributo->nombre_atributo=cadena_tmp;	
			p_entrada->primer_atributo=nuevo_atributo;
			resultado = 0; // operacion realizada correctamente
		  }
		  else // vemos si el atributo no estaba ya creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
			    atributo_encontrado=0; // estaba ya creado
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (atributo_encontrado == -1));
		    if (atributo_encontrado == -1 ) // si no estaba creado ...
			{ // ... creamos el nuevo atributo al final de la lista de atributos
              nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
		      if (nuevo_atributo == NULL) // sin memoria para mas
			  {
	            ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                return -1;
			  }
			  nuevo_atributo->sig_atributo = NULL;
			  nuevo_atributo->tipo_atributo = tipo_entero;
			  nuevo_atributo->valor.entero = valor;
			  bytes_cadena = strlen(alias_at)+1;
	          cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	          if (cadena_tmp == NULL) // sin memoria para mas
			  {
	            ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                return -1;
			  }
	          strcpy(cadena_tmp,alias_at);
			  nuevo_atributo->nombre_atributo=cadena_tmp;	
			  p_atributo_previo->sig_atributo=nuevo_atributo;
		      resultado = 0; // operacion realizada correctamente
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1) && (atributo_encontrado == -1));
	  if (resultado == -1) 
	  { 
		// no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	  }
      if (atributo_encontrado == 0) 
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_ya_existe_atributo; // Ya existia el atributo. No se ha modificado.
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (alias_at == NULL)
	{
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado; // Devolvemos el resultado
}


// ************** Crear un atributo cadena en una entrada ******************************
// Proceso DFD  : 3.2
// ID Requisito : REQ10
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo (ejemplos: "cadena", "real","cosa")
//                - Valor inicial para el atributo que estamos 
//                  creando (ejemplos: "algo", "12.345", "123abcd")
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Crea un nuevo atributo en la entrada, con el alias indicado. 
//                 Este atributo solo podrá almacenar cadenas (tipo char* de C)
// *************************************************************************************
int crear_atributo_cadena(int id_tabla, char *lexema, char *alias_at, char *valor)
{ // Crea un nuevo atributo cadena en la tabla indicada, en la entrada de lexema indicado
  // inicializandolo con el valor indicado
  int resultado = -1; // error por defecto
  int atributo_encontrado = -1; // lo pondremos a 0 si el atributo existe
  char* cadena_tmp; // cadena temporal (para copiarnos el alias_at)
  char* cadena_tmp2; // otra cadena temporal (para copiarnos el valor)
  int posicion;
  int bytes_cadena = 0; // para la longitud del lexema en bytes
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  tipo_atributo *nuevo_atributo;
  puntero = tablas_de_simbolos;

  if (valor==NULL) // (comentando estas 5 lineas permitiriamos almacenar cadenas NULL)
  {
    ultimo_id_error = cte_error_valor_es_null;  // No se permite almacenar cadenas NULL
    return -1; // no seguir
  }

  if ((puntero != NULL) && (lexema != NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
      resultado=-1;
	}
	else // vemos si existe ese lexema :
	{
	  resultado=-1;
	  p_entrada=puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ninguno creado aun, creamos el primer atributo:
            nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
		    if (nuevo_atributo == NULL) // sin memoria para mas
			{
	          ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
              return -1;
			}
			nuevo_atributo->sig_atributo = NULL;
			nuevo_atributo->tipo_atributo = tipo_cadena;
			if (valor == NULL)
			{
              nuevo_atributo->valor.cadena = NULL;	
			}
			else
			{
		      bytes_cadena = strlen(valor)+1;
	          cadena_tmp2 = (char*) malloc(bytes_cadena*sizeof(char*));
			  if (cadena_tmp2 == NULL) // sin memoria para mas
			  {
	            ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                return -1;
			  }
	          strcpy(cadena_tmp2,valor);
			  nuevo_atributo->valor.cadena = cadena_tmp2;	
			}
			bytes_cadena = strlen(alias_at)+1;
	        cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
		    if (cadena_tmp == NULL) // sin memoria para mas
			{
	          ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada
              return -1;
			}
	        strcpy(cadena_tmp,alias_at);
			nuevo_atributo->nombre_atributo=cadena_tmp;	
			p_entrada->primer_atributo=nuevo_atributo;
			resultado = 0; // operacion realizada correctamente
		  }
		  else // vemos si el atributo no estaba ya creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
			    atributo_encontrado=0; // estaba ya creado
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (atributo_encontrado == -1));
		    if (atributo_encontrado == -1 ) // si no estaba creado ...
			{ // ... creamos el nuevo atributo al final de la lista de atributos
              nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
		      if (nuevo_atributo == NULL) // sin memoria para mas
			  {
	            ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                return -1;
			  }
		      nuevo_atributo->sig_atributo = NULL;
			  nuevo_atributo->tipo_atributo = tipo_cadena;
			  if (valor == NULL)
			  {
                nuevo_atributo->valor.cadena = NULL;	
			  }
			  else
			  {
		        bytes_cadena = strlen(valor)+1;
	            cadena_tmp2 = (char*) malloc(bytes_cadena*sizeof(char*));
			    if (cadena_tmp2 == NULL) // sin memoria para mas
				{
	              ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                  return -1;
				}
	            strcpy(cadena_tmp2,valor);
			    nuevo_atributo->valor.cadena = cadena_tmp2;	
			  }
		      bytes_cadena = strlen(alias_at)+1;
	          cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
			  if (cadena_tmp == NULL) // sin memoria para mas
			  {
	            ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                return -1;
			  }
	          strcpy(cadena_tmp,alias_at);
			  nuevo_atributo->nombre_atributo=cadena_tmp;	
			  p_atributo_previo->sig_atributo=nuevo_atributo;
		      resultado = 0; // operacion realizada correctamente
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1) && (atributo_encontrado == -1));
	  if (resultado == -1) 
	  { 
		// no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	  }
      if (atributo_encontrado == 0) 
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_ya_existe_atributo; // Ya existia el atributo. No se ha modificado.
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (alias_at == NULL)
	{
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado; // Devolvemos el resultado
}


// ************** Asignar el valor de un atributo entero *******************************
// Proceso DFD  : 3.3
// ID Requisito : REQ11
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo.
//                - Nuevo valor que se quiere escribir en ese atributo (un entero)
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Da un valor al atributo (entero) de alias indicado en la 
//                 entrada indicada (sobrescribe el valor antiguo)
// *************************************************************************************
int asignar_valor_atributo_entero(int id_tabla, char *lexema, char *alias_at, int valor)
{  // Asigna un valor entero a un atributo entero
  int resultado = -1; // error por defecto
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  int posicion;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
	resultado = -1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ningun atributo -> fallo directo
			atributo_encontrado = -1;
			resultado = 0;
		  }
		  else // vemos si el atributo esta creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
				if (p_atributo->tipo_atributo != tipo_entero)
				{
                  error_tipos = 0;
                }
				else
				{
				  p_atributo->valor.entero = valor; // escribimos el nuevo valor 
				}
				resultado = 0; // salir
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (resultado == -1));
			if (p_atributo==NULL)
			{
			  // no se encontro el atributo
		      atributo_encontrado = -1;
			  resultado = 0;
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1) && (atributo_encontrado == 0));
	  if (resultado == -1) 
	  { 
		// no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	  } 
	  else if (atributo_encontrado == -1) 
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
	  }
	  else if (error_tipos == 0)
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_atributo_no_entero; // El atributo existe pero no es del tipo entero
	  }
	}
  }
  else
  {
	if (alias_at == NULL)
	{
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite atributo de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado; // Devolvemos el resultado de la operacion
}


// ************** Asignar el valor de un atributo cadena *******************************
// Proceso DFD  : 3.4
// ID Requisito : REQ12
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo.
//                - Nuevo valor que se quiere escribir en ese atributo (una cadena)

// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Da un valor al atributo (cadena) de alias indicado en la entrada 
//                 indicada (sobrescribe el valor antiguo)
// *************************************************************************************
int asignar_valor_atributo_cadena(int id_tabla, char *lexema, char *alias_at, char *valor)
{ // Asigna un valor cadena a un atributo cadena
  int resultado = -1; // error por defecto
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  int posicion;
  char* cadena_tmp; // cadena temporal
  int bytes_cadena = 0; // para la longitud de la cadena temporal,  en bytes
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;

  if (valor==NULL) // (comentando estas 5 lineas permitiriamos almacenar cadenas NULL)
  {
    ultimo_id_error = cte_error_valor_es_null;  // No se permite almacenar cadenas NULL
    return -1; // no seguir
  }

  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
	resultado = -1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ningun atributo -> fallo directo
			atributo_encontrado = -1;
			resultado = 0;
		  }
		  else // vemos si el atributo esta creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
				if (p_atributo->tipo_atributo != tipo_cadena)
				{
                  error_tipos = 0;
                }
				else
				{
				  // liberamos memoria de la anterior cadena
                  if (p_atributo->valor.cadena != NULL) // (si no era NULL)
				  {
                    free(p_atributo->valor.cadena);
				  }
				  // escribimos la nueva cadena
				  if (valor == NULL) // si el nuevo valor es null, es directo:
				  {
                    p_atributo->valor.cadena = NULL;
				  }
				  else // si no, reservamos memoria para la nueva cadena y nos guardamos una copia ahi
				  {
			        bytes_cadena = strlen(valor)+1;
	                cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
				    if (cadena_tmp == NULL) // sin memoria para mas
					{
	                  ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
                      return -1;
					}
	                strcpy(cadena_tmp,valor);
                    p_atributo->valor.cadena = cadena_tmp;
                  }
				}
				resultado = 0; // salir
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (resultado == -1));
			if (p_atributo==NULL)
			{
			  // no se encontro el atributo
		      atributo_encontrado = -1;
			  resultado = 0;
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1) && (atributo_encontrado == 0));
	  if (resultado == -1) 
	  { 
		// no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	  } 
	  else if (atributo_encontrado == -1) 
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
	  }
	  else if (error_tipos == 0)
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_atributo_no_cadena; // El atributo existe pero no es del tipo cadena
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (alias_at == NULL)
	{
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite atributo de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado; // Devolvemos el resultado
}


// ************** Consultar el valor de un atributo entero *****************************
// Proceso DFD  : 3.5
// ID Requisito : REQ13
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo.
//
// Salida       : - Valor del atributo entero.
//                - Devuelve 0 y genera un error si se produce error.
// Descripción  : Consulta el valor de un atributo entero.
// *************************************************************************************
int consultar_valor_atributo_entero(int id_tabla, char *lexema, char *alias_at)
{ // consulta un atributo entero
  int resultado = -1; // error por defecto
  int resultado_devuelto = 0;
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  int posicion;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
	resultado = -1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ningun atributo -> fallo directo
			atributo_encontrado = -1;
			resultado = 0;
		  }
		  else // vemos si el atributo esta creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
				if (p_atributo->tipo_atributo != tipo_entero)
				{
                  error_tipos = 0;
                }
				else
				{
				  resultado_devuelto = p_atributo->valor.entero; 
				}
				resultado = 0; // salir
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (resultado == -1));
			if (p_atributo==NULL)
			{
			  // no se encontro el atributo
		      atributo_encontrado = -1;
			  resultado = 0;
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1) && (atributo_encontrado == 0));
	  if (resultado == -1) 
	  { 
		// no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	  } 
	  else if (atributo_encontrado == -1) 
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
	  }
	  else if (error_tipos == 0)
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_atributo_no_entero; // El atributo existe pero no es del tipo entero
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (alias_at == NULL)
	{
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite atributo de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado_devuelto; // Devolvemos el resultado
}


// ************** Consultar el valor de un atributo cadena *****************************
// Proceso DFD  : 3.6
// ID Requisito : REQ14
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo.
//
// Salida       : - Valor del atributo cadena.
//                - Devuelve NULL y genera un error si se produce error.
// Descripción  : Consulta el valor de un atributo cadena.
// *************************************************************************************
char* consultar_valor_atributo_cadena(int id_tabla, char *lexema, char *alias_at)
{  // consulta un atributo cadena
  int resultado = -1; // error por defecto
  char* resultado_devuelto = NULL;
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  int posicion;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  puntero = tablas_de_simbolos;
  if ((puntero != NULL) && (lexema != NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 0; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  if (resultado == 0 )
  {
	resultado = -1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  // error
      ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ningun atributo -> fallo directo
			atributo_encontrado = -1;
			resultado = 0;
		  }
		  else // vemos si el atributo esta creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
				if (p_atributo->tipo_atributo != tipo_cadena)
				{
                  error_tipos = 0;
                }
				else
				{
				  resultado_devuelto = p_atributo->valor.cadena; 
				  if (resultado_devuelto == NULL)
				  {
					// En principio no deberia ocurrir, ya que
					// nuestras funciones de insercion y modificacion no lo permiten.
					// (pero puede modificarse el codigo facilmente para que lo permitan
					// si haciese falta)
                    ultimo_id_error = cte_error_atributo_cadena_es_null; 
				  }
				}
				resultado = 0; // salir
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (resultado == -1));
			if (p_atributo==NULL)
			{
			  // no se encontro el atributo
		      atributo_encontrado = -1;
			  resultado = 0;
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == -1) && (atributo_encontrado == 0));
	  if (resultado == -1) 
	  { 
		// no existia el lexema -> error
        ultimo_id_error = cte_error_no_existe_entrada; // No existe una entrada con ese lexema en esa tabla
	  } 
	  else if (atributo_encontrado == -1) 
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
	  }
	  else if (error_tipos == 0)
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_atributo_no_cadena; // El atributo existe pero no es del tipo cadena
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (alias_at == NULL)
	{
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite atributo de alias NULL
	}
	else
	{
      ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	}
  }
  return resultado_devuelto; // Devolvemos el resultado
}


// ************** Determinar la existencia de un atributo ******************************
// Proceso DFD  : 3.7
// ID Requisito : REQ17
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//                - Alias del atributo.
//
// Salida       : - Devuelve 0 si existe.
//                - Devuelve 1 si no existe.
//                - Devuelve -1 si se produce un error.
// Descripción  : Determina si un atributo (con el alias indicado) existe en una 
//                 determinada entrada (si existe la entrada y 
//                 aun su tabla no se ha destruido)
// *************************************************************************************
int existe_atributo(int id_tabla, char *lexema, char *alias_at)
{ // consulta si existe un atributo
  // -1 = error ; 0 = existe ; 1 = no existe
  int resultado = -1; 
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  int posicion;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  puntero = tablas_de_simbolos;
  // primero ver si la tabla existe
  if ((puntero != NULL) && (lexema !=NULL) && (alias_at != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	  {
        resultado = 1; // .. entonces es que existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla;
	  }
	}
	while ((puntero != NULL) && (resultado == -1));
  }
  // si la tabla existe, buscar lexema
  if (resultado == 1 )
  {
	resultado = 1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  
      resultado=1; // No existe una entrada con ese lexema en esa tabla
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{  // buscar atributo
		  if (p_entrada->primer_atributo == NULL)
		  {  // si no hay ningun atributo -> sabemos directamente que no existe
			atributo_encontrado = -1; // indicar que no se ha encontrado
			resultado = 0; // y salir del bucle
		  }
		  else // vemos si el atributo esta creado
		  {
			p_atributo=p_entrada->primer_atributo;
            do
            {
		      if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
			  {
				// encontrado 
				resultado = 0; // salir del bucle
			  }
			  else
			  {
				// avanzar en la lista de atributos
                p_atributo_previo=p_atributo;
				p_atributo=p_atributo->sig_atributo;
			  }
            }
		    while ((p_atributo !=NULL) && (resultado != 0));
			if (p_atributo==NULL)
			{
			  // no se encontro el atributo
		      atributo_encontrado = -1; // indicar que no se ha encontrado
			  resultado = 0; // // y salir del bucle
			}
		  }
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == 1) && (atributo_encontrado == 0));
      if (atributo_encontrado == -1) 
	  {
	    resultado = 1; // No existia el atributo.
	  }
	}
  }
  else
  {
	if (lexema == NULL)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_lexema_es_null; // Error: no se permite lexema de alias NULL
	}
	else if (alias_at == NULL)
	{
      resultado = -1;
      ultimo_id_error = cte_error_alias_es_null; // Error: no se permite atributo de alias NULL
	}
	else
	{
      // Ni siquiera existe la tabla indicada
	  resultado = 1 ; // lo consideraremos como -no existe-
	}
  }
  return resultado; // Devolvemos el resultado
}


// ************** Escribir el contenido actual de una tabla en un fichero **************
// Proceso DFD  : 4.1
// ID Requisito : REQ18
// Entrada      : - Identificador de la tabla de símbolos.
//                - Nombre del fichero donde se quiere escribir su contenido.

//
// Salida       : - Da una salida por fichero representando textualmente el 
//                   contenido de la tabla indicada en el momento de la llamada.
//                - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Refleja el contenido actual de una tabla de símbolos 
//                 en un fichero, de forma textual.
// *************************************************************************************
int escribir_tabla(int id_tabla, char *nombre_fichero)
{  // Crea un fichero con el contenido de la tabla indicada
	// 0 = operacion ok  ; -1 = error
   FILE* controlador = NULL; // controlador del fichero
   int resultado = -1; // resultado de la operacion.
   int i;
   int escrito_algo = -1 ; // hemos encontrado alguna entrada?
   tipo_tabla_de_simbolos *a_escribir; // tabla que vamos a escribir
   // algunos punteros auxiliares para recorrer el contenido de la tabla :
   tipo_entrada *p_entrada; 
   tipo_atributo *p_atributo;
   tipo_atributo *p_atributo_a_escribir;
   // Si la lista de tablas esta vacía, no seguir.
   // + No podemos aceptar un nombre "nulo" para el archivo
   if ((tablas_de_simbolos != NULL) && (nombre_fichero != NULL))
   {
	 a_escribir=tablas_de_simbolos; 
     do
	 {
	   if ( a_escribir->id_tabla == id_tabla )  // si encontramos la tabla con ese ID  ...
	   {
         resultado = 0; // .. entonces es que existe
	   }
	   else
	   {
         a_escribir = a_escribir->sig_tabla;
	   }
	 }
	 while ((a_escribir != NULL) && (resultado == -1));
   }
   if (resultado == 0)
   {
	 // Ya podemos escribir la tabla actual (a_escribir) ...
	 controlador = fopen(nombre_fichero, "w"); // (intentamos) abrir el fichero para escritura
	 if (controlador == NULL) 
	 {
       // si falla , no podemos seguir
	   ultimo_id_error = cte_error_creacion_fichero; // Error: El sistema no permitio crear el fichero.
	   resultado = -1;
	 }
	 else // se abrio correctamente
	 {
       fprintf(controlador,"CONTENIDO ACTUAL DE LA TABLA # %d :\n",id_tabla);
	   for (i=0; i<cte_espacio_hash; i++ ) 
	   {
         if (a_escribir->entrada[i] != NULL)
		 {
		   p_entrada=a_escribir->entrada[i];
		   do // recorremos todas las entradas enlazadas con esta entrada, escribiendolas
		   {
			 escrito_algo = 0;
			 fprintf(controlador,"---------------------------------------------------\n");
			 // escribimos lexema
		     if (p_entrada->lexema == NULL)
			 {
               fprintf(controlador,"* LEXEMA : <NULL> "); // <- no deberia ocurrir, pero por si acaso
			 }
		     else
			 {
               fprintf(controlador,"* LEXEMA : '%s' ",p_entrada->lexema); 
			 }
			 // escribimos tipo de entrada
		     if (p_entrada->tipo_entrada == NULL)
			 {
               fprintf(controlador,"(tipo de entrada no definido)\n");  // No definido
			 }
		     else
			 {
		       fprintf(controlador,"(tipo de entrada '%s')\n",p_entrada->tipo_entrada); 
			 }
			 // escribimos atributos
		       fprintf(controlador,"  ATRIBUTOS :\n"); 
		     if (p_entrada->primer_atributo==NULL )
			 {
               fprintf(controlador,"   <no se han definido atributos para este lexema>\n");
			 }
		     else
			 {
               p_atributo=p_entrada->primer_atributo;
			   do // recorremos la lista de atributos
			   {
                 p_atributo_a_escribir=p_atributo;
			     p_atributo=p_atributo->sig_atributo;
			     if (p_atributo_a_escribir->nombre_atributo == NULL) // <- no deberia ocurrir, pero por si acaso
				 {
                   fprintf(controlador,"   + <NULL> : ");
				 }               
			     else              
				 {                
			       fprintf(controlador,"   + %s : ",p_atributo_a_escribir->nombre_atributo); // escribimos nombre_atributo
				 }
			     if (p_atributo_a_escribir->tipo_atributo == tipo_cadena) // si el atributo es de tipo cadena ...
				 {
				   if (p_atributo_a_escribir->valor.cadena == NULL)
				   {
                     fprintf(controlador,"<NULL>\n");  // <- no deberia ocurrir, pero por si acaso
				   }
				   else
				   {
                     fprintf(controlador,"'%s'\n",p_atributo_a_escribir->valor.cadena); // ... escribimos la cadena
				   }
				 }
			     else // atributo numerico
				 {
                   fprintf(controlador,"%d\n",p_atributo_a_escribir->valor.entero); // ... escribimos el entero
				 }
			   }
               while (p_atributo !=NULL);
			 }
             p_entrada = p_entrada->sig_hash; // avanzamos en la lista de las colisiones hash
		   }
		   while (p_entrada != NULL);
		 }
	   }
	   fprintf(controlador,"---------------------------------------------------\n");
	   if (escrito_algo == -1)
       {
         fprintf(controlador,"  <la tabla esta vacía>\n");
	   }
	   fclose(controlador); // liberamos el controlador del fichero
	 } 
   }
   else
   {
	 if (nombre_fichero == NULL)
	 {
	   ultimo_id_error = cte_error_fichero_es_null; // Error: NULL no es un nombre valido para un fichero
	 }
	 else
     {
     ultimo_id_error = cte_error_no_existe_tabla; // Error: no existe la tabla indicada
	 }
   }
   return resultado; // Devolvemos el resultado
}


// ************** Consultar el ID del último error que se ha producido *****************
// Proceso DFD  : 5.1
// ID Requisito : REQ19
// Entrada      : <nada>
//
// Salida       : - Devuelve un entero positivo que representa el 
//                   último error que se ha producido.
//                - Devuelve 0 si de momento no han ocurrido errores.
// Descripción  : Devuelve un valor numérico que identifica el
//                 último error que se ha producido.
// *************************************************************************************
int consultar_id_ultimo_error()
{  // Devuelve el ID del ultimo error que se ha producido
   // (devuelve cero si no ha habido ningun error hasta el momento)
   int resultado;
   resultado=ultimo_id_error;
   return resultado;
}


// ************** Consultar la descripción del último error que se ha producido ********
// Proceso DFD  : 5.2
// ID Requisito : REQ20
// Entrada      : <nada>
//
// Salida       : - Devuelve una cadena indicando la descripción del 
//                   último error que se ha producido.
//                - Devuelve la cadena vacía ("") si de momento no han ocurrido errores.
// Descripción  : Devuelve una descripción textual en lenguaje natural 
//                 del último error que se ha producido.
// *************************************************************************************
char* consultar_descripcion_ultimo_error()
{  // Devuelve la descripcion del ultimo error que se ha producido
   // (devuelve "" si no ha habido ningun error hasta el momento)
   switch( ultimo_id_error )
   {
     case 0 : 
	   return "";
       break;
     case cte_error_no_existe_tabla :
	   return "No existe la tabla indicada";
	   break;
	 case cte_error_ya_existe_entrada :
	   return "Ya existe una entrada con ese lexema en esa tabla";
	   break;
	 case cte_error_no_existe_entrada :
	   return "No existe una entrada con ese lexema en esa tabla";
	   break;
	 case cte_error_ya_existe_atributo :
	   return "Ya existía el atributo. No se ha modificado";
	   break;	
	 case cte_error_no_existe_atributo :
	   return "No existe una atributo con ese nombre en la entrada indicada";
	   break;
	 case cte_error_lexema_es_null :
	   return "NULL no es una cadena válida para denominar un lexema";
	   break;
	 case cte_error_alias_es_null :
	   return "NULL no es una cadena válida para denominar un atributo";
	   break;		 
     case cte_error_fichero_es_null :
	   return "NULL no es una cadena válida para denominar un fichero";
	   break;
     case cte_error_tipo_es_null :
	   return "NULL no es una cadena válida para denominar un tipo de entrada";
	   break;
	 case cte_error_creacion_fichero :
	   return "El sistema no permitió crear el fichero";
	   break;
	 case cte_error_tipo_no_definido :
	   return "Tipo de entrada no definido";
	   break;
	 case cte_error_atributo_no_entero :
	   return "El atributo existe pero no almacena enteros";
	   break;
	 case cte_error_atributo_no_cadena :
	   return "El atributo existe pero no almacena cadenas";
	   break;
	 case cte_error_tipo_entrada_no_definido :
	   return "El tipo de entrada aún no se ha definido";
	   break;
	 case cte_error_demasiadas_tablas :
	   return "Se han creado demasiadas tablas";
	   break;
	 case cte_error_sin_memoria :
	   return "Memoria agotada";
	   break;
	 case cte_error_atributo_cadena_es_null :
	   return "El atributo cadena almacena un NULL";
	   break;
	 case cte_error_valor_es_null :
	   return "No se permite almacenar cadenas NULL";
	   break;
	 case cte_error_posicion_es_null : 
	   return "La posición indicada apunta a NULL";
	   break;
     default : 
	   return "Ocurrió un error por causas desconocidas";
	   break;
	}
}


// ************** Resetear la variable que almacena el último error producido  *********
// Proceso DFD  : 5.3
// ID Requisito : REQ32
// Entrada      : <nada>
//
// Salida       : <nada>
// Descripción  : Resetea la variable interna que indica el
//                 último error que se ha producido.
// *************************************************************************************
void resetear_ultimo_error()
{
	ultimo_id_error=0;
}


// ************** Asignar el tipo de entrada a una entrada (por posicion) **************
// Proceso DFD  : 2,2
// ID Requisito : REQ07 + REQ30
// Entrada      : - Posicion de la entrada
//                - Tipo que se le quiere dar a la entrada.
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Asigna un tipo a una entrada existente.
// *************************************************************************************
int asignar_tipo_entrada2(int posicion, char *tipo)
{ // Asigna tipo de entrada a una entrada, por posicion
  int resultado = -1; // error por defecto
  tipo_entrada *posicion_interna;
  char* cadena_tmp; // cadena temporal
  int bytes_cadena = 0; // para la longitud del lexema en bytes

  posicion_interna=(tipo_entrada*)posicion;
  if ( posicion_interna == NULL ) // si apunta a NULL , es un claro error
  {  
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
	return -1;
  }
  if (tipo == NULL)
  {
    ultimo_id_error = cte_error_tipo_es_null; // Error: se intento asignar un tipo nulo
  }
  else
  {
	resultado=0;
    // liberamos la cadena anterior
    if (posicion_interna->tipo_entrada != NULL)
	{
      free(posicion_interna->tipo_entrada);
	}
	// escribimos la nueva cadena
	bytes_cadena = strlen(tipo)+1;
	cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
    if (cadena_tmp == NULL) // sin memoria para mas
	{
      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
      return -1;
	}
	strcpy(cadena_tmp,tipo);
    posicion_interna->tipo_entrada=cadena_tmp;
  }
  return resultado; // Devolvemos el resultado
}



// ************** Consultar el tipo de entrada de una entrada (por posicion) ***********
// Proceso DFD  : 2.3
// ID Requisito : REQ08 + REQ30
// Entrada      : - Posicion de la entrada
//
// Salida       : - Devuelve tipo de la entrada si se ha definido uno y no hay errores.
//                - Devuelve NULL si se produce un error.
// Descripción  : Consulta el tipo de una entrada.
// *************************************************************************************
char* consultar_tipo_entrada2(int posicion)
{ // Consulta tipo de entrada de una entrada, por posicion
  int resultado = -1; // no existe por defecto
  char* cadena_resultado = NULL; // devolver error por defecto
  tipo_entrada* posicion_interna;

  posicion_interna=(tipo_entrada*)posicion;
  if ( posicion_interna == NULL ) // si apunta a NULL , es un claro error
  {  
    cadena_resultado = NULL;
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
  }
  else 
  {
	if (posicion_interna->tipo_entrada == NULL) // no se ha definido el tipo de entrada
	{
      cadena_resultado = NULL;
	  ultimo_id_error = cte_error_tipo_no_definido; // Error: tipo de entrada no definido
	}
	else
	{
	  cadena_resultado = posicion_interna->tipo_entrada; // devolver el tipo de entrada
	  resultado = 0;
	}
  }
  return cadena_resultado; // Devolvemos el resultado ( o NULL si algo ha fallado )
}



// ************** Crear un atributo entero en una entrada (por posicion) ***************
// Proceso DFD  : 3.1 
// ID Requisito : REQ09 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo (ejemplos: "desplazamiento", "ancho","tabla")
//                - Valor inicial para el atributo que estamos 
//                   creando (ejemplos: 16, 48, 3)
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Crea un nuevo atributo en la entrada, con el alias indicado. 
//                 Este atributo solo podrá almacenar valores enteros (tipo int de C)
// *************************************************************************************
int crear_atributo_entero2(int posicion, char *alias_at, int valor)
{ // Crea un nuevo atributo entero en la entrada de posicion indicada
  // inicializandolo con el valor indicado
  int resultado = -1; // error por defecto
  int atributo_encontrado = -1; // lo pondremos a 0 si el atributo existe
  char* cadena_tmp; // cadena temporal
  int bytes_cadena = 0; // para la longitud del lexema en bytes
  tipo_entrada* posicion_interna;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  tipo_atributo *nuevo_atributo;

  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return -1; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
    resultado = -1; // error
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
  }
  else 
  {
    resultado=-1;
    if (posicion_interna->primer_atributo == NULL)
	{  // si no hay ninguno creado aun, creamos el primer atributo:
      nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
	  if (nuevo_atributo == NULL) // sin memoria para mas
	  {
	    ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
        return -1;
	  }
	  nuevo_atributo->sig_atributo = NULL;
	  nuevo_atributo->tipo_atributo = tipo_entero;
	  nuevo_atributo->valor.entero = valor;
	  bytes_cadena = strlen(alias_at)+1;
	  cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	  if (cadena_tmp == NULL) // sin memoria para mas
	  {
	    ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
        return -1;
	  }
	  strcpy(cadena_tmp,alias_at);
	  nuevo_atributo->nombre_atributo=cadena_tmp;	
	  posicion_interna->primer_atributo=nuevo_atributo;
	  resultado = 0; // operacion realizada correctamente
	}
    else // vemos si el atributo no estaba ya creado
	{
	  p_atributo=posicion_interna->primer_atributo;
      do
      {
		if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
		{
		  atributo_encontrado=0; // estaba ya creado
		}
		else
		{
		  // avanzar en la lista de atributos
          p_atributo_previo=p_atributo;
		  p_atributo=p_atributo->sig_atributo;
		}
      }
	  while ((p_atributo !=NULL) && (atributo_encontrado == -1));
	  if (atributo_encontrado == -1 ) // si no estaba creado ...
	  { // ... creamos el nuevo atributo al final de la lista de atributos
        nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
		if (nuevo_atributo == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
          return -1;
		}
		nuevo_atributo->sig_atributo = NULL;
		nuevo_atributo->tipo_atributo = tipo_entero;
		nuevo_atributo->valor.entero = valor;
		bytes_cadena = strlen(alias_at)+1;
	    cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	    if (cadena_tmp == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
          return -1;
		}
	    strcpy(cadena_tmp,alias_at);
		nuevo_atributo->nombre_atributo=cadena_tmp;	
		p_atributo_previo->sig_atributo=nuevo_atributo;
		resultado = 0; // operacion realizada correctamente
	  }
      else // atributo_encontrado == 0
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_ya_existe_atributo; // Ya existia el atributo. No se ha modificado.
	  }
	}
  }
  return resultado; // Devolvemos el resultado
}



// ************** Crear un atributo cadena en una entrada (por posicion) ***************
// Proceso DFD  : 3.2 
// ID Requisito : REQ10 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo (ejemplos: "cadena", "real","cosa")
//                - Valor inicial para el atributo que estamos 
//                  creando (ejemplos: "algo", "12.345", "123abcd")
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Crea un nuevo atributo en la entrada, con el alias indicado. 
//                 Este atributo solo podrá almacenar cadenas (tipo char* de C)
// *************************************************************************************
int crear_atributo_cadena2(int posicion, char *alias_at, char *valor)
{ // Crea un nuevo atributo cadena en la entrada de posicion indicada
  // inicializandolo con el valor indicado
  int resultado = -1; // error por defecto
  int atributo_encontrado = -1; // lo pondremos a 0 si el atributo existe
  char* cadena_tmp; // cadena temporal (para copiarnos el alias_at)
  char* cadena_tmp2; // otra cadena temporal (para copiarnos el valor)
  int bytes_cadena = 0; // para la longitud del lexema en bytes
  tipo_entrada *posicion_interna;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  tipo_atributo *nuevo_atributo;

  if (valor==NULL) // (comentando estas 5 lineas permitiriamos almacenar cadenas NULL)
  {
    ultimo_id_error = cte_error_valor_es_null;  // No se permite almacenar cadenas NULL
    return -1; // no seguir
  }
  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return -1; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
    resultado = -1; // error
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
  }
  else 
  {
    resultado=-1;
    if (posicion_interna->primer_atributo == NULL)
	{  // si no hay ninguno creado aun, creamos el primer atributo:
      nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
	  if (nuevo_atributo == NULL) // sin memoria para mas
	  {
	    ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
        return -1;
	  }
	  nuevo_atributo->sig_atributo = NULL;
	  nuevo_atributo->tipo_atributo = tipo_cadena;
      if (valor == NULL)
	  {
        nuevo_atributo->valor.cadena = NULL;	
	  }
	  else
	  {
	    bytes_cadena = strlen(valor)+1;
	    cadena_tmp2 = (char*) malloc(bytes_cadena*sizeof(char*));
	    if (cadena_tmp2 == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
          return -1;
		}
	    strcpy(cadena_tmp2,valor);
		nuevo_atributo->valor.cadena = cadena_tmp2;	
	  }
	  bytes_cadena = strlen(alias_at)+1;
	  cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
	  if (cadena_tmp == NULL) // sin memoria para mas
	  {
	    ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada
        return -1;
	  }
	  strcpy(cadena_tmp,alias_at);
	  nuevo_atributo->nombre_atributo=cadena_tmp;	
	  posicion_interna->primer_atributo=nuevo_atributo;
	  resultado = 0; // operacion realizada correctamente
	}
	else // vemos si el atributo no estaba ya creado
	{
	  p_atributo=posicion_interna->primer_atributo;
      do
      {
	    if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
		{
		  atributo_encontrado=0; // estaba ya creado
		}
		else
		{
		  // avanzar en la lista de atributos
          p_atributo_previo=p_atributo;
		  p_atributo=p_atributo->sig_atributo;
		}
      }
	  while ((p_atributo !=NULL) && (atributo_encontrado == -1));
	  if (atributo_encontrado == -1 ) // si no estaba creado ...
	  { // ... creamos el nuevo atributo al final de la lista de atributos
        nuevo_atributo = (tipo_atributo *) malloc(sizeof(tipo_atributo));
	    if (nuevo_atributo == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
          return -1;
		}
		nuevo_atributo->sig_atributo = NULL;
		nuevo_atributo->tipo_atributo = tipo_cadena;
		if (valor == NULL)
		{
          nuevo_atributo->valor.cadena = NULL;	
		}
		else
		{
		  bytes_cadena = strlen(valor)+1;
	      cadena_tmp2 = (char*) malloc(bytes_cadena*sizeof(char*));
		  if (cadena_tmp2 == NULL) // sin memoria para mas
		  {
	        ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
            return -1;
		  }
	      strcpy(cadena_tmp2,valor);
	      nuevo_atributo->valor.cadena = cadena_tmp2;	
		}
		bytes_cadena = strlen(alias_at)+1;
	    cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
		if (cadena_tmp == NULL) // sin memoria para mas
		{
	      ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
          return -1;
		}
	    strcpy(cadena_tmp,alias_at);
		nuevo_atributo->nombre_atributo=cadena_tmp;	
		p_atributo_previo->sig_atributo=nuevo_atributo;
		resultado = 0; // operacion realizada correctamente
	  }
	  else // atributo_encontrado == 0
	  {
	    resultado = -1;
	    ultimo_id_error = cte_error_ya_existe_atributo; // Ya existia el atributo. No se ha modificado.
	  }
	}
  }
  return resultado; // Devolvemos el resultado
}



// ************** Asignar el valor de un atributo entero (por posicion) ****************
// Proceso DFD  : 3.3
// ID Requisito : REQ11 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo.
//                - Nuevo valor que se quiere escribir en ese atributo (un entero)
//
// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Da un valor al atributo (entero) de alias indicado en la 
//                 entrada indicada (sobrescribe el valor antiguo)
// *************************************************************************************
int asignar_valor_atributo_entero2(int posicion, char *alias_at, int valor)
{  // Asigna un valor entero a un atributo entero
  int resultado = -1; // error por defecto
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  tipo_entrada *posicion_interna;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;

  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return -1; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
	return -1; // no seguir
  }

  if (posicion_interna->primer_atributo == NULL)
  {  // si no hay ningun atributo -> fallo directo
    atributo_encontrado = -1;
	resultado = 0;
  }
  else // vemos si el atributo esta creado
  {
    p_atributo=posicion_interna->primer_atributo;
    do
    {
	  if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
	  {
		if (p_atributo->tipo_atributo != tipo_entero)
		{
          error_tipos = 0;
        }
		else
		{
		  p_atributo->valor.entero = valor; // escribimos el nuevo valor 
		}
		resultado = 0; // salir
	  }
	  else
	  {
		// avanzar en la lista de atributos
        p_atributo_previo=p_atributo;
		p_atributo=p_atributo->sig_atributo;
	  }
    }
    while ((p_atributo !=NULL) && (resultado == -1));
	if (p_atributo==NULL)
	{
	  // no se encontro el atributo
      atributo_encontrado = -1;
	}
	if (atributo_encontrado == -1) 
	{
	  resultado = -1;
	  ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
	}
	else if (error_tipos == 0)
	{
	  resultado = -1;
	  ultimo_id_error = cte_error_atributo_no_entero; // El atributo existe pero no es del tipo entero
	}
  }
  return resultado; // Devolvemos el resultado de la operacion
}


// ************** Asignar el valor de un atributo cadena (por posicion) ****************
// Proceso DFD  : 3.4
// ID Requisito : REQ12 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo.
//                - Nuevo valor que se quiere escribir en ese atributo (una cadena)

// Salida       : - Devuelve 0 si la operación se realizó correctamente.
//                - Devuelve -1 si se produce un error.
// Descripción  : Da un valor al atributo (cadena) de alias indicado en la entrada 
//                 indicada (sobrescribe el valor antiguo)
// *************************************************************************************
int asignar_valor_atributo_cadena2(int posicion, char *alias_at, char *valor)
{ // Asigna un valor cadena a un atributo cadena
  int resultado = -1; // error por defecto
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  char* cadena_tmp; // cadena temporal
  int bytes_cadena = 0; // para la longitud de la cadena temporal,  en bytes
  tipo_entrada *posicion_interna;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;

  if (valor==NULL) // (comentando estas 5 lineas permitiriamos almacenar cadenas NULL)
  {
    ultimo_id_error = cte_error_valor_es_null;  // No se permite almacenar cadenas NULL
    return -1; // no seguir
  }
  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return -1; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
	return -1; // no seguir
  }
  else // vemos si el atributo esta creado
  {
    p_atributo=posicion_interna->primer_atributo;
    do
	{
	  if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
	  {
	    if (p_atributo->tipo_atributo != tipo_cadena)
		{
          error_tipos = 0;
        }
		else
		{
		  // liberamos memoria de la anterior cadena
          if (p_atributo->valor.cadena != NULL) // (si no era NULL)
		  {
            free(p_atributo->valor.cadena);
		  }
		  // escribimos la nueva cadena
		  if (valor == NULL) // si el nuevo valor es null, es directo:
		  {
            p_atributo->valor.cadena = NULL;
		  }
		  else // si no, reservamos memoria para la nueva cadena y nos guardamos una copia ahi
		  {
			bytes_cadena = strlen(valor)+1;
	        cadena_tmp = (char*) malloc(bytes_cadena*sizeof(char*));
			if (cadena_tmp == NULL) // sin memoria para mas
			{
	          ultimo_id_error = cte_error_sin_memoria; // Error: memoria agotada 
              return -1;
			}
	        strcpy(cadena_tmp,valor);
            p_atributo->valor.cadena = cadena_tmp;
          }
		}
		resultado = 0; // salir
	  }
	  else
	  {
		// avanzar en la lista de atributos
        p_atributo_previo=p_atributo;
		p_atributo=p_atributo->sig_atributo;
	  }
    }
	while ((p_atributo !=NULL) && (resultado == -1));
	if (p_atributo==NULL)
	{
	  // no se encontro el atributo
      atributo_encontrado = -1;
	}
	if (atributo_encontrado == -1) 
	{
	  resultado = -1;
	  ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
	}
	else if (error_tipos == 0)
	{
	  resultado = -1;
      ultimo_id_error = cte_error_atributo_no_cadena; // El atributo existe pero no es del tipo cadena
	}
  }
  return resultado; // Devolvemos el resultado
}








// ************** Consultar el valor de un atributo entero (por posicion) **************
// Proceso DFD  : 3.5 
// ID Requisito : REQ13 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo.
//
// Salida       : - Valor del atributo entero.
//                - Devuelve 0 y genera un error si se produce error.
// Descripción  : Consulta el valor de un atributo entero.
// *************************************************************************************
int consultar_valor_atributo_entero2(int posicion, char *alias_at)
{ // consulta un atributo entero , por posicion
  int resultado = -1; // error por defecto
  int resultado_devuelto = 0;
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  tipo_entrada *posicion_interna;

  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return -1; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
	return -1; // no seguir
  }
  if (posicion_interna->primer_atributo == NULL)
  {  // si no hay ningun atributo -> fallo directo
	atributo_encontrado = -1;
	resultado = 0;
  }
  else // vemos si el atributo esta creado
  {
	p_atributo=posicion_interna->primer_atributo;
    do
    {
	  if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
	  {
		if (p_atributo->tipo_atributo != tipo_entero)
		{
          error_tipos = 0;
        }
		else
		{
		  resultado_devuelto = p_atributo->valor.entero; 
		}
		resultado = 0; // salir
	  }
	  else
	  {
		// avanzar en la lista de atributos
        p_atributo_previo=p_atributo;
		p_atributo=p_atributo->sig_atributo;
	  }
    }
	while ((p_atributo !=NULL) && (resultado == -1) && (error_tipos == -1));
	if (p_atributo==NULL)
	{
	  // no se encontro el atributo
	  atributo_encontrado = -1;
	  resultado = 0;
	}
  }
  if (atributo_encontrado == -1) 
  {
	resultado = -1;
	ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
  }
  else if (error_tipos == 0)
  {
	resultado = -1;
	ultimo_id_error = cte_error_atributo_no_entero; // El atributo existe pero no es del tipo entero
  }
  return resultado_devuelto; // Devolvemos el resultado
}


// ************** Consultar el valor de un atributo cadena (por posicion) **************
// Proceso DFD  : 3.6
// ID Requisito : REQ14 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo.
//
// Salida       : - Valor del atributo cadena.
//                - Devuelve NULL y genera un error si se produce error.
// Descripción  : Consulta el valor de un atributo cadena.
// *************************************************************************************
char* consultar_valor_atributo_cadena2(int posicion, char *alias_at)
{  // consulta un atributo cadena, por posicion
  int resultado = -1; // error por defecto
  char* resultado_devuelto = NULL;
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;
  tipo_entrada *posicion_interna;

  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return NULL; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
	return NULL; // no seguir
  }
  if (posicion_interna->primer_atributo == NULL)
  {  // si no hay ningun atributo -> fallo directo
	atributo_encontrado = -1;
	resultado = 0;
  }
  else // vemos si el atributo esta creado
  {
	p_atributo=posicion_interna->primer_atributo;
    do
    {
	  if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
	  {
	    if (p_atributo->tipo_atributo != tipo_cadena)
		{
          error_tipos = 0;
        }
		else
		{
		  resultado_devuelto = p_atributo->valor.cadena; 
		  if (resultado_devuelto == NULL)
		  {
			// En principio no deberia ocurrir, ya que
			// nuestras funciones de insercion y modificacion no lo permiten.
			// (pero puede modificarse el codigo facilmente para que lo permitan
			// si haciese falta)
            ultimo_id_error = cte_error_atributo_cadena_es_null; 
		  }
		}
		resultado = 0; // salir
	  }
	  else
	  {
		// avanzar en la lista de atributos
        p_atributo_previo=p_atributo;
		p_atributo=p_atributo->sig_atributo;
	  }
    }
	while ((p_atributo !=NULL) && (resultado == -1));
	if (p_atributo==NULL)
	{
	  // no se encontro el atributo
	  atributo_encontrado = -1;
	  resultado = 0;
	}
  }
  if (atributo_encontrado == -1) 
  {
	resultado = -1;
	ultimo_id_error = cte_error_no_existe_atributo; // No existia el atributo.
  }
  else if (error_tipos == 0)
  {
	resultado = -1;
    ultimo_id_error = cte_error_atributo_no_cadena; // El atributo existe pero no es del tipo cadena
  }
  return resultado_devuelto; // Devolvemos el resultado
}





// ************** Determinar la existencia de un atributo (por posicion) ***************
// Proceso DFD  : 3.7
// ID Requisito : REQ17 + REQ30
// Entrada      : - Posicion de la entrada
//                - Alias del atributo.
//
// Salida       : - Devuelve 0 si existe.
//                - Devuelve 1 si no existe.
//                - Devuelve -1 si se produce un error.
// Descripción  : Determina si un atributo (con el alias indicado) existe en una 
//                 determinada entrada (si existe la entrada y 
//                 aun su tabla no se ha destruido)
// *************************************************************************************
int existe_atributo2(int posicion, char *alias_at)
{ // consulta si existe un atributo, por posicion
  // -1 = error ; 0 = existe ; 1 = no existe
  int resultado = 1; 
  int atributo_encontrado = 0; // lo pondremos a -1 si el atributo no existe
  int error_tipos = -1; // lo pondremos a 0 si hay el atributo existe pero no cooncuerda el tipo
  tipo_entrada *posicion_interna;
  tipo_atributo *p_atributo;
  tipo_atributo *p_atributo_previo = NULL;

 
  if (alias_at == NULL)
  {
    ultimo_id_error = cte_error_alias_es_null; // Error: no se permite insertar un atributo de alias NULL
    return -1; // no seguir
  }
  posicion_interna = (tipo_entrada*)posicion;
  if (posicion_interna == NULL)
  {
	ultimo_id_error = cte_error_posicion_es_null; // La posicion indicada apunta a NULL
	return -1; // no seguir
  }
  if (posicion_interna->primer_atributo == NULL)
  {  // si no hay ningun atributo -> sabemos directamente que no existe
	atributo_encontrado = -1; // indicar que no se ha encontrado
	resultado = 0; // y salir del bucle
  }
  else // vemos si el atributo esta creado
  {
	p_atributo=posicion_interna->primer_atributo;
    do
    {
	  if ( strcmp(alias_at,p_atributo->nombre_atributo) == 0 )  
	  {
		// encontrado 
		resultado = 0; // salir del bucle
	  }
	  else
	  {
		// avanzar en la lista de atributos
        p_atributo_previo=p_atributo;
		p_atributo=p_atributo->sig_atributo;
	  }
    }
	while ((p_atributo !=NULL) && (resultado == 1));
	if (p_atributo==NULL)
	{
	  // no se encontro el atributo
	  atributo_encontrado = -1; // indicar que no se ha encontrado
	}
  }
  if (atributo_encontrado == -1) 
  {
	resultado = 1; // No existia el atributo.
  }
  return resultado; // Devolvemos el resultado
}




// ************** Buscar la posicion de una entrada ************************************
// Proceso DFD  : 2.4
// ID Requisito : REQ29
// Entrada      : - Identificador de la tabla de símbolos de la entrada.
//                - Lexema de la entrada.
//
// Salida       : - Devuelve la posicion de la entrada (si existe)
//                - Devuelve 0 si no existe una entrada con ese id_tabla y lexema
// Descripción  : Busca la posicion de una entrada 
//                 ( si ya fue ya creada y aun no se ha destruido su tabla )
// *************************************************************************************
int buscar_posicion_entrada(int id_tabla, char *lexema)
{ // busca un lexema en una tabla
  // 0 = no existe ; valor positivo: posicion
  int resultado = 1;  
  int posicion;
  int devolver_pos = 0;
  tipo_entrada *p_entrada;
  tipo_entrada *p_entrada_previa;
  tipo_tabla_de_simbolos *puntero;
  puntero = tablas_de_simbolos;
  // primero ver si la tabla existe
  if ((puntero != NULL) && (lexema != NULL))
  {
    do
	{
	  if ( puntero->id_tabla == id_tabla )  
	  {
        resultado = 0; // existe
	  }
	  else
      {
	    puntero = puntero->sig_tabla; // seguir buscando
	  }
	}
	while ((puntero != NULL) && (resultado == 1));
  }
  // si la tabla existe, buscar lexema
  if (resultado == 0 )
  {
	resultado = 1;
    posicion=funcion_hash(lexema); // calculamos hash para este lexema
	if ( puntero->entrada[posicion] == NULL ) // si no ningun lexema aun en esa entrada
	{  
      resultado = 1; // no existe, salir
	}
	else // vemos si existe ese lexema :
	{
	  p_entrada = puntero->entrada[posicion];
      do
	  {
	    if ( strcmp(lexema,p_entrada->lexema) == 0 )  // si encontramos el lexema ya
		{ 
		  devolver_pos=(int) p_entrada; // devolvemos la entrada convertida a entero
		  // PARA MANEJARLO COMO PUNTERO SERIA:
		  // HABRIA QUE HACER UN (tipo_entrada*) mi_posicion_obtenida;
		  // PERO NO ES RECOMENDABLE PARA EL USUARIO
	      resultado = 0; // salir
		}
		else
		{  // si no, seguir buscando en la lista de colisiones
          p_entrada_previa = p_entrada;
		  p_entrada = p_entrada->sig_hash;
		}
	  }
	  while ((p_entrada != NULL) && (resultado == 1));
	}
  }

  return devolver_pos; // Devolvemos la posicion (sera 0 si no se encontro)
}


#endif
