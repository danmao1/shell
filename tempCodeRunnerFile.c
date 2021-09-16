
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
int main (int argc, char **argv){

    char* test="cd ..";
    char* comp1;
    char* comp2;
    comp1=strtok(test," ");
    comp2= strtok(test," ");

    printf("%s\n",comp1);
    printf("%s\n",comp2);


    return 0;
}