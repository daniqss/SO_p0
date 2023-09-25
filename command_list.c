#include "command_list.h"

bool isEmptyC(tListC L) {
    return L == CNULL;
}

void createListC(tListC *L) {
    L = CNULL;
}

tPosC nextC(tPosC pos) {
    return pos->next;
} // Devolvemos la posición siguiente

tPosC lastC(tListC L) {
    tPosC p;
    for (p = L; nextC(p) != CNULL; p = nextC(p));
    return p;
} // Buscamos la última posición y la devolvemos

bool createNodeC(tPosC *p) {
    *p = malloc(sizeof(struct tNodeC));
    return *p != CNULL;
} // Función auxiliar en la que intentamos reservar memoria para una posición y devolvemos si lo hemos logrado o no.

bool insertElementC(tItemC item, tListC *L) {
    tPosC p, q;
    if (!createNodeC(&q)) {
        printf("Node creation failed\n");
        return false;
    }

        // Devuelve false si no hemos podido crear el nodo
    else {
        q->data = item;
        q->next = CNULL;

        //If list is empty, insert element as first element
        if (isEmptyC(*L)) *L = q;
        //Else, insert element at the end of the list
        p = lastC(*L);
        p->next = q;

        return true;
    } // Creamos el elemento, lo insertamos al final y devolvemos un "true", ya que hemos podido insertar el elemento.
}

void removeElementC(tPosC p, tListC *L) {
    tPosC q;
    if (p == *L) // Si queremos eliminar la primera posición hacemos que la lista empiece en la siguiente
        *L = nextC(*L);
    else if (p == lastC(*L)) {
        for (q = *L; (q->next != CNULL) && (q->next != p); q = nextC(q));
        q->next = CNULL;
    } // Si queremos eliminar la última posición hacemos que la lista acabe en el penúltimo elemento
    else {
        q = nextC(p);
        p->data = q->data;
        p->next = q->next;
        p = q;
    }        // Si queremos eliminar una posición intermedia duplicamos el siguiente nodo en esa posición y hacemos que "p" apunte al siguiente.
    free(p); // Liberamos los datos de la posición.
}

void displayListC(tListC L) {
    tPosC p;
    for (p = L; p != CNULL; p = nextC(p)) {
        printf(" %s \n", p->data);
    } // Recorremos la lista y mostramos cada elemento
}

void freeListC(tListC *L) {
    tPosC p, q;
    int i = 0;
    printf("Liberando lista de comandos\n");

    p = *L;

    while (!isEmptyC(p)) {
        if (i == 0) printf("entrando al buqle n%d\n", i);
        printf("%d\n", i);

        q = nextC(p);
        i++;

        removeElementC(p, L);
        p = q;
    }
}