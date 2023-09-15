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
    printf("%s\n", arguments[0]);
    printf("%s\n", arguments[1]);

    if (strcmp(arguments[0], "authors") == 0) cmdAuthors(arguments, nArguments);
    else if ((strcmp(arguments[0], "quit") || strcmp(arguments[0], "bye"))|| strcmp(arguments[0], "exit")) return true;
    return false;
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
    if (nArguments == 1) printf("%s\n%s", authorsNames, authorsLogins);
    else {
        if (strcmp(arguments[1], "-l") == 0) printf ("%s", authorsLogins);
        else printf("%s", authorsNames);
    }
}