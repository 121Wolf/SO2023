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

int parserinput(char * userinput){
    return 1;
}

int main() {
    int fd; // the file descripter of my_pipe 
    char userinput[256]; // user inputs on the terminal
    pid_t pid_status[10]; // here we put the pids of child that have not finished yet
    int bytes_written;
    int bytesread;
    int counter = 0; //counts the number of active processes in pid_status
    char *command = "ls";
    int status = 0;
    int parser = 0;


         while(1){
            printf("[DEBUG] Insert command: \n");
            if((bytesread = read(STDIN_FILENO,userinput,sizeof(userinput))) == -1){
                perror("Userinput:");
            }
            //passes the input
             parser = parserinput(userinput);
             pid_t pidcontrol=fork();
            
                //the child process shall process the input command and the parent shall wait for more user inputs
            if(pidcontrol == 0){

                 switch (parser)
             {
             case 0:
                printf("[DEBUG] This input cannot be processed, try again");
                break;

             case 1: //execute -u 

                struct timeval start,end,diff;
                 printf("[DEBUG] This executes the execl function \n");
                 if((fd = open(PIPE_NAME, O_WRONLY)) == -1){
                         perror("FIFO");
                }
                if((bytes_written = write(fd, &userinput,sizeof(start))) == -1){
                         perror("FIFO Write:");
                }
                gettimeofday(&start,NULL);
                if((bytes_written = write(fd, &start,sizeof(start))) == -1){
                         perror("FIFO Write:");
                }
                     pid_t pid=fork();
                        switch(pid) {
                        case 0: 
                            printf("[DEBUG] I AM ALIVE \n");
                            execl("/bin/ls",command,NULL);
                            _exit(0);
                         default:
                //writes the pid in the array of unfinished child pids, to be used for status
                /* counter = 0;                   
                for(int i = 0;i<10;i++){
                    if(pid_status != 0){
                        counter ++;
                    }
                    pid_status[counter++] = pid;
                    }*/
               printf("[DEBUG] Este é o pid do processo filho %d \n",pid);
               // printf("[DEBUG] The seconds %ld and the microseconds %ld \n",start.tv_sec,start.tv_usec);
                pid_status[counter]= pid;
                counter++;
                printf("Fork successful %d\n with counter %d \n",pid,counter);
                for(int i=0;i < counter;i++){
                    printf("The child  %d \n",pid_status[i]);
                }
                wait(&status);
                gettimeofday(&end,NULL);
                if((bytes_written = write(fd, &end,sizeof(start))) == -1){
                         perror("FIFO Write:");
        }
                diff.tv_sec = end.tv_sec - start.tv_sec;
                diff.tv_usec = end.tv_usec - start.tv_usec;
                    //normalizes the diffrence
                     if (diff.tv_usec < 0) {
                        diff.tv_sec--;
                         diff.tv_usec += 1000000;
                             }
                printf("[DEBUG] The seconds %ld and the microseconds %ld \n",diff.tv_sec,diff.tv_usec);    
                break;    
                 }

             case 2:


              printf("[DEBUG] This returns the status");
              //code here
                break;
             }

    printf("[DEBUG] Sent %d bytes to server\n", bytes_written);
     //waits for child PID _exit(0)
    //pid_t terminated_pid = wait(&status);

    // close the pipe 
     close(fd);
     return 0;
}
            
    }
}

   
    //}
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