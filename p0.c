#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <time.h>
#include "file_list.h"
#include "command_list.h"
#include<time.h>

#define MAX 1024
#define BUFFER_SIZE 1024

void readInputs(char *cmd, char *arguments[], int *nArguments, tListC *commandList); //Lee las entradas

int chopCmd(char *cmd, char *tokens[]); //Divide a entrada de comandos

void printPrompt(); //Imprime Prompt

bool processCommand(char *arguments[MAX], int nArguments, int * recursiveCount, tListF * fileList, tListC * comandList);

void freeMemory(char *cmd, char *arguments[MAX], int nArguments, tListC *commandList, tListF *fileList);

//Comandos

void cmd_authors(char *arguments[MAX], int nArguments);

void cmd_pid(char *arguments[MAX], int nArguments);

void cmd_chdir(char *arguments[MAX], int nArguments);

void cmd_date();

void cmd_time();

void cmd_infosys(char *arguments[MAX], int nArguments);

void cmd_hist(char *arguments[MAX], int nArguments, tListC *commandList);

void cmd_command(char *arguments[MAX], int nArguments, int *recursiveCount, tListC *commandList, tListF *fileList);

void cmd_listopen( tListF *fileList);

void cmd_open(char *arguments[MAX], int nArguments, tListF *fileList);

void cmd_close (char *arguments[MAX], tListF *fileList);

void cmd_dup (char *arguments[MAX], tListF *fileList);

void cmd_help(char *arguments[MAX], int nArguments);



int main() {
    char *cmd = NULL;
    char *arguments[MAX];
    int nArguments;
    int recursiveCount = 0;

    tListC commandList;
    createListC(&commandList);

    tListF fileList;
    createListF(&fileList);

    if (!insertStdFiles(&fileList)) 
        exit(EXIT_FAILURE);

    do {
        printPrompt();
        readInputs(cmd, arguments, &nArguments, &commandList);
    } while (processCommand(arguments, nArguments, &recursiveCount , &fileList, &commandList));


    freeMemory(cmd, arguments, nArguments, &commandList, &fileList);
    return EXIT_SUCCESS;
}

void printPrompt() {
    printf("-> ");
}

void readInputs(char *cmd, char *arguments[], int *nArguments, tListC *commandList) {
    ssize_t bytesRead; // ssize_t y size_t son unsigned int
    size_t bufferSize = 0;
    char *inputCopy;

    bytesRead = getline(&cmd, &bufferSize, stdin); // getline devuelve el número de bytes escritos si no hay errores
    inputCopy = strdup(cmd);

    inputCopy[strlen(inputCopy)-1] = '\0';


    if (bytesRead == -1 || !insertElementC(inputCopy, commandList)) {
        perror("Lectura fallida");
        exit(EXIT_FAILURE);
    }
    *nArguments = chopCmd(cmd, arguments);
}

