#include "file_list.h"

bool isEmptyF(tListF L) {
    return L == FNULL;
}

void createListF(tListF *L) {
    L = FNULL;
}


tPosF nextF(tPosF pos, tListF L) {
    return pos->next;
} // Devolvemos la posición siguiente

tPosF lastF(tListF L) {
    tPosF p;
    for (p = L; nextF(p, L) != FNULL; p = nextF(p, L));
    return p;
} // Buscamos la última posición y la devolvemos

bool createNodeF(tPosF *p) {
    *p = malloc(sizeof(struct tNodeF));
    return *p != FNULL;
} // Función auxiliar en la que intentamos reservar memoria para una posición y devolvemos si lo hemos logrado o no.

bool insertElementF(tItemF item, tListF *L){
    tPosF p, q;
    if (!createNodeF(&q)) {
        free(q);
        printf("Node creation failed\n");
        return false;
    }

    // Devuelve false si no hemos podido crear el nodo
    q->data.fileName = (char *) malloc(strlen(item.fileName) + 1); // Asigna memoria dinámica para fileName
    if (q->data.fileName == NULL) {
        printf("Memory allocation for fileName failed\n");
        free(q);
        return false;
    }
    strcpy(q->data.fileName, item.fileName);
    q->data.descriptor = item.descriptor;
    q->data.mode = item.mode;
    q->next = FNULL;

    // If list is empty, insert element as first element
    if (isEmptyF(*L))
        *L = q;
    else {
        // Else, insert element at the end of the list
        p = lastF(*L);
        p->next = q;
    }
    return true;
}

void removeElementF(tPosF p, tListF *L) {
    tPosF q;
    if (p == *L) // Si queremos eliminar la primera posición hacemos que la lista empiece en la siguiente
        *L = nextF(*L, *L);
    else if (p == lastF(*L)) {
        for (q = *L; (q->next != FNULL) && (q->next != p); q = nextF(q, *L));
        q->next = FNULL;
    } // Si queremos eliminar la última posición hacemos que la lista acabe en el penúltimo elemento
    else {
        q = nextF(p, *L);
        strcpy(p->data.fileName, q->data.fileName);
        p->data.descriptor = q->data.descriptor;
        p->data.mode = q->data.mode;

        p->next = q->next;
        p = q;
    }
    free(p->data.fileName);
    free(p);
}

tPosF findElementF(int fileDescriptor, tListF L) {
    tPosF p;

    for (p = L; (p != FNULL) && (p->data.descriptor != fileDescriptor); p = p->next);

    return p;
}

char *getModeString (int mode) {
    switch (mode) {
    case O_CREAT:
        return "O_CREAT";
        break;
    case O_EXCL:
        return "O_EXCL";
        break;
    case O_RDONLY:
        return "O_RDONLY";
        break;
    case O_WRONLY:
        return "O_WRONLY";
        break;
    case O_RDWR:
        return "O_RDWR";
        break;
    case O_APPEND:
        return "O_APPEND";
        break;
    case O_TRUNC:
        return "O_TRUNC";
        break;
    default:
        return "Error";
        break;
    }
}

void displayListF(tListF L) {
    tPosF p;
    for (p = L; p != FNULL; p = nextF(p, L)) {
        printf("descriptor: %d -> %s %s\n", p->data.descriptor,p->data.fileName, getModeString(p->data.mode));
    } // Recorremos la lista y mostramos cada elemento
}

void freeListF(tListF *L) {
    tPosF p;

    if(!isEmptyF(*L)) { 
        for(p=(*L); p!=NULL; p=nextF(p,*L)) { // Iteramos por todos los jurados de la lista.
            printf("liberando %s\n", p->data.fileName);
            removeElementF(p, L);
        }
    }
}

bool insertStdFiles (tListF *L) {
    if (!insertElementF((tItemF) {"stdin", 0, O_RDWR}, L)) {
        perror("Error al insertar stdin");
        return false;
    }
    if (!insertElementF((tItemF) {"stdout", 1, O_RDWR}, L)) {
        perror("Error al insertar stdout");
        return false;
    }
    if (!insertElementF((tItemF) {"stderr", 2, O_RDWR}, L)) {
        perror("Error al insertar stderr");
        return false;
    }
    return true;
}