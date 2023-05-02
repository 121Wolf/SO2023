#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPE_NAME "my_pipe"

int main() {
    size_t fdfifo, fd_log;
    char buffer[256];
    int bytes_read;

   // buffer = (char *) malloc(256);
    // create named pipe
    if(mkfifo(PIPE_NAME, 0666) == -1){
        perror("MKFIFO");
    }
    //open log.txt
    if ((fd_log = open("log.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1) {
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
    printf("[DEBUG] Server listening... fdfifo %ld \n",fdfifo);

    // read from pipe and print to console
    while ((bytes_read = read(fdfifo, buffer, sizeof(buffer))) > 0) {
        printf("Received: %s\n", buffer);
    }
        //write to log file the buffer message to save the command and arguments
        write(fd_log,buffer,bytes_read);
        write(fd_log, "\n", 1);
        printf("[DEBUG] wrote %s to file\n", buffer);
    // close the pipe
    close(fdfifo);
    printf("Pipe closed \n");
    close(fd_log);

    // remove the named pipe
    unlink(PIPE_NAME);

    return 0;
}
