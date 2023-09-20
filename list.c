//
// Created by ranicocs on 14/09/23.
//

#include "list.h"

void CreateList(tListC* L) {
    L = CNULL;
}
tPos next(tPos pos,tListC L) {
    return pos->next;
} //Devolvemos la posición siguiente

tPos last(tListC L) {
    tPos p;
    for(p=L; next(p,L)!=CNULL; p = next(p,L));
    return p;
}//Buscamos la última posición y la devolvemos

bool createNodeC(tPos *p) {
    *p= malloc(sizeof(struct tNode));
    return *p!=CNULL;
} //Función auxiliar en la que intentamos reservar memoria para una posición y devolvemos si lo hemos logrado o no.

bool InsertElement(tItem item, tListC* L) {
    tPos p,q;
    if(!createNodeC(&q))
        return false;
    //Devuelve false si no hemos podido crear el nodo
    else{
        q->data = item;
        q->next = CNULL;
        p = last(*L);
        p->next = q;
        return true;
    }//Creamos el elemento, lo insertamos al final y devolvemos un "true", ya que hemos podido insertar el elemento.
}

void RemoveElement(tPos p, tListC *L) {
    tPos q;
    if(p == *L) //Si queremos eliminar la primera posición hacemos que la lista empiece en la siguiente
        *L= next(*L,*L);
    else if (p == last(*L)){
        for (q = *L; (q->next != CNULL)&&(q->next != p)  ; q = next(q,*L));
        q->next=CNULL;
    }//Si queremos eliminar la última posición hacemos que la lista acabe en el penúltimo elemento
    else{
        q = next(p,*L);
        p->data = q->data;
        p->next = q->next;
        p = q;
    }//Si queremos eliminar una posición intermedia duplicamos el siguiente nodo en esa posición y hacemos que "p" apunte al siguiente.
    free(p); //Liberamos los datos de la posición.
}

void DisplayList(tListC L) {
    tPos p;
    for ( p = L; p != CNULL ; p = next(p,L)) {
        printf(" %s \n",p->data);
    } //Recorremos la lista y mostramos cada elemento
}