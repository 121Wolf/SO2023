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
    char name[20];
    struct timeval time;
    struct Node* next;
};

struct Node* pidIs(struct Node* head, pid_t pid);
void insertAtBeginning(struct Node* head, pid_t pid,struct timeval time, char *name);
int checkList(struct Node* head, pid_t pid,char *name, struct timeval tempo);
void removeNode(struct Node* head, pid_t pid);
struct Node* createNode(pid_t pid, char *name, struct timeval time);

