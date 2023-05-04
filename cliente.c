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
    char message[256];
    char userinput[256];
    char *endmessage = "Coisas aconteceram bro";
    int bytes_written, bytes_written2;
    int bytesread;
    char *command = "ls";
    pid_t pid;
    int status;

     // open pipe for writing
    if((fd = open(PIPE_NAME, O_WRONLY)) == -1){
        perror("FIFO");
    }
          if( bytesread = read(STDIN_FILENO,userinput,sizeof(userinput)) == -1){
          perror("Userinput:");
        }
            commandparser(userinput,bytesread);
        

    if ((pid = fork()) == 0) {
        printf("This is child PID speaking");
		//execl("/bin/ls", "/bin/ls", "-l", NULL);
		_exit(0);
}

    else{
        while(1){
             if( bytesread = read(STDIN_FILENO,userinput,sizeof(userinput)) == -1){
          perror("Userinput:");
        }
            commandparser(userinput,bytesread);
        }
    }

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
    // write message to pipe
    if((bytes_written = write(fd, message, strlen(message))) == -1){
        perror("FIFO Write:");
    }
    
    printf("[DEBUG] Sent %d bytes to server\n", bytes_written);
    
    //waits for child PID _exit(0)
    pid_t terminated_pid = wait(&status);
    
    /*if(bytes_written2 = write(fd,endmessage,strlen(endmessage)) == -1){
        perror("A comunicação falhou devido:");
    }
    printf("[DEBUG] Sent %d bytes to server\n", bytes_written2);*/
    }
    // close the pipe 
    close(fd);

    return 0;
}