int chopCmd(char *cmd, char *tokens[]) {
    int i = 1;
    if ((tokens[0] = strtok(cmd, " \n\t")) == NULL)
        return 0;
    while ((tokens[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

bool processCommand(char *arguments[MAX], int nArguments, int *recursiveCount, tListF * fileList, tListC * commandList) {
    if(*recursiveCount > 10){
        printf("Demasiada recursión en hist \n");
        (*recursiveCount) = 0;
        return true;
    }
    else if(strcmp(arguments[0], "authors") == 0)
        cmd_authors(arguments, nArguments);
    else if (strcmp(arguments[0], "pid") == 0)
        cmd_pid(arguments, nArguments);
    else if (strcmp(arguments[0], "chdir") == 0)
        cmd_chdir(arguments, nArguments);
    else if (strcmp(arguments[0], "infosys") == 0)
        cmd_infosys(arguments, nArguments);
    else if (strcmp(arguments[0], "date") == 0)
        cmd_date();
    else if (strcmp(arguments[0],"time")==0)
        cmd_time();
    else if (strcmp(arguments[0],"listopen")==0)
        cmd_listopen(fileList);
    else if (strcmp(arguments[0],"open")==0)
        cmd_open(arguments, nArguments, fileList);
    else if (strcmp(arguments[0],"close")==0)
        cmd_close(arguments, fileList);
    else if (strcmp(arguments[0],"dup")==0)
        cmd_dup(arguments, fileList);
    else if (strcmp(arguments[0],"hist")==0)
        cmd_hist(arguments, nArguments, commandList);
    else if (strcmp(arguments[0],"command")==0)
        cmd_command(arguments, nArguments, recursiveCount ,commandList, fileList);

    else if ((strcmp(arguments[0], "quit") == 0) || (strcmp(arguments[0], "bye") == 0) || (strcmp(arguments[0], "exit") == 0))
        return false;
    else
        printf("No ejecutado: No such file or directory\n");
    return true;
}

void freeMemory(char *cmd, char *arguments[MAX], int nArguments, tListC *commandList, tListF *fileList) {
    printf("Liberando...\n");
    free(cmd);
    for (int i = 0; i < nArguments; i++) {
        free(arguments[i]);
    }

    freeListF(fileList);
    printf("Liberada lista de ficheros...\n");

    freeListC(commandList);
    printf("Liberada lista de comandos...\n");
}

/////////////////////////
// COMANDOS  ////////////
/////////////////////////

void cmd_authors(char *arguments[MAX], int nArguments) {
    char authorsNames[] = "Santiago Daniel";
    char authorsLogins[] = "s.garea@udc daniel.queijo.seoane@udc";

    switch (nArguments) {
        case 1:
            printf("%s\n%s\n", authorsNames, authorsLogins);
            break;
        case 2:
            if (strcmp(arguments[1], "-l") == 0)
                printf("%s\n", authorsLogins);
            else if (strcmp(arguments[1], "-n") == 0)
                printf("%s\n", authorsNames);
            else
                printf("Error: Unexpected argument '%s' found\n", arguments[1]);
            break;
        default:
            printf("Error: Multiple arguments\n");
            break;
    }
}

void cmd_pid(char *arguments[MAX], int nArguments) {

    switch (nArguments) {
        case 1:
            printf("Current process identifier: %d\n", getpid());
            break;
        case 2:
            if (strcmp(arguments[1], "-l") == 0)
                printf("Current process parent identifier: %d\n", getppid());
            else
                printf("Error: Unexpected argument '%s' found\n", arguments[1]);
            break;
        default:
            printf("Error: Multiple arguments\n");
            break;
    }
}

void cmd_chdir(char *arguments[MAX], int nArguments) {
    char *cwd = (char *) malloc(BUFFER_SIZE);
    if (cwd == NULL) {
        perror("Memory allocation error");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    switch (nArguments) {
        case 1:
            if (getcwd(cwd, BUFFER_SIZE) == NULL) {
                perror("getcwd() error");
                free(cwd);
                exit(EXIT_FAILURE);
            } else {
                printf("Current working directory: %s\n", cwd);
                free(cwd);
            }
            break;

        case 2:
            chdir(arguments[1]);
            free(cwd);
            break;

        default:
            printf("Error: Multiple arguments\n");
            free(cwd);
            break;
    }
}

struct tm getDateTime(){
    time_t actual;
    struct tm *organized;
    time(&actual); //Conseguimos el tiempo actual
    organized = localtime(&actual); //Descomponemos el tiempo en valores (horas, minutos, segundos, años, dia,...)
    return *organized;
} //Función auxiliar que devuelve un struct con la hora y fechas actuales

void cmd_date(){
    struct tm organizedData;
    organizedData = getDateTime();
    printf("%02d/%02d/%04d\n", organizedData.tm_mday,organizedData.tm_mon, organizedData.tm_year);
}

void cmd_time(){
    struct tm organizedTime;
    organizedTime = getDateTime();
    printf("%02d:%02d:%02d\n", organizedTime.tm_hour,organizedTime.tm_min, organizedTime.tm_sec);
}

bool esEnteroPositivo(const char *cadena,int *numero){
    if (*cadena == '\0')
        return false;
    char *temp;
    *numero = (int)strtol(cadena,  &temp, 10);
    if (temp != NULL && numero>=0)
        return true;
    else
        return false;
}

void cmd_hist(char *arguments[MAX], int nArguments, tListC *commandList){
    size_t len = sizeof(arguments[1])-1;
    char *primerChar;
    char restoDeLaCadena[len];
    int numero;
    switch (nArguments) {
        case 1: //Mostrar historial
            displayListC(*commandList);
            break;
        case 2:
            primerChar = &arguments[1][0];
            if(*primerChar == '-'){
                strcpy(restoDeLaCadena, arguments[1] + 1);
                if (restoDeLaCadena[len - 1] == '\n') {
                    restoDeLaCadena[len - 1] = '\0'; // Reemplaza el '\n' con '\0'
                }
                if(strcmp(restoDeLaCadena, "c")==0){
                    freeListC(commandList); //Borrar historial de comandos
                }
                else if (esEnteroPositivo(restoDeLaCadena, &numero)){
                    displayNFirstElements(numero,*commandList); //Mostrar los N primeros elementos si se introduce un número natural positivo
                }
                else{
                    printf("Error: Unexpected argument '%s' found\n", arguments[1]);
                }
            }
            else{
                printf("Error: Unexpected argument '%s' found\n", arguments[1]);
            }
            break;
        default:
            printf("Error: Multiple arguments\n");
            break;
    }
}

void cmd_command(char *arguments[MAX], int nArguments, int *recursiveCount, tListC *commandList, tListF *fileList){
    int numero, nArgumentsHist;
    tItemC command;
    char *argumentsHist[MAX];
    switch (nArguments) {
        case 1:
            printf("Necesitas especificar el número del comando que deseas repetir \n");
            displayListC(*commandList);
            break;
        case 2:
            if(esEnteroPositivo(arguments[1],&numero)){
                command = strdup(getNthElement(numero,*commandList));
                printf("Ejecutando hist (%d): %s \n",numero,command);
                nArgumentsHist = chopCmd(command, argumentsHist);
                (*recursiveCount)++;
                processCommand(argumentsHist, nArgumentsHist, recursiveCount, fileList, commandList);
                free(command);
            }
            else{
                printf("Error: Unexpected argument '%s' found\n", arguments[1]);
            }
            break;
        default:
            printf("Error: Multiple arguments\n");
            break;
    }
}

void cmd_infosys(char *arguments[MAX], int nArguments) {
    struct utsname machineInfo;

    if (uname(&machineInfo) == -1) {
        perror("ename error");
        exit(EXIT_FAILURE);
    }
    printf("%s (%s), OS: %s-%s-%s\n",machineInfo.nodename,machineInfo.machine,machineInfo.sysname,machineInfo.release, machineInfo.version);
}

void cmd_listopen(tListF *fileList){
    // Listar archivos abiertos
    if (isEmptyF(*fileList))
        printf("No hay archivos abiertos");
    else 
        displayListF(*fileList);
}

void cmd_open(char *arguments[MAX], int nArguments, tListF *fileList) {
    int i, fileDescriptor, mode = 0;

    if (nArguments == 1)
        // Listar archivos abiertos
        cmd_listopen(fileList);
    else {

        for (i = 2; arguments[i] != NULL; i++) {
            if (!strcmp(arguments[i], "cr"))
                mode |= O_CREAT;
            else if (!strcmp(arguments[i], "ex"))
                mode |= O_EXCL;
            else if (!strcmp(arguments[i], "ro"))
                mode |= O_RDONLY;
            else if (!strcmp(arguments[i], "wo"))
                mode |= O_WRONLY;
            else if (!strcmp(arguments[i], "rw"))
                mode |= O_RDWR;
            else if (!strcmp(arguments[i], "ap"))
                mode |= O_APPEND;
            else if (!strcmp(arguments[i], "tr"))
                mode |= O_TRUNC;
            else
                break;
        }
        if ((fileDescriptor = open(arguments[1], mode, 0777)) == -1)
            perror("Imposible abrir fichero: No such file or directory");
        else {
            insertElementF((tItemF) {arguments[1], fileDescriptor, mode}, fileList);

            printf("Añadida entrada %d a la tabla de ficheros abiertos\n", fileDescriptor);
        }
    }
}

void cmd_close (char *arguments[MAX], tListF *fileList) {
    int fileDescriptor;

    if (arguments[1]==NULL || (fileDescriptor=atoi(arguments[1]))<0) {
        cmd_listopen(fileList);
        return;
    }
    if (close(fileDescriptor)==-1)
        perror("Imposible cerrar descriptor");
    else {
        removeElementF(findElementF(fileDescriptor, *fileList), fileList);
    }
}

void cmd_dup (char *arguments[MAX], tListF *fileList) {
    int fileDescriptor, newFileDescriptor;
    tPosF file;
    char aux[MAX];

    if (arguments[1]==NULL || (fileDescriptor=atoi(arguments[1]))<0) {
        cmd_listopen(fileList);
        return;
    }
    if ((file = findElementF(fileDescriptor, *fileList)) == NULL) {
        printf("Imposible duplicar descriptor: Bad file descriptor\n");
        return;
    }
    fileDescriptor = file->data.descriptor;
    newFileDescriptor = dup(fileDescriptor);

    sprintf (aux,"dup %d (%s)",fileDescriptor, file->data.fileName);
    insertElementF((tItemF) {aux, newFileDescriptor, file->data.mode}, fileList);
}



