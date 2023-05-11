#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "assist.h"


#define PIPE_NAME "my_pipe"

int main() {
    size_t fdfifo, fd_log;
    char buffer[256];
    int bytes_read;
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
    else 
        printf("[DEBUG] opened FIFO for reading\n");
    //puts all values of buffer to \0
        bzero(buffer, 256);
        printf("[DEBUG] Server listening... fdfifo %lu \n",fdfifo);

    
     // read from pipe and print to console
        size_t readen = 0;
        while ((bytes_read = read(fdfifo, buffer, sizeof(buffer))) > 0) {
            readen += bytes_read;
            printf("Received: %s and  read %lu bytes \n", buffer, readen);
        }
        int pid=0;
        for (int i = 0;i < readen; i++) {
            pid = pid*10;
            pid+=buffer[i]-'0';
        }

        printf("Este é o PID do filho: %d\n", pid);

        //write to log file the buffer message to save the command and arguments
        //if ((bytes_wrote = write(fd_log,buffer,readen)) == -1) {
        //    printf("[DEBUG] wrote %s to file\n", buffer);
    
    
    // close the pipe
    close(fdfifo);
    printf("Pipe closed \n");
    close(fd_log);

    return 0;
}
