#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5
#define BUFFER_SIZE 1024

int chopCmd(char * cmd, char * tokens[]);
void printPrompt ();
void readInputs(char *cmd, char *arguments[], int *nArguments);
bool processCommand();

void cmdAuthors ();

int main() {
    bool quit;

    char *cmd;
    char *arguments[MAX];
    int nArguments;


    do {
        printPrompt();
        readInputs(cmd, arguments, &nArguments);
        quit = processCommand(arguments);

    } while (quit);


    return 0;
}


void printPrompt () {
    printf(">> ");
}

void readInputs(char *cmd, char *arguments[], int *nArguments) {
    int i = 0;

    cmd = malloc(sizeof(char) * BUFFER_SIZE);
    fgets(cmd, BUFFER_SIZE, stdin);
    // printf("%s\n", cmd);


    *nArguments = chopCmd(cmd, arguments);
    // while (i < *nArguments) {
    //         printf("%s\n", arguments[i]);
    //         i++;
    // }
    // Tokeniza ben

    free(cmd);
}

bool processCommand(char *arguments[MAX]) {

    if (strcmp(arguments[0], "authors") == 0) cmdAuthors();
    // switch () {
    //     case 'authors':

    //         break;
    //     case 'date':

    //         break;
    //     case 'time':

    //         break;

    //     default:
    //         break;
    // }
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


void cmdAuthors () {
    printf("iago dani");
}