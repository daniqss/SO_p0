#include "file_list.h"

void createListF(tListF* L){
    L = FNULL;
}

tPosF nextF(tPosF pos,tListF L){
    return pos->next;
} //Devolvemos la posición siguiente

tPosF lastFF(tListF L){
    tPosF p;
    for(p=L; nextF(p,L)!=FNULL; p = nextF(p,L));
    return p;
}//Buscamos la última posición y la devolvemos

bool createNodeF(tPosF *p){
    *p= malloc(sizeof(struct tNodeF));
    return *p!=FNULL;
} //Función auxiliar en la que intentamos reservar memoria para una posición y devolvemos si lo hemos logrado o no.

bool insertElementF(tItemF item, tListF* L) {
    tPosF p,q;
    if(!createNodeC(&q)) {
        free(q);
        printf("Node creation failed\n");
        return false;
    }
    
    //Devuelve false si no hemos podido crear el nodo
    
    else {
        q->data.fileName = (char *)malloc(strlen(item.fileName) + 1); // Asigna memoria dinámica para fileName
        if (q->data.fileName == NULL) {
            printf("Memory allocation for fileName failed\n");
            free(q);
            return false;
        }
        strcpy(q->data.fileName, item.fileName);
        q->data.descriptor = item.descriptor;
        q->data.mode = item.mode;
        q->next = FNULL;
        p = lastF(*L);
        p->next = q;
        return true;
    }//Creamos el elemento, lo insertamos al final y devolvemos un "true", ya que hemos podido insertar el elemento.
}

void removeElementF(tPosF p, tListF *L) {
    tPosF q;
    if(p == *L) //Si queremos eliminar la primera posición hacemos que la lista empiece en la siguiente
        *L= nextF(*L,*L);
    else if (p == lastF(*L)){
        for (q = *L; (q->next != FNULL)&&(q->next != p)  ; q = nextF(q,*L));
        q->next=FNULL;
    }//Si queremos eliminar la última posición hacemos que la lista acabe en el penúltimo elemento
    else{
        q = nextF(p,*L);
        p->data = q->data;
        p->next = q->next;
        p = q;
    }//Si queremos eliminar una posición intermedia duplicamos el siguiente nodo en esa posición y hacemos que "p" apunte al siguiente.
    free(p); //Liberamos los datos de la posición.
}

void DisplayList(tListF L){
    tPosF p;
    for ( p = L; p != FNULL ; p = nextF(p,L)) {
        printf(" %s \n",p->data);
    } //Recorremos la lista y mostramos cada elemento
}