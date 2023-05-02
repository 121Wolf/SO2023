#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PIPE_NAME "my_pipe"

int main() {
    size_t fd;
    char buffer[256];
    int bytes_read;

   // buffer = (char *) malloc(256);
    // create named pipe
    if(mkfifo(PIPE_NAME, 0666) == -1){
        printf("Something is wrong");
    }

    // open pipe for reading
    fd = open(PIPE_NAME, O_RDONLY);

    printf("Server listening... fd %ld \n",fd);

    // read from pipe and print to console
    while ((bytes_read = read(fd, buffer, sizeof(char))) > 0) {
        printf("Received: %s\n", buffer);
    }

    // close the pipe
    close(fd);
    printf("Pipe closed \n");

    // remove the named pipe
    unlink(PIPE_NAME);

    return 0;
}
