

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

void pipeExec(char cmd1[],pid_t fork,char cmd2[],pid_t fork2,int p, int j){
    int pipefd[2];
        if(pipe(pipefd)<0){
            printf("\nPipe could not be initilized\n");
        }
    if(p!=j-1){
        if(fork==0){
            close(pipefd[0]);
            dup2(pipefd[1],STDOUT_FILENO);
            close(pipefd[1]);
            int exec=execvp(cmd1[0],cmd1);
            if (exec<0){
                printf("\nCould not execute command\n");
                exit(0);
            }
        }
    }            
    else if(p!=0){
                             
        
        if(fork!=0){
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            int exec=execvp(cmd2[0],cmd2);
            if (exec<0){
                printf("\nCould not execute command\n");
                exit(0);
            }
        }
        
        else{
            waitpid(fork,NULL,0);
            waitpid(fork2,NULL,0);

        }
    }



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
	fgets(input, 256, stdin);
    while(input!=EOF){
        
        
         
        size_t len= strlen(input);
        if(input[len-1]=='\n'){
		    input[len-1]='\0';
        }
        cmd=strtok(input," ");
        if(strcmp(cmd,"cd")==0){
                       
            my_argv[0]=cmd;
            
            my_argv[1]=strtok(NULL," ");
            chdir(my_argv[1]);
        }
        else{
            pid_t* fork_list[16];
            prog = strtok(input, " ( ");
            int j = 0;
            while(prog != NULL){
                my_argv[j] = prog;
                
                prog = strtok(NULL, " ( ");
                printf("\n%d\n",prog);
                fork_list[j]=fork();
                
                j++;
            }
            
            my_argv[j]='\0';

            
            char* argus[15];
            char* str;
            

            for (int i = 0; i < j; ++i)
            {
                
                int k = 1;
                str = strtok(my_argv[i], " ");
                argus[0] = str;
                
                while(str != NULL){
                    str = strtok(NULL, " ");
                    argus[k]=str;
                    
                    k++;
                }
                argus[k]='\0';
                for (int p=0;p<j;p++){
                    
                    pipeExec(argus[p],fork_list[p],argus[p+1],fork_list[p+1],p,j);
                    
                }
                


            }
            printf("%s ", getcwd(wdir, 100));
	        fgets(input, 256, stdin);
        }
        
        
    }
    
    return 0;
    
}
