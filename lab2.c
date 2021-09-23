

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

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

}


int main(int argc, char **argv)
{	char input[256];
	char wdir[100];
	char* cmd;
	int my_argv_size;
	int max_args = 15;
	int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
	char* prog;
	char* my_argv[max_argv_size];
	signal(SIGINT, NULL);
	printf("%s ", getcwd(wdir, 100));
	char* ret_val = fgets(input, 256, stdin);
    pid_t fork_list[16];

	int oldpipe = 0;

	while(ret_val != NULL){



		size_t len= strlen(input);
		if(input[len-1]=='\n'){
			input[len-1]='\0';
		}
        char* commands[15]; //Max 15 commands

        char* ret; 
        ret=strtok(input,"(");
        commands[0] = ret;
        int i = 1;
        ret = strtok( NULL, "(" );
        while( ret != NULL ){
            commands[i] = ret;
            ret = strtok( NULL, "(" ); 
            
            i++;
        }
        commands[i]='\0';

        

        for(int j=0 ; j<i; j++ ){
            char* cmd;
            char* retParse;
            retParse=strtok(commands[j]," ");
            cmd=retParse;
            my_argv[0]=cmd;
            fork_list[j]=fork();
            int k=1;
            while(retParse!=NULL){
                retParse=strtok(NULL," ");
                my_argv[k]=retParse;
                
                k++;
            }
            my_argv[k]='\0';
    
            if (strcmp(cmd, "cd") == 0){
				chdir(my_argv[1]);
			}
            else{
                for (int p=0;p<i;p++){

					oldpipe = pipeExec(my_argv,fork_list[p],p,i, oldpipe);

				}
            }
        }
        printf("%s ", getcwd(wdir, 100));
		ret_val = fgets(input, 256, stdin);
	}

	return 0;

}
