#include "file_list.h"

bool isEmptyF(tListF L) {
    return L == FNULL;
}

void createListF(tListF *L) {
    *L = NULL;
}

tPosF firstF(tListF L) {
    return L;
} // Devolvemos la primera posición

tPosF nextF(tPosF p) {
    return p->next;
} // Devolvemos la posición siguiente

tPosF lastF(tListF L) {
    tPosF p;
    for (p = L; nextF(p) != FNULL; p = nextF(p));
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
        return false;
    }
    // Devuelve false si no hemos podido crear el nodo
    q->data.fileName = (char *) malloc(strlen(item.fileName) + 1); // Asigna memoria dinámica para fileName

    if (q->data.fileName == NULL) {
        printf("Memory allocation for fileName failed\n");
        free(q->data.fileName);
        free(q);
        return false;
    }
    strcpy(q->data.fileName, item.fileName);
    q->data.descriptor = item.descriptor;
    q->data.mode = item.mode;
    q->next = FNULL;
    // If list is empty, insert element as first element
    if (isEmptyF(*L)) {
        *L = q;
    }
    else {
        // Else, insert element at the end of the list
        p = lastF(*L);
        p->next = q;
    }
    return true;
}

void removeElementF(tPosF p, tListF *L) {
    tPosF q;
    if (p == *L)
        *L = nextF(*L);
        // Si queremos eliminar la primera posición hacemos que la lista empiece en la siguiente

    else if (p == lastF(*L)) {
        for (q = *L; (q->next != FNULL) && (q->next != p); q = nextF(q));
        q->next = FNULL;
        // Si queremos eliminar la última posición hacemos que la lista acabe en el penúltimo elemento

    }
    else {
        q = nextF(p);
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
    char *resultString = (char *) malloc(58);
    if (resultString == NULL) {
        printf("Memory allocation for modeString failed\n");
        return "Error allocating memory for modeString\n";
    } else {
        strcpy(resultString, "");
    }

    /*
    O_EXCL     Exclusivo, se usa generalmente con O_CREAT  00000010 2
    O_RDONLY   Lectura solamente 00000100 4
    O_CREAT    Representa la creación del archivo  00000001 1
    O_WRONLY   Escritura solamente  00001000 8
    O_RDWR     Lectura/Escritura  00010000 16
    O_APPEND   Adjuntar  00100000 32
    O_TRUNC    Truncar  01000000 64
    */

    if (mode & O_CREAT)
        strcat(resultString, "O_CREAT ");
    if (mode & O_EXCL)
        strcat(resultString, "O_EXCL ");
    if (mode & O_RDONLY)
        strcat(resultString, "O_RDONLY ");
    if (mode & O_WRONLY)
        strcat(resultString, "O_WRONLY ");
    if (mode & O_RDWR)
        strcat(resultString, "O_RDWR ");
    if (mode & O_APPEND)
        strcat(resultString, "O_APPEND ");
    if (mode & O_TRUNC)
        strcat(resultString, "O_TRUNC ");
    strcat(resultString, "\0");
    return resultString;
}

void displayListF(tListF L) {
    tPosF p;
    char *modeString;

    for (p = L; p != FNULL; p = nextF(p)) {
        modeString = getModeString(p->data.mode);
        printf("descriptor: %d -> %s %s\n", p->data.descriptor,p->data.fileName, modeString);
        free(modeString);
    } // Recorremos la lista y mostramos cada elemento
}

void freeListF(tListF *L) {
    while(!isEmptyF(*L)) {
        printf("Eliminando %s\n", (*L)->data.fileName);
        removeElementF(*L, L);
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