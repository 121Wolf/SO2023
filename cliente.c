#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPE_NAME "my_pipe"

int main() {
    int fd;
    char *message = "Hello, server!";
    int bytes_written;
    char *command = "ls";

    // open pipe for writing
    fd = open(PIPE_NAME, O_WRONLY);
    if(fd == -1) printf("pila");

   //execl("/bin/ls", command, NULL);

    // write message to pipe
    bytes_written = write(fd, message, strlen(message) + 1);

    printf("Sent %d bytes to server\n", bytes_written);

    // close the pipe
    close(fd);

    return 0;
}