
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#define bufferSize 200
int main (int argc, char **argv){

    char buffer[bufferSize];
    fgets(buffer, bufferSize, stdin);
    size_t len= strlen(buffer);
    if(buffer[len-1]=='\n'){
		buffer[--len]='\0';
	}
    char* comp1;
    char* comp2;
    comp1=strtok(buffer," ");
    comp2=strtok(buffer," ");

    printf("%s\n",comp1);
    printf("%s\n",comp2);


    return 0;
}