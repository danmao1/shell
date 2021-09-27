#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

int parseByPipe(char input[], char* parsed[]){
    int j = 0;
    char* cmd = strtok(input, "(");

    while( cmd != NULL ){
		
		parsed[j] = cmd;
        cmd = strtok(NULL, "(");
    	j++;

    }
	parsed[j]='\0';
    return j;

}

 

int parseCmds( char* parsed, char* cmds[]){

    int j = 0;

    char* cmd = strtok(parsed, " ");

    while ( cmd != NULL ){
		cmds[j] = cmd;

        cmd = strtok(NULL, " ");
		if(cmd==NULL && j>1){
			perror("Invalid null command");
		}

        j++;

    }
	cmds[j]='\0';

    return j;

}

 

int pipeExec(char* cmd1[], int p, int j, int oldpipe){
	pid_t fork_id=fork();
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
				perror("\nprog: Not found\n");
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
				perror("\nprog: Not found\n");
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
				perror("\nprog: Not found\n");
				exit(0);
			}
		} else {
			//Parent
			close( oldpipe );
			waitpid( fork_id, NULL, 0 );
		}
	}

	return pipefd[0];

}

 

 

 

 

 

int main( int argc, char* argv[] ){
    char input[256];
	char wdir[100];
	char* env;
	int oldpipe = 0;
	signal(SIGINT,  NULL);
	env=getenv("HOME");
	getcwd(wdir, 100);
	char* result=wdir + strlen(env)+1;
	printf("slush|%s> ", result);
    char*  ret_val=fgets(input, 256, stdin);
	
	while(ret_val!=NULL){
        
        input[strlen(input) - 1] = '\0';
		char* my_argv[16]; // max args is 15 + 1 for null terminator
		int len = parseByPipe(input, my_argv);

        for (int j = 0; j <len; j++){

			char* cmds[15];
			int numProgs = parseCmds(my_argv[j], cmds);
			if (strcmp(cmds[0], "cd") == 0){
				chdir(cmds[1]);
			} 
			else if (len==1){
				int exec=execvp(cmds[0],cmds);
				if (exec<0){
					perror("\nprog: Not found\n");
					exit(0);
				}
			}
			else {	

				for (int p = 0; p < len;p++){
					oldpipe = pipeExec(cmds, p, len, oldpipe);

				}

			}
		}
		getcwd(wdir, 100);
		result=wdir + strlen(env)+1;
		printf("%s>> ", result);
    	ret_val=fgets(input, 256, stdin);
		printf("\n");

	}

    return 0;

}