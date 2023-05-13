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
    //int pipefd[2];
    //pid_t pid_worker;
    size_t fdfifo, fd_log;
    char buffer[256];
    int bytes_read;
    int fire = 0;
  //  int bytes_wrote;
    //char *message = "mete isto";

   // buffer = (char *) malloc(256);
    // create named pipe
    if(mkfifo(PIPE_NAME, 0666) == -1){
        perror("MKFIFO");
    }
    //open log.txt
    if ((fd_log = open("log.txt",  O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1) {
        perror("Open log.txt");
        return -1;
    }


    // open pipe for reading
    if((fdfifo = open(PIPE_NAME, O_RDONLY, 0666)) == -1){
        perror("Open fifo");
    }
        printf("[DEBUG] opened FIFO for reading\n");
    //puts all values of buffer to \0
       while(1){
        bzero(buffer, 256);
        printf("[DEBUG] Server listening... fdfifo %lu \n",fdfifo);

/*
         pid_t processes = fork();
     if(processes == 0){
        close(pipefd[1]); //close write end
        while(1){
                if (read(pipefd[0], pid_worker, sizeof(pid_t)) == -1) {
                        perror("read");
                        }
                    printf("[DEBUG] PID RECEBIDO %d \n",pid_worker[0]);
                   checkList(head,pid_worker[0]);
                   printf("[DEBUG] ");
                
            }
     }
     */

    
     // read from pipe and print to console
        size_t readen = 0;
        while ((bytes_read = read(fdfifo, buffer, sizeof(buffer))) > 0) {
            readen += bytes_read;
            printf("Received: %s and  read %lu bytes \n", buffer, readen);
        }
        fire = parserinput(buffer); //will parse the command that was given by the client
        printf("Este é o Caso do parser: %d\n", fire);
        
        switch (fire)
        {
        case 0:/* constant-expression */
            /* code */
            break;
        
        default:
            break;
        }








        /* this is something idk
        int pid=0;
        for (int i = 0;i < readen; i++) {
            pid = pid*10;
            pid+=buffer[i]-'0';
        }
        */
        

        //write to log file the buffer message to save the command and arguments
        //if ((bytes_wrote = write(fd_log,buffer,readen)) == -1) {
        //    printf("[DEBUG] wrote %s to file\n", buffer);
    
    
    // close the pipe
    close(fdfifo);
    printf("Pipe closed \n");
    close(fd_log);

    return 0;
}
}
