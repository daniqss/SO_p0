//
// Created by ranicocs on 14/09/23.
//

#ifndef NASHE_LIST_H
#define NASHE_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_COMMANDS 4096
#define CNULL NULL


typedef char *tItemC;
typedef struct tNodeC *tPosC;

struct tNodeC{
    tItemC data;
    tPosC next;
};

typedef tPosC tListC;


void CreateList(tListC* L); //Función que crea una lista vacía.

tPosC next(tPosC pos,tListC L); //Función que accede a la siguiente posición a una dada en una lista.

tPosC last(tListC L); //Función que accede a la última posición de una lista.

void InsertElement(tItemC item, tListC* L); //Función que inserta un elemento dado al final de una lista.

void RemoveElement(tPosC p, tListC* L); //Función que remueve un elemento en la posición dada de una lista.

void DisplayList(tListC* L); //Función que muestra todos los elementos de una lista.




#endif //NASHE_LIST_H
