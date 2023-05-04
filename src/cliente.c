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
    int fd;
    char *message = "Hello, server!\n";
    int bytes_written;
    char *command = "ls";
    //int status = 0;


    // open pipe for writing
    fd = open(PIPE_NAME, O_WRONLY);
    if(fd == -1) printf("pila");

   //execl("/bin/ls", command, NULL);
    for (int i = 0; i < 5; i++) {
        // Guardar o pid do filho
        pid_t pid=fork();
        switch(pid) {
            case 0:
                execl("/bin/ls",command,NULL);
                _exit(0);
            default:
                printf("Fork successful %d\n",pid);
        }
    }

    // write message to pipe
    bytes_written = write(fd, message, strlen(message) );

    printf("Sent %d bytes to server\n", bytes_written);

    // close the pipe
    close(fd);

    return 0;
}