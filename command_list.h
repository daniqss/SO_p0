/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AUTORES: Santiago Garea Bestilleiro (s.garea@udc.es)
         Daniel Queijo Seoane (daniel.queijo.seoane@udc.es)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 4096
#define CNULL NULL

typedef char *tItemC;
typedef struct tNodeC *tPosC;

struct tNodeC
{
    tItemC data;
    tPosC next;
};

typedef tPosC tListC;

bool isEmptyC(tListC L); // Función que devuelve si una lista está vacía o no.

void createListC(tListC *L); // Función que crea una lista vacía.

tPosC nextC(tPosC pos); // Función que accede a la siguiente posición a una dada en una lista.

tPosC lastC(tListC L); // Función que accede a la última posición de una lista.

bool insertElementC(tItemC item, tListC *L); // Función que inserta un elemento dado al final de una lista.

void removeElementC(tPosC p, tListC *L); // Función que remueve un elemento en la posición dada de una lista.

bool getNthElement(int n, tListC L, tItemC *item);

void displayNFirstElements(int n, tListC L);

void displayListC(tListC L); // Función que muestra todos los elementos de una lista.

void freeListC(tListC *L); // Función que libera la memoria de una lista.

#endif
