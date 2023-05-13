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




/*

dif = 0x001
pid = 0x001

pidl = 0x002

*pid = [nome*4,pid*4,next*4]

*pid -> nome = 0x111

**pid.pif = 1234
**pid.nome = "ps"
**pid.next = 0x002

struct timeval time[100];
pid_t pid[100];
int counter= 0; 

while(head !=NULL){
    counter++;
    pid = head->pid ;
    time= head->time;
}

*/


struct Node pid_status;// here we put the pids of child that have not finished yet
struct Node *head = NULL;


int parserinput(char * userinput){
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
    char userinput[256]; // user inputs on the terminal
    pid_t pid_worker[1]; //buffer that hold the pid of a process 
    int bytes_written = 0, bytesread = 0;
    //int counter = 0; //counts the number of active processes in pid_status
    int pipefd[2]; // file descriptors of a pipe that reads from processes and is written by other childs
    int status = 0;
    int parser = 1;

    //create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t processes = fork();

    if (processes == 0) {
        close(pipefd[1]); //close write end
        while(read(pipefd[0], pid_worker, sizeof(pid_t))) {
            printf("[DEBUG] PID RECEBIDO %d \n",pid_worker[0]);
         //   checkList(head,pid_worker[0]);
        }
        close(pipefd[0]);
    } else {
        close(pipefd[0]);
        if((fd = open(PIPE_NAME, O_WRONLY)) == -1) perror("FIFO\n");
        while(parser) {
            write(STDOUT_FILENO,"[DEBUG] Insert command: ",25);
            if((bytesread = read(STDIN_FILENO,userinput,sizeof(userinput))) == -1) perror("Userinput:");

            //passes the input  

            parser = parserinput(userinput);
            
            pid_t pidcontrol=fork();
            
            //the child process shall process the input command and the parent shall wait for more user inputs
            if (pidcontrol == 0){
                switch (parser) {
                    case 0:
                        printf("\nO programa vai fechar\n");
                        break;
                    case 1: //execute -u 
                        struct timeval time[3];
                        printf("[DEBUG] This executes the execl function \n");
                        if((bytes_written = write(fd, userinput,strlen(userinput))) == -1) perror("FIFO Write:\n");
                        gettimeofday(time,0);
                        if((bytes_written = write(fd, time,sizeof(struct timeval))) == -1) perror("FIFO Write:\n");
                        printf("[DEBUG] This reaches the execl function \n");
                        pid_t pid=fork();
                        switch(pid) {
                            case 0: 
                                printf("[DEBUG] I AM ALIVE \n");
                                close(pipefd[0]);
                                pid_worker[0] = getpid();
                                // this will write the pid to his grandparent to be added to list of active processes
                                if (write(pipefd[1], pid_worker, sizeof(pid_t)) == -1) {
                                    perror("cannot write to father");
                                    exit(1);
                                }
                                close(pipefd[1]);
                                execl("/bin/ps","ps",NULL);
                            default:
                                printf("[DEBUG] Este é o pid do processo filho %d \n",pid);
                                printf("[DEBUG] Este é o pid do processo pai %d \n",getpid());
                                pid_worker[0] = pid;
                                wait(&status);
                                close(pipefd[0]);
                                if (write(pipefd[1], pid_worker, sizeof(pid_t)) == -1) {
                                    perror("cannot write to father");
                                    _exit(1);
                                }
                                close(pipefd[1]);
                                gettimeofday(time+1,NULL);
                                if((bytes_written = write(fd, time+1,sizeof(struct timeval))) == -1){
                                    perror("FIFO Write:");
                                }
                                time[2].tv_sec = time[1].tv_sec - time[0].tv_sec;
                                time[2].tv_usec = time[1].tv_usec - time[0].tv_usec;
                                //normalizes the diffrence
                                if (time[2].tv_usec < 0) {
                                    time[2].tv_sec--;
                                    time[2].tv_usec += 1000000;
                                }
                                printf("\n[DEBUG] The seconds %ld and the microseconds %ld \n",time[2].tv_sec,time[2].tv_usec);
                                break;
                            }
                        close(fd);
                        _exit(0);
                        break;
                    case 2:
                        printf("\n[DEBUG] This returns the status \n");
                        //code here
                        _exit(0);
                        break;
                    default:
                        printf("\n[DEBUG] Unknown command\n");
                        _exit(0);
                        break;
                }
                bzero(userinput,sizeof(userinput));
                printf("[DEBUG] Sent %d bytes to server\n", bytes_written);
            } else {
                printf("Fork successful \n");
            }
        }
        close(pipefd[0]);
        close(fd);
        // close the pipe 
        return 0;
    }         
    
}

