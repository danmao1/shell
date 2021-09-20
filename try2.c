#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>


int  main(int argc, char* argv[]){

	char input[256];
	char wdir[100];
	char* cmd;
	int max_args = 15;
    int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
    char* my_argv[max_argv_size];
    signal(SIGINT, NULL);

    while(1){

    	printf("%s ", getcwd(wdir, 100));
    	fgets(input, 256, stdin);

    	input[strlen(input) - 1] = '\0';

    	int i = 0;
    	cmd = strtok(input, " ( ");
    	while (cmd != NULL){ // tokenizing commands from input by " ( "
    		my_argv[i] = cmd;
    		cmd = strtok(NULL, " ( ");
    		i++;
    	}

  		//i think first pipe is created here
    	for (i = i - 1; i >= 0; i--){ // tokenizing progs and args from commands from left to right 
    		char* curProg[17];
    		char* progName;

    		int k = 0;
    		progName = strtok(commands[i], " ");
    		while (progName != NULL){
    			curProg[k] = progName;
    			progName = strtok(NULL, " ");
    			k++;
    		}

    		if (strcmp(curProg[0], "cd") == 0){ // built in commands
    			chdir(curProg[1]);
    		} else {
    			
    		}
    	}
    }

	return 0;
}