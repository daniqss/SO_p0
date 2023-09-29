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

#define MAX_ARGUMENTS 1024
#define MAX_BUFFER 1024

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DECLARACIÓN~~~DE~~~FUNCIONES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool readInputs(char cmd[MAX_BUFFER], char *arguments[], int *nArguments, tListC *commandList);
//Lee los inputs del usuarios

int chopCmd(char cmd[MAX_BUFFER], char *tokens[]);
//Divide el input en palabras

void printPrompt();
//Imprime Prompt (Salida por defecto)

bool processCommand(char *arguments[MAX_ARGUMENTS], int nArguments, int * recursiveCount, tListF * fileList, tListC * comandList);
//Identifica a que comando se refiere la entrada y lo llama.

void cmd_authors(char *arguments[MAX_ARGUMENTS], int nArguments);
/*
 * Muestra el nombre y los logins los autores del programa
 * Adicionalmente, si agregamos a la entrada un:
 *  -l Muestra solo los logins
 *  -n Muestra solo los nombres
 */
void cmd_pid(char *arguments[MAX_ARGUMENTS], int nArguments);
/*
 * Muestra el pid del proceso que se está ejecutando en el shell:
 * Adicionalmente, si agregamos a la entrada un:
 *  -p Muestra el pid del proceso padre del proceso
 */
void cmd_chdir(char *arguments[MAX_ARGUMENTS], int nArguments);
/*
 * Cambia la dirección de trabajo del shell por la que se le indique.
 * Adicionalmente, si no se le indica ningunha dirección muestra la dirección actual.
 */
void cmd_date();
/*
 * Muestra la fecha actual.
 */
void cmd_time();
/*
 * Muestra la hora actual.
 */
void cmd_hist(char *arguments[MAX_ARGUMENTS], int nArguments, tListC *commandList);
/*
 * Muestra todos los comandos ejecutados en el programa por orden (historial de comando)
 * Adicionalmente, si agregamos a la entrada un:
 *  -c Borra el historial en vez de mostrarlo
 *  -N (siendo N un número natural) muestra los N primeros comandos.
 */
void cmd_command(char *arguments[MAX_ARGUMENTS], int nArguments, int *recursiveCount, tListC *commandList, tListF *fileList);
/*
 * Ejecuta el comando número N (siendo N un número natural)
 *
 */
void cmd_open(char *arguments[MAX_ARGUMENTS], int nArguments, tListF *fileList);
/*
 * Permite abrir un archivo y añadirlo a la lista de archivos abiertos
 * Le debemos pasar el nombre del fichero y el/los modos en los que queremos abrirlo
 * Adicionalmente, si no le pasamos ningún parámetro muestra la lista de archivos abiertos
 */
void cmd_close (char *arguments[MAX_ARGUMENTS], tListF *fileList);
/* 
 * Permite cerrar un fichero abierto y eliminarlo de la lista de archivos abiertos
 * Debemos pasarle como parámetro el descriptor del fichero que queremos cerrar
 * Adicionalmente, si no le pasamos ningún parámetro muestra la lista de archivos abiertos
 */
void cmd_dup (char *arguments[MAX_ARGUMENTS], tListF *fileList);
/*
 * Permite duplicar el descriptor de fichero abierto y añadirlo a la lista de archivos abiertos
 * Le debemos pasar el descriptor del fichero que queremos duplicar
 * Adicionalmente, si no le pasamos ningún parámetro muestra la lista de archivos abiertos
 */
void cmd_listopen( tListF *fileList);
/*
 * Muestra la lista de archivos abiertos
 */
void cmd_infosys(char *arguments[MAX_ARGUMENTS], int nArguments);
/*
 * Muestra la información de la máquina que ejecuta el shell
 */
void cmd_help(char *arguments[MAX_ARGUMENTS], int nArguments);
/*
 * Muestra la lista de comandos disponibles del shell
 * Adicionalmente, si agregamos a la entrada un comando muestra una descripción sobre lo que hace ese comando
 */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PROGRAMA~~~~PRINCIPAL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



