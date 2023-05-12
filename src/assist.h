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


// Define the structure for a node in the linked list
struct Node {
    pid_t pid;
    struct Node* next;
};


void insertAtBeginning(struct Node* head, int data);
int checkList(struct Node* head, pid_t pid);
void removeNode(struct Node* head, int value);
struct Node* createNode(pid_t pid);

