#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "assist.h"


#define PIPE_NAME "my_pipe"



int main() {
    int fd; // the file descripter of my_pipe 
    char message[256];
    char userinput[256]; // user inputs on the terminal
    pid_t pid_status[10]; // here we put the pids of child that have not finished yet
    //char *endmessage = "Coisas aconteceram bro";
    int bytes_written;
    int bytesread;
    char *command = "ls";
    int status = 0;


     // open pipe for writing
    if((fd = open(PIPE_NAME, O_WRONLY)) == -1){
        perror("FIFO");
    }
        if((bytesread = read(STDIN_FILENO,userinput,sizeof(userinput))) == -1){
          perror("Userinput:");
        }
    else{
        while(1){
            if((bytesread = read(STDIN_FILENO,userinput,sizeof(userinput))) == -1){
                perror("Userinput:");
            }
            //commandparser(userinput,bytesread);
        }
    }




    //execl("/bin/ls", command, NULL);
    for (int i = 0; i < 5; i++) {
        // Guardar o pid do filho
        pid_t pid=fork();
        switch(pid) {
            case 0: 
                struct timeval start, end, diff;
                gettimeofday(&start,NULL);
                printf("[DEBUG] The seconds %ld and the microseconds %ld",start.tv_sec,start.tv_usec);
                execl("/bin/ls",command,NULL);
                gettimeofday(&end,NULL);
                //calculates the diffrence between the start and the end
                diff.tv_sec = end.tv_sec - start.tv_sec;
                diff.tv_usec = end.tv_usec - start.tv_usec;
                    //normalizes the diffrence
                     if (diff.tv_usec < 0) {
                        diff.tv_sec--;
                         diff.tv_usec += 1000000;
                             }
                printf("[DEBUG] The seconds %ld and the microseconds %ld",diff.tv_sec,diff.tv_usec);
                _exit(0);
            default:
            //writes the pid in the array of unfinished child pids, to be used for status
                int counter = 0;
                for(int i = 0;i<10;i++){
                    if(pid_status != 0){
                        counter ++;
                    }
                    pid_status[counter++] = pid;

                }
                printf("Fork successful %d\n",pid);
                wait(&status);
        }
    }
        // int to string, to be confirmed 
   /* long j;
    for (j = 1; j < pid ;j = j * 10);
    j = j / 10;
    int i;
    for (i = 0; j != 0; i++) {
        message[i] = (pid/j) + '0';
        pid = pid % j;
        j = j / 10;
    }
    
    message[i] = 0;
*/
    // write message to pipe
    if((bytes_written = write(fd, message, strlen(message))) == -1){
        perror("FIFO Write:");
    }
    
    printf("[DEBUG] Sent %d bytes to server\n", bytes_written);
    
    //waits for child PID _exit(0)
    //pid_t terminated_pid = wait(&status);
    
    /*if(bytes_written2 = write(fd,endmessage,strlen(endmessage)) == -1){
        perror("A comunicação falhou devido:");
    }
    printf("[DEBUG] Sent %d bytes to server\n", bytes_written2);*/

    // close the pipe 
    close(fd);

    return 0;
}