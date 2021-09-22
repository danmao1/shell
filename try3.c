#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

char* parseByPipe(char input[]){
	int j = 0;
	char* cmd = strtok(input, "(");
	while( cmd != NULL ){
		parsed[j] = cmd;
		cmd = strtok(NULL, "(");
		j++;
	}
	return parsed;
}

char* parseCmds( char* parsed){
	int j = 0;
	char* str=malloc(16);
	char* cmd = strtok(parsed, " ");
	while ( cmd != NULL ){
		str[j]=cmd;
		cmd = strtok(NULL, " ");
		j++;
	}
	return str;
}



int main( int argc, char* argv[] ){

	char input[256];
	char wdir[100];
	signal(SIGINT,  NULL);
	
	while(1){

		printf("%s>> ", getcwd(wdir, 100));
		fgets(input, 256, stdin);
		input[strlen(input) - 1] = '\0';

		char* my_argv[16]; // max args is 15 + 1 for null terminator

		my_argv= parseByPipe(input);
		for (int j = my_argv.size() - 1; j >= 0; j--){									
			char* cmds[15];
			cmds=parseCmds(my_argv[j]);
			if (strcmp(cmds[0], "cd") == 0){
				chdir(cmds[1]);
			}
			printf("\n%s\n",cmds[j]);
		}
	}

	return 0;
}