
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#define bufferSize 200
int pipeExec(char* cmd1[], pid_t fork_id, int p, int j, int oldpipe){

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

}

int main () {
   int oldpipe=0;
   pid_t id1=fork();
   pid_t id2=fork();
   pid_t id3=fork();
   char* cmds[3]={"ls",NULL};
   char* cmds2[2]={"sort",NULL};
   char* cmds3[3]={"ps","aux",NULL};
   oldpipe=pipeExec(cmds,id1,0,3,oldpipe);
   oldpipe=pipeExec(cmds,id2,1,3,oldpipe);
   oldpipe=pipeExec(cmds,id3,2,3,oldpipe);
      
   


   
   return(0);
}