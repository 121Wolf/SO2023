#include "assist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdlib.h>

int parserinput(char * input_string){
    if (strncmp(input_string, "execute -u", 10) == 0) {
        return 1;
    } else if (strcmp(input_string, "status") == 0) {
        return 2;
    } else {
        return 0;
    }
}



void stringToData(char* string, pid_t activepids[], char* name[], struct timeval stop[], int* size) {
    int tempSize;
    sscanf(string, "Number of elements: %d\n", &tempSize);
    int counter = strlen("Number of elements: %d\n");
    for (int i = 0; i < tempSize; i++) {
        sscanf(string + counter, "Active PID[%*d]: %d\n", &activepids[i]);
        counter += strlen("Active PID[%*d]: %d\n");
    }
    
    for (int i = 0; i < tempSize; i++) {
       sscanf(string + counter, "Name[%*d]: %[^\n]\n", name[i]);
        counter += strlen("Name[%*d]: %[^\n]\n");
    }
    
    for (int i = 0; i < tempSize; i++) {
        sscanf(string + counter, "Stop time[%*d]: %ld seconds, %ld microseconds\n", &stop[i].tv_sec, &stop[i].tv_usec);
        counter += strlen("Stop time[%*d]: %ld seconds, %ld microseconds\n");
    }
    
    *size = tempSize;
}




void dataToString(pid_t activepids[], char* name[], struct timeval stop[], int size, char* string) {
    int counter = 0;
    counter += sprintf(string + counter, "Number of elements: %d\n", size);
    for (int i = 0; i < size; i++) {
        counter += sprintf(string + counter, "Active PID[%d]: %d\n", i, activepids[i]);
    }
    for (int i = 0; i < size; i++) {
        counter += sprintf(string + counter, "Name[%d]: %s\n", i, name[i]);
    }
    for (int i = 0; i < size; i++) {
        counter += sprintf(string + counter, "Stop time[%d]: %ld seconds, %ld microseconds\n", i, stop[i].tv_sec, stop[i].tv_usec);
    }
}



// Creates a new node in the linked list
struct Node* createNode(pid_t pid, char *name,struct timeval time) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->pid = pid;
    strncpy(newNode->name, name, sizeof(newNode->name) - 1); //n sei se está certo
    newNode->time = time;
    newNode->next = NULL;
    return newNode;
}

void removeNode(struct Node* head, pid_t value) {
    if (head == NULL) {
        printf("[DEBUG] List is empty");
        return;
    }

    struct Node* current = head;
    struct Node* previous = NULL;

    if(current != NULL && current->pid == value){
        head = current->next;
        free(current);
        return;
    }
    while (current != NULL && current->pid != value) {
        previous = current;
        current = current->next;
    }
    if(current->pid == value){
        previous->next = current->next;
        free(current);
        return;
    }
}

void insertAtBeginning(struct Node* head, pid_t pid, struct timeval time, char *name) {
    struct Node* newNode = createNode(pid,name, time );
    newNode->next = head;
    head = newNode;
}
struct Node* pidIs(struct Node* head, pid_t pid){ //returns a node from linked list if the process is alreadty there. Returns null otherwise
     struct Node* temp = head;
     while (temp != NULL && temp->pid != pid)
        temp = temp->next;

     if ( temp->pid == pid){
            removeNode(head,pid);
            return temp;
    } 
        else return NULL;
}

int checkList(struct Node* head, pid_t pid,char *name, struct timeval tempo) {
    struct Node* temp = head;
    while (temp != NULL && temp->pid != pid)
        temp = temp->next;

    if (temp == NULL) {
        insertAtBeginning(head,pid,tempo,name);
        return 0;
        }
    else if(temp->pid == pid){
        removeNode(head,pid);
        return 1;}
        return 3;
}

char* longToString(long arg) {
    long exponential = 1;
    int index = 0;
    char* message = malloc(256 * sizeof(char));

    // Alcança o exponencial de 10^N maior que pid
    while (exponential < arg)
        exponential *= 10;

    // Torna o exponencial em 10^(N-1)
    exponential /= 10;

    
    while (exponential != 0) {
        message[index++] = (arg/exponential) + '0';
        arg %= exponential;
        exponential /= 10;
    }

    // Acaba a mensagem com '\0'
    message[index] = '\0';

    return message;
}

void writeStrings(int fileDesciptor, char** strings) {
    int index = 0;
    char *tmp;

    while ((tmp = strings[index++]) != NULL) {
        write(fileDesciptor, tmp, strlen(tmp) * sizeof(char));
    }
    write(fileDesciptor, "!", 2);
}

void time_format(struct timeval tv, char** logMessage) {
    logMessage[10] = longToString(tv.tv_usec);
    logMessage[8] = longToString(tv.tv_sec % 60);
    logMessage[6] = longToString((tv.tv_sec / 60) % 60);
    logMessage[4] = longToString((tv.tv_sec / 3600) % 24);
    long hour = (tv.tv_sec / 3600); 
    logMessage[2] = longToString(((hour / 24) % 365));
    logMessage[0] = longToString(1970 + (hour / 8766));
}

void freeLogPointers (char** logMessage) {
    free(logMessage[10]);
    free(logMessage[8]);
    free(logMessage[6]);
    free(logMessage[4]);
    free(logMessage[2]);
    free(logMessage[0]);
}