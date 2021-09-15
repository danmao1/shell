

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
        cmd=strtok(input, " ");
        my_argv[0]=cmd;
        if(strcmp(cmd,"cd")==0){
            
           my_argv[1]=strtok(NULL," ");
           chdir(my_argv[1]);
        } 
    }
    fgets(buffer, bufferSize, stdin); // get initial input and 
    buffer[strlen(buffer) - 1] = '\0'; // replace newline with null terminator

    prog = strtok(buffer, " ( ");
    int j = 0;
    while(prog != NULL){
        my_argv[j] = prog;
        prog = strtok(NULL, " ");
        j++;
    }

    size_t length = sizeof(my_argv)/sizeof(my_argv[0]);
    char* cmd;
    char* argus[15];
    for (int i = 0; i < length; ++i)
    {
        int k = 0;
        cmd = strtok(my_argv[i], " ");
        while(cmd != NULL){
            argus[k] = cmd;
            cmd = strtok(NULL, " ");
            k++;
        }
    }
    returno 0;
    
}
