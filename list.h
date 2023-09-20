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

typedef char *tItem;
typedef struct tNode *tPos;

struct tNode {
    tItem data;
    tPos next;
};

typedef tPos tListC;


void CreateList(tListC *L); // Función que crea una lista vacía.

tPos next(tPos pos, tListC L); // Función que accede a la siguiente posición a una dada en una lista.

tPos last(tListC L); // Función que accede a la última posición de una lista.

bool InsertElement(tItem item, tListC *L); // Función que inserta un elemento dado al final de una lista.

void RemoveElement(tPos p, tListC *L); // Función que remueve un elemento en la posición dada de una lista.

void DisplayList(tListC L); // Función que muestra todos los elementos de una lista.

#endif
