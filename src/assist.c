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

/*
int parserinput(char * input_string){
    if (strncmp(input_string, "execute -u", 10) == 0) {
        return 1;
    } else if (strcmp(input_string, "status") == 0) {
        return 2;
    } else {
        return 0;
    }
}
*/


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
    printf("A criar novos nos\n");
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->pid = pid;
    strncpy(newNode->name, name, sizeof(newNode->name) - 1); //n sei se está certo
    newNode->time = time;
    newNode->next = NULL;
    printf("Este é o novo nó %d\n",newNode->pid);
    return newNode;
}

void removeNode(struct Node* head, pid_t value) {
    if (head == NULL) {
        return;
    }

    struct Node* current = head;
    struct Node* previous = NULL;

    if(current != NULL && current->pid == value){
        head = current->next;
        free(current);
        if (head == NULL) printf("Teste 1\n");
        printf("Aconteceu isto\n");
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

struct Node * insertAtBeginning(struct Node** head, pid_t pid, struct timeval time, char *name) {
    struct Node* newNode = createNode(pid,name, time);
    newNode->next = *head;
    return newNode;
}
struct Node* pidIs(struct Node* head, pid_t pid){ //returns a node from linked list if the process is alreadty there. Returns null otherwise
     struct Node* temp = head;
     while (temp != NULL && temp->pid != pid)
        temp = temp->next;

    /*NULL -> pid*/
    if (temp == NULL) {
        head = NULL;
        printf("Afinal ficou NULL\n");
        return NULL;
    }
    else {
        removeNode(head,pid);
        if (head == NULL)
            printf("É o suposto de acontecer\n");
        return temp;
    } 
}

struct Node* checkList(struct Node** head, pid_t pid,char *name, struct timeval tempo) {
    struct Node* temp = *head;
    while (temp != NULL && temp->pid != pid)
        temp = temp->next;
    if (*head == NULL) printf("È o suposto numa vazia\n");
    if (temp == NULL) {
        temp = insertAtBeginning(head,pid,tempo,name);
        printf("Este é o Temp %d\n",temp->pid);
        return temp;
    }
    return NULL;
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
    printf("%s\n", strings[4]);
    while ((tmp = strings[index++]) != NULL) {
        write(fileDesciptor, tmp, strlen(tmp) * sizeof(char));
    }
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