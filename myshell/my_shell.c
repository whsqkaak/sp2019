/*
    whsqkaak@naver.com 
    2019/04/24
    myshell.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CMD_LEN 100 // The Maximum command length.


int main(void){
    char *args[CMD_LEN + 1]; // command line arguments.
    char *input; // Input command.
    int i;
    // Get input command repeatedly
    while (1) {
        printf("LSH_SHELL>>> ");
	fflush(stdout); // Flush stdout buffer.

        input = (char*)malloc(CMD_LEN * sizeof(char));
        fgets(input, CMD_LEN, stdin);

        i = 0;
	args[i] = strtok(input, " \n");

        // strtok input command line
	while(args[i] != NULL){
            i++;
	    args[i] = strtok(NULL," \n");
	}
        
        // NULL input
	if(args[0] == NULL){
            goto no_input;
	}

        // exit command
	if(strcmp(args[0], "exit")==0){
            break;
	}

        no_input:;
	free(input);

    }
    return 0;
}
