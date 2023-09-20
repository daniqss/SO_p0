//
// Created by ranicocs on 14/09/23.
//

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

void CreateListC(tListC *L); // Función que crea una lista vacía.

tPosC nextC(tPosC pos, tListC L); // Función que accede a la siguiente posición a una dada en una lista.

tPosC lastC(tListC L); // Función que accede a la última posición de una lista.

bool InsertElementC(tItemC item, tListC *L); // Función que inserta un elemento dado al final de una lista.

void RemoveElementC(tPosC p, tListC *L); // Función que remueve un elemento en la posición dada de una lista.

void DisplayListC(tListC L); // Función que muestra todos los elementos de una lista.

#endif
