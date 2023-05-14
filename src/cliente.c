#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include "assist.h"

#define PIPE_NAME "my_pipe"



void logWrite(int fd,pid_t pid,char *  message) {
    struct timeval tv;
    size_t size = strlen(message);
    // String do timestamp
    gettimeofday(&tv, 0);
    
    write(fd,&size,sizeof(size_t));
    write(fd,message,size);
    write(fd,&pid,sizeof(pid));
    write(fd,&tv,sizeof(tv));
}

int parserinput(char* userinput) {
    strtok(userinput,"\n");
    char* firstWord = strtok(userinput," ");
    if (strcmp(userinput,"status") == 0)
        return 2;
    else if (strcmp(userinput,"exit") == 0)
        return 0;
    else if (strcmp(firstWord,"execute") == 0) {
        char* secondWord = strtok(NULL," ");
        if (strcmp(secondWord,"-u") == 0)
            return 1;
    }
    return -1;
}

int main() {
    int fd; // the file descripter of my_pipe 
    int fd_status; //fd of status pipe
    char userinput[256]; // user inputs on the terminal
    char buffer[256]; // 
    int bytes_written = 0, bytesread = 0;
    int status = 0;
    int parser = 1;
    
    if((fd_status = open("status_pipe", O_RDONLY, 0666)) == -1) perror("Open fifo");

    while(parser) {
        if((bytesread = read(STDIN_FILENO,userinput,sizeof(userinput))) == -1) perror("Userinput:");
        //passes the input  
        char temp[265];
        strcpy(temp,userinput);
        parser = parserinput(userinput);
        
        pid_t pidcontrol=fork();
        
        //the child process shall process the input command and the parent shall wait for more user inputs
        if (pidcontrol == 0){
            switch (parser) {
                case 0:
                    size_t j = 4;
                    if((fd = open(PIPE_NAME, O_WRONLY)) == -1) perror("FIFO\n");
                    write(fd,&j,sizeof(size_t));
                    if ((bytes_written = write(fd, "exit",j)) == -1) perror("FIFO Write:\n");
                    close(fd);
                    break;
                case 1: //execute -u 
                    pid_t pid =fork();
                    switch(pid) {
                        case 0: 
                            int i = 0;
                            char *args[64];
                            while ((args[i++] = strtok(NULL," ")) != NULL);
                            execvp(args[0],args);
                            break;
                        default:
                            char * word = strtok(NULL," ");
                            if((fd = open(PIPE_NAME, O_WRONLY)) == -1) perror("FIFO\n");
                            logWrite(fd,pid,word);
                            close(fd);
                            wait(&status);
                            if((fd = open(PIPE_NAME, O_WRONLY)) == -1) perror("FIFO\n");
                            logWrite(fd,pid,word);
                            close(fd);
                            _exit(0);
                            break;
                        }
                    break;
                case 2:
                    size_t i = 7;
                    if((fd = open(PIPE_NAME, O_WRONLY)) == -1) perror("FIFO\n");
                    write(fd,&i,sizeof(size_t));
                    if ((bytes_written = write(fd, "status",7)) == -1) perror("FIFO Write:\n");
                    close(fd);
                    bzero(buffer, 256);
                    while ((bytesread = read(fd_status, buffer, sizeof(buffer))) > 0) {
                             printf("Received: %s \n", buffer);
                    }               
                    //showPIDS(statusString); //Reveals to the user the pids, names and times of active processes
                    _exit(0);
                    break;
                default:
                    _exit(0);
                    break;
            }
            bzero(userinput,sizeof(userinput));
        }
    }
    // close the pipe 
    return 0;        
}

