#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]){
    char buffer[MAX_SIZE];
    int fifo;
    
}