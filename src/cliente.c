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
int liveness = 1;




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
    char* firstWord = strtok(userinput," ");
    if (strcmp(userinput,"status\n") == 0)
        return 2;
    else if (strcmp(userinput,"exit\n") == 0)
        return 3;
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
    int bytes_written, bytesread;
    //int counter = 0; //counts the number of active processes in pid_status
    int pipefd[2]; // file descriptors of a pipe that reads from processes and is written by other childs
    int status = 0;
    int parser = 0;

    //create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    pid_t processes = fork();

    if (processes == 0) {
        close(pipefd[1]); //close write end
        while(1) {
            if (read(pipefd[0], pid_worker, sizeof(pid_t)) == -1) perror("read");
            printf("[DEBUG] PID RECEBIDO %d \n",pid_worker[0]);
         //   checkList(head,pid_worker[0]);
        }
    } else {
        while(liveness) {
            printf("[DEBUG] Insert command: \n");
            if((bytesread = read(STDIN_FILENO,userinput,sizeof(userinput))) == -1) perror("Userinput:");

            //passes the input  
            parser = parserinput(userinput);
            pid_t pidcontrol=fork();
            
            //the child process shall process the input command and the parent shall wait for more user inputs
            if (pidcontrol == 0){
                switch (parser) {
                    case 0:
                        printf("[DEBUG] This input cannot be processed, try again");
                        break;

                    case 1: //execute -u 
                        struct timeval time[3];
                        printf("[DEBUG] This executes the execl function \n");
                        if((fd = open(PIPE_NAME, O_WRONLY)) == -1) perror("FIFO");
                        if((bytes_written = write(fd, userinput,strlen(userinput))) == -1) perror("FIFO Write:");
                        gettimeofday(time,0);
                        if((bytes_written = write(fd, time,sizeof(struct timeval))) == -1) perror("FIFO Write:");
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
                                strtok(userinput," ");
                                strtok(NULL," ");
                                char* word = strtok(NULL," ");
                                execl(word,word,NULL);
                            default:
                                printf("[DEBUG] Este é o pid do processo filho %d \n",pid);
                                printf("[DEBUG] Este é o pid do processo pai %d \n",getpid());
                                pid_worker[0] = pid;
                                wait(&status);
                                close(pipefd[0]);
                                if (write(pipefd[1], pid_worker, sizeof(pid_t)) == -1) {
                                    perror("cannot write to father");
                                    exit(1);
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
                                printf("[DEBUG] The seconds %ld and the microseconds %ld \n",time[2].tv_sec,time[2].tv_usec);       
                                break;
                            }
                        break;
                    case 2:
                        printf("[DEBUG] This returns the status \n");
                        //code here
                        break;
                    case 3:
                        printf("[DEBUG] This quits the program\n");
                        liveness = 0;
                        break;
                    default:
                        printf("[DEBUG] Unknown command\n");
                        break;
                }
                printf("[DEBUG] Sent %d bytes to server\n", bytes_written);
                //waits for child PID _exit(0)
                //pid_t terminated_pid = wait(&status);
            } else {
                /* close(pipefd[1]); //close write end
                if (read(pipefd[0], &pid_worker, sizeof(pid_t)) == -1) {
                    perror("read");
                    exit(1);
                }
                insertAtBeginning(&head, pid_worker); //this adds pid of child to linked list of processes that have yet not ended
                printf("Child PID received: %d\n", pid_worker);
                close(pipefd[0]);
                */
                printf("Fork successful \n");
                //  for(int i=0;i < counter;i++){
                //      printf("The child  %d \n",pid_status[i]);
                //  }
            }
        }
        // close the pipe 
        close(fd);
        return 0;
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