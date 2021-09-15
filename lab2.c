

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
    
    char* my_argv[16];
    char* tmp;
    signal(SIGINT, NULL);
    while(1){
        printf("%s ", getcwd(wdir, 100));
	    fgets(input, 256, stdin);
        size_t len= strlen(input);
        if(input[len-1]=='\n'){
		    input[--len]='\0';
        }
        cmd=strtok(input, " ");
        my_argv[0]=cmd;
        if(strcmp(cmd,"cd")==0){
            
           my_argv[1]=strtok(NULL," ");
           chdir(my_argv[1]);

            
            
        } 
        
       
        
         

    }
    
}
