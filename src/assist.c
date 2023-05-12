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
struct Node* createNode(pid_t pid) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->pid = pid;
    newNode->next = NULL;
    return newNode;
}

void removeNode(struct Node** head, int value) {
    if (*head == NULL) {
        printf("[DEBUG] List is empty");
        return;
    }

    struct Node* current = *head;
    struct Node* previous = NULL;

    if(current != NULL && current->pid == value){
        *head = current->next;
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

void insertAtBeginning(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}


int checkList(struct Node* head, pid_t pid) {
    struct Node* temp = head;

    while (temp != NULL && temp->pid != pid) {
        temp = temp->next;
    }
    if(temp->pid == pid) {
        removeNode(&head,pid);
        printf("Pid removed from list %d \n",pid);
        return 0;
    }
    else {
        insertAtBeginning(&head,pid);
        printf("Pid added to list %d \n",pid);
        return 1;
        }
}