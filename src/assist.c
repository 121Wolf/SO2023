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