int main() {
    char cmd[MAX_BUFFER];
    char *arguments[MAX_ARGUMENTS];
    int nArguments;
    int recursiveCount = 0;
    bool quit;

    tListC commandList;
    createListC(&commandList);

    tListF fileList;
    createListF(&fileList);

    if (!insertStdFiles(&fileList)) 
        exit(EXIT_FAILURE);

    do {
        printPrompt();
        quit = readInputs(cmd, arguments, &nArguments, &commandList);
        if (quit)
            quit = processCommand(arguments, nArguments, &recursiveCount , &fileList, &commandList);
    } while (quit);

    freeListC(&commandList);
    freeListF(&fileList);
    return EXIT_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEFINICIÓN~~~DE~~~FUNCIONES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void printPrompt() {
    char *userName = getenv("USER"); //Conseguimos el usuario del dispositvo
    char hostName[250];
    if (userName == NULL || gethostname(hostName, sizeof(hostName)) != 0)
        printf("\033[1;31m[?]>> \033[0m"); //Si no obtenemos el usuario y nombre del dispositivo mostramos un interrogante por defecto
    else {
        printf("\033[1;32m");
        printf("[%s@%s]>> \033[0m", userName, hostName); // Formato : [usuario@nombreDelDispositivo]>>
    }
}


bool readInputs(char cmd[MAX_BUFFER], char *arguments[], int *nArguments, tListC *commandList) {
    size_t stringSize = 0;
    bool aux = true;
    int actualCharacter;
    char *inputCopy;
    

    while (aux) {
        actualCharacter = getchar();
        if (actualCharacter == '\n' || actualCharacter == EOF) {
            cmd[stringSize] = '\0';
            aux = false;
        } else {
            cmd[stringSize] = (char)actualCharacter;
            stringSize++;

            if (stringSize == MAX_BUFFER - 1) {
                printf("\n\033[31mError:\033[0m buffer size exceeded, extra characters will be ignore\n");
                printPrompt();
                while ((actualCharacter = getchar()) != '\n' && actualCharacter != EOF)
                aux = false; // Terminar la lectura
            }
        }
    }

    inputCopy = (char *) malloc(strlen(cmd) + 1);
    strcpy(inputCopy, cmd);

    if (!insertElementC(inputCopy, commandList)) {
        perror("\033[31mError:\033[0m read failed");
        free(inputCopy);
        exit(EXIT_FAILURE);
    }
    free(inputCopy);

    *nArguments = chopCmd(cmd, arguments);
    return true;
}

int chopCmd(char cmd[MAX_BUFFER], char *tokens[]) {
    int i = 1;
    if ((tokens[0] = strtok(cmd, " \n\t")) == NULL)
        return 0;
    while ((tokens[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

bool processCommand(char *arguments[MAX_ARGUMENTS], int nArguments, int *recursiveCount, tListF * fileList, tListC * commandList) {
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
    else if (strcmp(arguments[0],"help")==0)
        cmd_help(arguments,nArguments);

    else if ((strcmp(arguments[0], "quit") == 0) || (strcmp(arguments[0], "bye") == 0) || (strcmp(arguments[0], "exit") == 0))
        return false;
    else
        printf("\033[31mNo ejecutado: \033[0mNo such file or directory\n");
    return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COMANDOS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


void cmd_authors(char *arguments[MAX_ARGUMENTS], int nArguments) {
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
                printf("\033[31mError:\033[0m Unexpected argument \033[33m'%s'\033[0m found\n", arguments[1]);
            break;
        default:
            printf("\033[31mError:\033[0m Multiple arguments\n");
            break;
    }
}

void cmd_pid(char *arguments[MAX_ARGUMENTS], int nArguments) {

    switch (nArguments) {
        case 1:
            printf("Current process identifier: %d\n", getpid());
            break;
        case 2:
            if (strcmp(arguments[1], "-l") == 0)
                printf("Current process parent identifier: %d\n", getppid());
            else
                printf("\033[31mError:\033[0m Unexpected argument '%s' found\n", arguments[1]);
            break;
        default:
            printf("\033[31mError:\033[0m Multiple arguments\n");
            break;
    }
}

void cmd_chdir(char *arguments[MAX_ARGUMENTS], int nArguments) {
    char *cwd = (char *) malloc(MAX_BUFFER);
    if (cwd == NULL) {
        perror("\033[31mError:\033[0m Memory allocation error");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    switch (nArguments) {
        case 1:
            if (getcwd(cwd, MAX_BUFFER) == NULL) {
                perror("\033[31mError:\033[0m getcwd() error");
                free(cwd);
                exit(EXIT_FAILURE);
            } else {
                printf("Current working directory:\033[34m %s\n\033[0m", cwd);
                free(cwd);
            }
            break;

        case 2:
            chdir(arguments[1]);
            free(cwd);
            break;

        default:
            printf("\033[31mError:\033[0m Multiple arguments\n");
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
    *numero = (int)strtol(cadena,  &temp, 10); //Guarda los números y en temp el resto de caracteres
    if (*temp == '\0' && *numero >= 0) //Si no hay otros caracteres o el número es mayor igual que cero devolvemos true.
        return true;
    else
        return false;
}//Función auxiliar que comprueba si una cadena está compuesta solo de dígitos (por lo tanto un número positivo).

void cmd_hist(char *arguments[MAX_ARGUMENTS], int nArguments, tListC *commandList){
    char *primerChar;
    int numero;
    char *restoDeLaCadena;
    int length;

    switch (nArguments) {
        case 1: // Mostrar historial
            displayListC(*commandList);
            break;
        case 2:
            primerChar = &arguments[1][0];
            if (*primerChar == '-') {
                restoDeLaCadena = arguments[1] + 1;
                length = strlen(restoDeLaCadena) - 1;
                if (restoDeLaCadena[length] == '\n') {
                    restoDeLaCadena[length] = '\0'; // Reemplaza el '\n' con '\0'
                }
                if (strcmp(restoDeLaCadena, "c") == 0) {
                    freeListC(commandList); // Borrar historial de comandos
                } else if (esEnteroPositivo(restoDeLaCadena, &numero)) {
                    displayNFirstElements(numero, *commandList); // Mostrar los N primeros elementos si se introduce un número natural positivo
                } else {
                    printf("\033[31mError:\033[0m Unexpected argument \033[33m'%s'\033[0m found\n", arguments[1]);
                }
            } else {
                printf("\033[31mError:\033[0m Unexpected argument \033[33m'%s'\033[0m found\n", arguments[1]);
            }
            break;
        default:
            printf("\033[31mError:\033[0m Multiple arguments\n");
            break;
    }
}


void cmd_command(char *arguments[MAX_ARGUMENTS], int nArguments, int *recursiveCount, tListC *commandList, tListF *fileList){
    int numero, nArgumentsHist;
    tItemC command = NULL;
    char *argumentsHist[MAX_ARGUMENTS];
    switch (nArguments) {
        case 1:
            printf("Necesitas especificar el número del comando que deseas repetir \n");
            displayListC(*commandList);
            break;
        case 2:
            if(esEnteroPositivo(arguments[1],&numero)){
                if(!getNthElement(numero,*commandList,&command)) //Comprueba si existe el Nth elemento en la lista
                    printf("No hay elemento %d en el historico \n",numero);
                else{
                    command = strdup(command); //Duplicamos la lista, para evitar los problemas de memoria
                    printf("Ejecutando hist (%d): %s \n",numero,command);
                    nArgumentsHist = chopCmd(command, argumentsHist); //Conseguimos el número de argumentos
                    (*recursiveCount)++; //Aumentamos el contador de recursividad
                    processCommand(argumentsHist, nArgumentsHist, recursiveCount, fileList, commandList);
                }
                free(command);
            }
            else
                printf("\033[31mError:\033[0m Unexpected argument '%s' found\n", arguments[1]);
            break;
        default:
            printf("\033[31mError:\033[0m Multiple arguments\n");
            break;
    }
}

void cmd_infosys(char *arguments[MAX_ARGUMENTS], int nArguments) {
    struct utsname machineInfo;

    if (uname(&machineInfo) == -1) {
        perror("\033[31mError:\033[0m ename error");
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

void cmd_open(char *arguments[MAX_ARGUMENTS], int nArguments, tListF *fileList) {
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
            perror("\033[31mImposible abrir fichero:\033[0m No such file or directory");
        else {
            insertElementF((tItemF) {arguments[1], fileDescriptor, mode}, fileList);

            printf("Añadida entrada \033[33m%d\033[0m a la tabla de ficheros abiertos\n", fileDescriptor);
        }
    }
}

void cmd_close (char *arguments[MAX_ARGUMENTS], tListF *fileList) {
    int fileDescriptor;

    if (arguments[1]==NULL || (fileDescriptor=atoi(arguments[1]))<0) {
        cmd_listopen(fileList);
        return;
    }
    if (close(fileDescriptor)==-1)
        perror("\033[31mError:\033[0m Imposible cerrar descriptor");
    else {
        removeElementF(findElementF(fileDescriptor, *fileList), fileList);
    }
}

void cmd_dup (char *arguments[MAX_ARGUMENTS], tListF *fileList) {
    int fileDescriptor, newFileDescriptor;
    tPosF file;
    char aux[MAX_ARGUMENTS];

    if (arguments[1]==NULL || (fileDescriptor=atoi(arguments[1]))<0) {
        cmd_listopen(fileList);
        return;
    }
    if ((file = findElementF(fileDescriptor, *fileList)) == NULL) {
        perror("\033[31mImposible duplicar descriptor: \033[0m Bad file descriptor\n");
        return;
    }
    fileDescriptor = file->data.descriptor;
    newFileDescriptor = dup(fileDescriptor);

    sprintf (aux,"dup \033[33m%d\033[0m (\033[34m%s\033[0m)",fileDescriptor, file->data.fileName);
    insertElementF((tItemF) {aux, newFileDescriptor, file->data.mode}, fileList);
}

void cmd_help(char *arguments[MAX_ARGUMENTS], int nArguments) {
    char *comando = arguments[1];
    switch (nArguments) {
        case 1:
            printf("'help [cmd]' ayuda sobre comandos\n"
                   "\t\tComandos disponibles: \n");
            printf("exit bye fin quit help infosys listopen dup close open command hist time date chdir pid authors \n");
            break;
        case 2:
            if (strcmp(comando, "time") == 0) {
                printf("time \t Muestra la hora actual\n");
            } else if (strcmp(comando, "date") == 0) {
                printf("date\tMuestra la fecha actual\n");
            } else if (strcmp(comando, "hist") == 0) {
                printf("hist [-c|-N]\tMuestra (o borra) el historial de comandos\n");
                printf(" \t -N: muestra los N primeros\n");
                printf(" \t -c: borra el historial\n");
            } else if (strcmp(comando, "command") == 0) {
                printf("command [-N] \t Repite el comando N (del historial)\n");
            } else if (strcmp(comando, "open") == 0) {
                printf("open fich m1 m2... \t Abre el fichero fich. y lo añade a la lista de ficheros abiertos del shell\n");
                printf("\tm1, m2... es el modo de apertura (or bit a bit de los siguientes).\n");
                printf("\tcr: O_CREAT\tap: O_APPEND\n");
                printf("\tex: O_EXCL \tro: O_RDONLY\n");
                printf("\trw: O_RDWR \two: O_WRONLY\n");
                printf("\ttr: O_TRUNC\n");
            } else if (strcmp(comando, "close") == 0) {
                    printf("close df \t Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n");
            } else if (strcmp(comando, "dup") == 0) {
                printf("dup df \t Duplica el descriptor de fichero df y añade una nueva entrada a la lista de ficheros abiertos\n");
            } else if (strcmp(comando, "listopen") == 0) {
                printf("listopen [n] \t Lista los ficheros abiertos (al menos n) del shell\n");
            } else if (strcmp(comando, "infosys") == 0) {
                printf("infosys\tMuestra información de la máquina donde corre el shell\n");
            } else if (strcmp(comando, "help") == 0) {
                printf("help [cmd|-lt|-T] \t Muestra ayuda sobre los comandos\n");
                printf("\tcmd: info sobre el comando cmd\n");
            } else if (strcmp(comando, "quit") == 0 || strcmp(comando, "exit") == 0 || strcmp(comando, "bye") == 0) {
                printf("%s\tTermina la ejecución del shell\n", comando);
            } else if (strcmp(comando, "chdir") == 0) {
                printf("chdir [dir] \t Cambia (o muestra) el directorio actual del shell\n");
            } else if (strcmp(comando, "pid") == 0) {
                printf("pid [-p] \t Muestra el PID del shell o de su proceso padre\n");
                printf("\t-p: muestra el PID del proceso padre\n");
            } else if (strcmp(comando, "authors") == 0) {
                printf("authors [-n|-l] \t Muestra los nombres y/o logins de los autores\n");
                printf("\t-n: muestra solo los nombres de los autores\n");
                printf("\t-l: muestra solo los logins de los autores\n");
            }
            break;
        default:
            printf("Comando desconocido: %s\n", comando);
            break;
    }

}


