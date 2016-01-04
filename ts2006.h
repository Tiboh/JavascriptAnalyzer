//////////////////////////////////////
// Interfaz de la tabla de símbolos //
//////////////////////////////////////

// impedir desastres si se incluye este código varias veces :
#ifndef _ts2006_h_incluido_
#define _ts2006_h_incluido_

// librerías necesarias:
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1. GESTIÓN DE TABLAS

// Crear una tabla. Obtendremos un id
int crear_tabla();

// Destruir una tabla
int destruir_tabla(int id_tabla);

// Determinar la existencia de una tabla
int existe_tabla(int id_tabla);

// 2. GESTIÓN DE ENTRADAS

// Crear una entrada en una tabla
int crear_entrada(int id_tabla, char *lexema);

// Asignar el tipo de entrada a una entrada
int asignar_tipo_entrada(int id_tabla, char *lexema, char *tipo);

// Consultar el tipo de entrada de una entrada
char* consultar_tipo_entrada(int id_tabla, char *lexema);

// Determinar si existe una entrada
int existe_entrada(int id_tabla, char *lexema);

// 3. GESTIÓN DE ATRIBUTOS

// Crear un atributo para almacenar enteros
int crear_atributo_entero(int id_tabla, char *lexema, char *alias_at, int valor);

// Crear un atributo para almacenar cadenas
int crear_atributo_cadena(int id_tabla, char *lexema, char *alias_at, char *valor);

// Asignar un nuevo valor a un atributo entero
int asignar_valor_atributo_entero(int id_tabla, char *lexema, char *alias_at, int valor);

// Asignar un nuevo valor a un atributo cadena
int asignar_valor_atributo_cadena(int id_tabla, char *lexema, char *alias_at, char *valor);

// Consultar el valor de un atributo entero
int consultar_valor_atributo_entero(int id_tabla, char *lexema, char *alias_at);

// Consultar el valor de un atributo cadena
char* consultar_valor_atributo_cadena(int id_tabla, char *lexema, char *alias_at);

// Determinar si existe un atributo
int existe_atributo(int id_tabla, char *lexema, char *alias_at);

// 4. GESTIÓN DE FICHEROS

// Escribir el contenido de una tabla en fichero
int escribir_tabla(int id_tabla, char *nombre_fichero);

// 5. GESTIÓN DE ERRORES

// Obtener el ID del ultimo error que ha ocurrido
int consultar_id_ultimo_error();

// Obtener la descripcion del ultimo error que ha ocurrido
char* consultar_descripcion_ultimo_error();

// Resetear la variable interna que almacena
// el último error que ha ocurrido
void resetear_ultimo_error();

// 6. FUNCIONES ALTERNATIVAS
// Basicamente tienen la misma funcionalidad
// pero manejan "posición"
// en lugar de "id tabla" + "lexema"
// Son mas eficientes, pero solo para usuarios avanzados.
// El usuario solo introduce y recibe enteros, pero aón así,
// ahora habrá que tener mucho más cuidado de no consultar
// posiciones de memoria incorrectas
// Además, sería extremadamente peligroso intentar liberar 
// o destruir la memoria
// de estas posiciones de forma directa.

// Asignar el tipo de entrada a una entrada (por posición)
int asignar_tipo_entrada2(int posicion, char *tipo);

// Consultar el tipo de entrada de una entrada (por posición)
char* consultar_tipo_entrada2(int posicion);

// Crear un atributo para almacenar enteros (por posición)
int crear_atributo_entero2(int posicion, char *alias_at, int valor);

// Crear un atributo para almacenar cadenas (por posición)
int crear_atributo_cadena2(int posicion, char *alias_at, char *valor);

// Asignar un nuevo valor a un atributo entero (por posición)
int asignar_valor_atributo_entero2(int posicion, char *alias_at, int valor);

// Asignar un nuevo valor a un atributo cadena (por posición)
int asignar_valor_atributo_cadena2(int posicion, char *alias_at, char *valor);

// Consultar el valor de un atributo entero (por posición)
int consultar_valor_atributo_entero2(int posicion, char *alias_at);

// Consultar el valor de un atributo cadena (por posición)
char* consultar_valor_atributo_cadena2(int posicion, char *alias_at);

// Determinar si existe un atributo (por posición)
int existe_atributo2(int posicion, char *alias_at);

// Buscar la posición de una entrada
// (devuelve la posición si existe,y sino 0)
int buscar_posicion_entrada(int id_tabla, char *lexema);


#endif

