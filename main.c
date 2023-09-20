#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <fcntl.h>

#define MAX 5
#define BUFFER_SIZE 1024

int chopCmd(char *cmd, char *tokens[]);
void printPrompt();
void readInputs(char *cmd, char *arguments[], int *nArguments);
bool processCommand(char *arguments[MAX], int nArguments);

void cmd_authors(char *arguments[MAX], int nArguments);
void cmd_pid(char *arguments[MAX], int nArguments);
void cmd_chdir(char *arguments[MAX], int nArguments);
void cmd_infosys(char *arguments[MAX], int nArguments);

int main()
{
    bool quit;

    char *cmd = NULL;
    char *arguments[MAX];
    int nArguments;

    do
    {
        printPrompt();
        readInputs(cmd, arguments, &nArguments);
        quit = processCommand(arguments, nArguments);

    } while (quit);
    for (int i = 0; i < nArguments; i++)
    {
        free(arguments[i]);
    }
    free(cmd);
    return EXIT_SUCCESS;
}

void printPrompt()
{
    char *userName = getenv("USER");
    if (userName == NULL)
        exit(EXIT_FAILURE);

    printf("[%s]>> ", userName);
}

void readInputs(char *cmd, char *arguments[], int *nArguments)
{
    ssize_t bytesRead; // ssize_t y size_t son unsigned int
    size_t bufferSize = 0;

    bytesRead = getline(&cmd, &bufferSize, stdin); // getline devuelve el número de bytes escritos si no hay errores

    if (bytesRead == -1)
    {
        perror("Lectura fallida");
        exit(EXIT_FAILURE);
    }

    *nArguments = chopCmd(cmd, arguments);
}

int chopCmd(char *cmd, char *tokens[])
{
    int i = 1;
    if ((tokens[0] = strtok(cmd, " \n\t")) == NULL)
        return 0;
    while ((tokens[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

bool processCommand(char *arguments[MAX], int nArguments)
{
    // if (arguments[0] != NULL) printf("%s\n", arguments[0]);
    // if (arguments[1] != NULL) printf("%s\n", arguments[1]);
    // if (arguments[2] == NULL) printf("el dos apunta a null\n");
    // printf("%d arguments\n", nArguments);

    if (strcmp(arguments[0], "authors") == 0)
        cmd_authors(arguments, nArguments);
    else if (strcmp(arguments[0], "pid") == 0)
        cmd_pid(arguments, nArguments);
    else if (strcmp(arguments[0], "chdir") == 0)
        cmd_chdir(arguments, nArguments);
    else if (strcmp(arguments[0], "infosys") == 0)
        cmd_infosys(arguments, nArguments);

    else if (strcmp(arguments[0], "quit") == 0)
        return false;
    else if (strcmp(arguments[0], "bye") == 0)
        return false;
    else if (strcmp(arguments[0], "exit") == 0)
        return false;

    else
        printf("macaco escribe ben\n");

    return true;
}

void cmd_authors(char *arguments[MAX], int nArguments)
{
    char authorsNames[] = "Iago Dani";
    char authorsLogins[] = "Iago@udc Dani@udc";

    switch (nArguments)
    {
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
        printf("Error: Multiple arguements\n");
        break;
    }
}

void cmd_pid(char *arguments[MAX], int nArguments)
{

    switch (nArguments)
    {
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
        printf("Error: Multiple arguements\n");
        break;
    }
}

void cmd_chdir(char *arguments[MAX], int nArguments)
{
    char *cwd = (char *)malloc(sizeof(BUFFER_SIZE));
    if (cwd == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    switch (nArguments)
    {
    case 1:
        if (getcwd(cwd, BUFFER_SIZE) == NULL)
        {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Current working directory: %s\n", cwd);
        }
        free(cwd);
        break;

    case 2:
        chdir(arguments[1]);
        break;

    default:
        printf("Error: Multiple arguements\n");
        break;
    }
}

void cmd_infosys(char *arguments[MAX], int nArguments)
{
    struct utsname machineInfo;

    if (uname(&machineInfo) == -1)
    {
        perror("ename error");
        exit(EXIT_FAILURE);
    }
    printf("Operative system = %s\n", machineInfo.sysname);
    printf("Machine name   = %s\n", machineInfo.nodename);
    printf("Kernel version     = %s\n", machineInfo.release);
    printf("???     = %s\n", machineInfo.version);
    printf("Arquitectura     = %s\n", machineInfo.machine);
}

void cmd_open(char *arguments[], int nArguments)
{
    int i, fileDescriptor, mode = 0;

    if (arguments[0] == NULL)
    {

        return;
    }

    for (i = 1; arguments[i] != NULL; i++)
    {
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
    if ((fileDescriptor = open(arguments[0], mode, 0777)) == -1)
        perror("Imposible abrir fichero");
    else
    {
        //...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        printf("Anadida entrada a la tabla ficheros abiertos..................");
    }