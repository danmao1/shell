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

<<<<<<< HEAD
char* parseCmds( char* parsed){
=======
int parseCmds( char* parsed, char* cmds[]){
>>>>>>> b0bc37e (try3 push)
	int j = 0;
	char* str=malloc(16);
	char* cmd = strtok(parsed, " ");
	while ( cmd != NULL ){
		str[j]=cmd;
		cmd = strtok(NULL, " ");
		j++;
	}
<<<<<<< HEAD
	return str;
=======
	return j;
}

int pipeExec(char cmd1[], pid_t fork_id, int p, int j, int oldpipe){

	//Note, pipe only for beginning and middle processes of pipeline
	int pipefd[2];
	if(pipe(pipefd)<0){
		printf("\nPipe could not be initilized\n");
	}
	if(p == 0){ //Starting process
		if(fork_id==0){ //Child process
			close(pipefd[0]); //close read end
			dup2(pipefd[1],STDOUT_FILENO); //redirect write end to stdout
			//close(pipefd[1]); don't close this! Need write end of pipe
			int exec=execvp(cmd1[0],cmd1);
			if (exec<0){
				printf("\nCould not execute command\n");
				exit(0);
			}
		} else {
			//Parent
			close( pipefd[1] );

		}
	}            
	else if( (p != 0) && (p != j-1) ){ //Middle process


		if(fork_id == 0){ //Child process
			close(pipefd[0]); //Close read end of pipe we just made
			dup2( pipefd[1], STDOUT_FILENO );
			dup2( oldpipe, STDIN_FILENO );
			int exec=execvp(cmd1[0],cmd1);
			if (exec<0){
				printf("\nCould not execute command\n");
				exit(0);
			}
		}

		else{
			//Parent
			close( pipefd[1] );
			close( oldpipe );
		}
	} else if ( p == j-1 ){ //End process

		if( fork_id == 0 ){ //Child process
			dup2( oldpipe, STDIN_FILENO );
			int exec=execvp(cmd1[0],cmd1);
			if (exec<0){
				printf("\nCould not execute command\n");
				exit(0);
			}
		} else {
			//Parent
			close( oldpipe );
			waitpid( fork_id, NULL, 0 );
		}
	}

	return pipefd[0];

>>>>>>> b0bc37e (try3 push)
}





int main( int argc, char* argv[] ){

	char input[256];
	char wdir[100];
	char* fork_list[16];
	int oldpipe = 0;
	signal(SIGINT,  NULL);
	
	while(1){

		printf("%s>> ", getcwd(wdir, 100));
		fgets(input, 256, stdin);
		input[strlen(input) - 1] = '\0';

		char* my_argv[16]; // max args is 15 + 1 for null terminator

		my_argv= parseByPipe(input);
		for (int j = my_argv.size() - 1; j >= 0; j--){									
			char* cmds[15];
<<<<<<< HEAD
			cmds=parseCmds(my_argv[j]);
=======
			int numProgs = parseCmds(my_argv[j], cmds);
>>>>>>> b0bc37e (try3 push)
			if (strcmp(cmds[0], "cd") == 0){
				chdir(cmds[1]);
			} else {
				fork_list[j] = fork();
				for (int p = 0; p < numProgs - 1;p++){
					oldpipe = pipeExec(my_argv, fork_list[j], p, numProgs - 1, oldpipe);
				}
			}
			printf("\n%s\n",cmds[j]);
		}
	}

	return 0;
}