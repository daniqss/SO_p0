//
// Created by ranicocs on 14/09/23.
//

#ifndef FILE_LIST_H
#define FILE_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAX_FILES 4096
#define FNULL NULL

typedef struct{
    char *fileName; // Puntero al nombre del archivo
    int descriptor; // Descriptor de archivo (file descriptor)
    int mode;       // Modo de apertura del archivo
} tItemF;

typedef struct tNodeF *tPosF;

struct tNodeF
{
    tItemF data;
    tPosF next;
};

typedef tPosF tListF;

bool isEmptyF(tListF L); // Función que devuelve si una lista está vacía o no.

void createListF(tListF *L); // Función que crea una lista vacía.

bool insertStdFiles (tListF *L);        // Inserta los ficheros estándar

tPosF nextF(tPosF pos, tListF L); // Función que accede a la siguiente posición a una dada en una lista.

tPosF lastF(tListF L); // Función que accede a la última posición de una lista.

bool insertElementF(tItemF item, tListF *L); // Función que inserta un elemento dado al final de una lista.

void removeElementF(tPosF p, tListF *L); // Función que remueve un elemento en la posición dada de una lista.

tPosF findElementF(int fileDescriptor, tListF L);

void displayListF(tListF L); // Función que muestra todos los elementos de una lista.

void freeListF(tListF *L); // Función que libera la memoria de una lista.

#endif
