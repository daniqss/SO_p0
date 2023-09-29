#include "command_list.h"

bool isEmptyC(tListC L) {
    return L == CNULL;
}

void createListC(tListC *L) {
    *L = CNULL;
}

tPosC firstC(tListC L){
    return L;
}

tPosC nextC(tPosC pos) {
    return pos->next;
} // Devolvemos la posición siguiente

tPosC lastC(tListC L) {
    tPosC p;
    for (p = L; nextC(p) != CNULL; p = nextC(p));
    return p;
} // Buscamos la última posición y la devolvemos

tPosC prev(tPosC p, tListC L){
    tPosC q;
    if(p == firstC(L))
        q = CNULL;
    else
        for(q = firstC(L); nextC(q)!=CNULL && nextC(q)!=p; q= nextC(q));
    return q;
}

bool createNodeC(tPosC *p) {
    *p = malloc(sizeof(struct tNodeC));
    return *p != CNULL;
} // Función auxiliar en la que intentamos reservar memoria para una posición y devolvemos si lo hemos logrado o no.

bool insertElementC(tItemC item, tListC *L) {
    tPosC p, q;
    if (!createNodeC(&q)) {
        free(q);
        printf("Node creation failed\n");
        return false;
    }
    // Devuelve false si no hemos podido crear el nodo
    q->data= (char *) malloc(strlen(item) + 1); // Asigna memoria dinámica para fileName
    if (q->data == NULL) {
        printf("Memory allocation for fileName failed\n");
        free(q->data);
        free(q);
        return false;
    }
    // Devuelve false si no hemos podido crear el nodo
    
    strcpy(q->data, item);
    q->next = CNULL;
    //If list is empty, insert element as first element
    
    if (isEmptyC(*L))
        *L = q;
    else{
        p = lastC(*L);
        p->next = q;
    }
    //Else, insert element at the end of the list
    return true;
    // Creamos el elemento, lo insertamos al final y devolvemos un "true", ya que hemos podido insertar el elemento.
}

void removeElementC(tPosC p, tListC *L) {
    tPosC q;
    if (p == *L) 
        // Si queremos eliminar la primera posición hacemos que la lista empiece en la siguiente
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
    }    
    free(p->data);    
    free(p); // Liberamos los datos de la posición.
    // Si queremos eliminar una posición intermedia duplicamos el siguiente nodo en esa posición y hacemos que "p" apunte al siguiente.
}


bool getNthElement(int n, tListC L, tItemC *item){
    tPosC p = NULL;
    int cnt=0;
    for (p = L; (p!= CNULL) && (cnt != n) ; p = nextC(p))
        cnt++;
    if (p!=CNULL){
        *item = p->data;
        return true;
    }
    else
        return false;
}


void displayNFirstElements(int n, tListC L) {
    tPosC p;
    int cnt=0;
    for (p = L; p!= CNULL && cnt != n ; p = nextC(p)) {
        printf("%d -> %s \n",cnt, p->data);
        cnt = cnt + 1;
    } // Recorremos la lista y mostramos cada elemento
}


void displayListC(tListC L) {
    tPosC p;
    int cnt=0;
    for (p = L; p!= CNULL; p = nextC(p)) {
        printf("%d -> %s \n",cnt, p->data);
        cnt = cnt + 1;
    } // Recorremos la lista y mostramos cada elemento
}

void freeListC(tListC *L) {
    while(!isEmptyC(*L))
        removeElementC(*L, L);
}