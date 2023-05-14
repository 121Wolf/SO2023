#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "assist.h"


#define PIPE_NAME "my_pipe"

int parserinput(char * userinput){
    strtok(userinput,"\n");
    strtok(userinput," ");
    if (strcmp(userinput,"status") == 0)
        return 2;
    else return 1;
}

void fullparser(char * string, struct timeval * timestamp, pid_t *pid, char * name) {
    timestamp = malloc(sizeof(struct timeval));
    *pid = 1;
    name = string;
}

 struct Node pid_status;// here we put the pids of child that have not finished yet
struct Node *head = NULL;


int main() {
    int pipefd[2];
    //pid_t pid_worker[1];
    pid_t pid_fire;
    size_t fdfifo, fd_log,fd_status;
    char buffer[256];
    int fire = 0;
    int bytes_written;

    if(mkfifo(PIPE_NAME, 0666) == -1){
        perror("MKFIFO");
    }
    //create named pipe to write to client
    if(mkfifo("status_pipe", 0666) == -1){
        perror("MKFIFO");
    }
    //open log.txt
    if ((fd_log = open("../log.txt",  O_CREAT | O_APPEND | O_RDWR , 0666)) == -1) {
        perror("Open log.txt");
        return -1;
    }
       //create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    // open pipe for reading
    if((fd_status = open("status_pipe", O_WRONLY, 0666)) == -1){
        perror("Open fifo");
    }
    printf("[DEBUG] opened FIFO for reading\n");
    pid_t processes = fork();
    if(processes == 0){
        close(pipefd[1]); //close write end
            while(1){
                int flag = 0;
                struct Node node;
                struct timeval diff;
                read(pipefd[0],&flag,sizeof(int));
                if (flag == 1) {
                    if (read(pipefd[0], &node, sizeof(struct Node )) == -1) perror("read");
                    printf("[DEBUG] PID %d TEMPO %ld nome %s \n",node.pid,node.time.tv_usec,node.name);
                    struct Node* spain;
                    if((spain=pidIs(head,node.pid))!= NULL){   
                                                                                                              //percorre e encontra um pid igual na lista ligada 
                                                                                                            //faz a contagem do tempo
                                                                                                            //meter o processo no log file
                                                                                                            //retira o node com este pid
                                diff.tv_sec = node.time.tv_sec - spain->time.tv_sec;
                                diff.tv_usec = node.time.tv_usec - spain->time.tv_usec;
                                //normalizes the diffrence
                                if (diff.tv_usec < 0) {
                                    diff.tv_sec--;
                                    diff.tv_usec += 1000000;
                                }
                                printf("[DEBUG] The seconds %ld and the microseconds %ld \n",diff.tv_sec,diff.tv_usec); 
                                //escrever no log     NEED TO BE DONE !!!!!!!!!! 
                                break;
                    }else{
                        //adds the new process to the begining of the list
                        if((head = checkList(head,node.pid,node.name,node.time)) != NULL) {
                            printf("[DEBUG] NODE ADDED TO LIST OF ACTIVE PROCESSES\n");
                        }
                        else printf("[DEBUG] Something is wrong I can feel it\n");
                    }       
                    printf("[DEBUG] LIST OF PROCESSES UPDATE, will read again\n");
                } else if (flag == 2) {
                    pid_t activepids[50];
                    char *name[50];
                    struct timeval stop[50];
                    struct Node* temp = head;
                    int counter = 0; //atributes the first process to [0] and so on. counter +1 = number of processes
                    char statusString[300];
                    if (head != NULL) printf("HEAD is %d\n",head->pid);
                    //SHOULD THIS BE DONE IN THE PROCESS CHILD TO SECURE DATA?
                    while (temp != NULL){
                        activepids[counter] = temp->pid;
                        name[counter] = temp->name;
                        stop[counter].tv_sec = temp->time.tv_sec;
                        stop[counter].tv_usec = temp->time.tv_usec;
                        temp = temp->next;
                        counter++;
                    } 
                    if (counter == 0) {
                        printf("Não existem processos ativos\n");
                        break;
                    }
                    struct timeval end,diff[50];
                    gettimeofday(&end,NULL);
                    for(int i = 0; i < counter;i++){
                        diff[i].tv_sec = end.tv_sec - stop[i].tv_sec;
                        diff[i].tv_usec = end.tv_usec - stop[i].tv_usec;
                        //normalizes the diffrence
                        if (diff[i].tv_usec < 0) {
                            diff[i].tv_sec--;
                            diff[i].tv_usec += 1000000;
                        }
                        printf("[DEBUG] The seconds %ld and the microseconds %ld \n",diff[i].tv_sec,diff[i].tv_usec);
                    }

                    dataToString(activepids,name,diff,counter,statusString);//will parse informartion into a string
                    if((bytes_written = write(fd_status, statusString,strlen(statusString))) == -1) perror("FIFO Write:\n");
                }
            }
        close(pipefd[0]);
    }else{
    //puts all values of buffer to \0
        int flag;
        while(1){
            if((fdfifo = open(PIPE_NAME, O_RDONLY, 0666)) == -1) perror("Open fifo");
            bzero(buffer, 256);
            pid_t pid;
            size_t size;
            struct timeval timestamp;
            printf("[DEBUG] Server listening... fdfifo %lu \n",fdfifo);
            // read from pipe and print to console
            if (read(fdfifo,&size,sizeof(size_t)) > 0)
                read(fdfifo,buffer,size);
            if ((fire= parserinput(buffer)) ==  1) {
                read(fdfifo,&pid,sizeof(pid_t));
                read(fdfifo,&timestamp,sizeof(struct timeval));
                printf("PID:%d NOME:%s TIMESTAMP:%ld\n",pid,buffer,timestamp.tv_usec);
            }
            pid_fire = fork();
            if(pid_fire == 0){
            switch (fire){
                case 1: //this is the execute case
                    flag = 1;
                   close(pipefd[0]);
                   struct Node* node = createNode(pid,buffer,timestamp);
                   write(pipefd[1],&flag,sizeof(int));
                    if (write(pipefd[1], node, sizeof(struct Node)) == -1) {
                        perror("cannot write to pid controller");
                        exit(1);
                    }
                    free(node);
                    close(pipefd[1]);
                    break;
        
                case 2: //this is status case
                    printf("Received status\n");
                    flag = 2;
                    write(pipefd[1], &flag, sizeof(int));                    
                    break;
                default:
                    printf("[DEBUG] Error! Command unknown");
                    _exit(0);
                    break;
            }
            close(fdfifo);
            _exit(0);
        }
    }
    
    // close the pipe
    printf("Pipe closed \n");
    close(fd_log);
    return 0;
}
}

