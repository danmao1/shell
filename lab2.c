

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

int main(int argc, char **argv)
{	char input[256];
    char wdir[100];
    char* cmd;
    
    int max_args = 15;
    int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
    char* prog;
    char* my_argv[max_argv_size];
    signal(SIGINT, NULL);
    while(1){
        printf("%s ", getcwd(wdir, 100));
	    fgets(input, 256, stdin);
         
        size_t len= strlen(input);
        if(input[len-1]=='\n'){
		    input[--len]='\0';
        }
        cmd=strtok(input," ");
        if(strcmp(cmd,"cd")==0){
            
            
            my_argv[0]=cmd;
            
            my_argv[1]=strtok(NULL," ");
            chdir(my_argv[1]);
        }
        else{
            int pipefd[2];
           if(pipe(pipefd)<0){
               printf("\nPipe could not be initilized\n");
           }
            prog = strtok(input, " ( ");
            int j = 0;
            while(prog != NULL){
                my_argv[j] = prog;
                prog = strtok(NULL, " ( ");
                j++;
            }
            size_t length = sizeof(my_argv)/sizeof(my_argv[0]);

            char* argus[15];
            char* str;
            pid_t* fork_list[length];

            for (int i = 0; i < length; ++i)
            {
                int p=0;
                int k = 1;
                str = strtok(my_argv[i], " ");
                argus[0] = str;
                
                while(str != NULL){
                    str = strtok(NULL, " ");
                    argus[k]=str;
                    
                    k++;
                }
                argus[k]='\0';
                fork_list[p]=fork();
                int oldfd;
                if(fork_list[p]<0){
                    printf("\ncould not fork\n");
                }
                if(fork_list[p]==0){
                    
                    close(pipefd[0]);
                    dup2(pipefd[1],STDOUT_FILENO);
                    close(pipefd[1]);
                    int exec=execvp(cmd,argus);
                    if (exec<0){
                        printf("\nCould not execute command\n");
                        exit(0);
                    }

                }
                else{
                    fork_list[p+1]=fork();
                    if (fork_list[p+1]<0){
                        printf("\n could not fork\n");
                    }
                    int k = 1;
                    i++;
                    str = strtok(my_argv[i], " ");
                    argus[0] = str;
                
                    while(str != NULL){
                        str = strtok(NULL, " ");
                        argus[k]=str;
                    
                         k++;
                    }
                    if (fork_list[p+1]==0){
                        close(pipefd[1]);
                        dup2(pipefd[0], STDIN_FILENO);
                        close(pipefd[0]);
                        int exec=execvp(cmd,argus);
                        if (exec<0){
                            printf("\nCould not execute command\n");
                            exit(0);
                        }
                    }
                    else{
                        waitpid(fork_list[p],NULL,0);
                        waitpid(fork_list[p+1],NULL,0);

                    }
                }
                p+=2;


            }
    
        }
        
    }
    
    return 0;
    
}
