#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int chopString(char * cadena, char * trozos[]);
void printPrompt ();
void readInputs();
void processCommand();

int main() {
    bool quit = true;

    while (quit) {
        printPromp();
        readInputs();

    }


    return 0;
}


void printPrompt () {
    printf(">> ");
}

void readInputs() {
    char *cmd;
    char *arguments[5];
    int nArguments;

    fgets(cmd, 60, stdin);
    nArguments = chopString(cmd, arguments);


}

int chopString(char * cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}


