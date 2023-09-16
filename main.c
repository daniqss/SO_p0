#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5
#define BUFFER_SIZE 1024

int chopCmd(char * cmd, char * tokens[]);
int printPrompt ();
void readInputs(char *cmd, char *arguments[], int *nArguments);
bool processCommand(char *arguments[MAX], int nArguments);

void cmdAuthors ();
void cmdPid (char *arguments[MAX], int nArguments);

int main() {
    bool quit;

    char *cmd = NULL;
    char *arguments[MAX];
    int nArguments;

    do {
        if (printPrompt() == 1) return 1;
        readInputs(cmd, arguments, &nArguments);
        quit = processCommand(arguments, nArguments);

    } while (quit);


    return 0;
}


int printPrompt () {
    char * userName = getenv("USER");
    if (userName == NULL) return EXIT_FAILURE;
    printf("[%s]>> ", userName);
}

void readInputs(char *cmd, char *arguments[], int *nArguments) {
    int i = 0;

    cmd = malloc(sizeof(char) * BUFFER_SIZE);
    fgets(cmd, BUFFER_SIZE, stdin);
    //printf("%s\n", cmd);

    *nArguments = chopCmd(cmd, arguments);
    // while (i < *nArguments) {
    //         printf("%s\n", arguments[i]);
    //         i++;
    // }
    //Tokeniza ben
}

bool processCommand(char *arguments[MAX], int nArguments) {
    if (arguments[0] != NULL) printf("%s\n", arguments[0]);
    if (arguments[1] != NULL) printf("%s\n", arguments[1]);
    if (arguments[2] == NULL) printf("el dos apunta a null\n");
    printf("%d arguments\n", nArguments);

    if (strcmp(arguments[0], "authors") == 0) cmdAuthors(arguments, nArguments);
    else if (strcmp(arguments[0], "pid") == 0) cmdPid(arguments, nArguments);
    else if (strcmp(arguments[0], "quit") == 0) return false;
    else if (strcmp(arguments[0], "bye") == 0) return false;
    else if (strcmp(arguments[0], "exit") == 0) return false;
    else printf("macaco escribe ben");
     
    printf("nashe");
    return true;
}

int chopCmd(char * cmd, char * tokens[]) {
    int i=1;
    if ((tokens[0]=strtok(cmd," \n\t"))==NULL)
        return 0;
    while ((tokens[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}


void cmdAuthors (char *arguments[MAX], int nArguments) {
    char authorsNames[] = "Iago Dani";
    char authorsLogins[] = "Iago@udc Dani@udc";

    switch (nArguments) {
        case 1:
            printf("%s\n%s", authorsNames, authorsLogins);
            break;
        case 2:
                if (strcmp(arguments[1], "-l") == 0) printf ("%s\n", authorsLogins);
                else if (strcmp(arguments[1], "-n") == 0) printf ("%s\n", authorsNames);
                else printf("Error: Unexpected argument '%s' found\n", arguments[1]);
        default:
            printf("Error: Multiple arguements\n");
            break;
        }
}

void cmdPid (char *arguments[MAX], int nArguments) {
    char *pid = getenv("$$");
    char *ppid = getenv("PPID");

    switch (nArguments) {
        case 1:
            printf("Actual process identifier: %s", pid);
            break;
        case 2:
                if (strcmp(arguments[1], "-l") == 0) printf("Actual process identifier: %s", ppid);
                else printf("Error: Unexpected argument '%s' found", arguments[1]);
        default:
            printf("Error: Multiple arguements");
            break;
        }
}