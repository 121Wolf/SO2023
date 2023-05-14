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

char* longToString(long arg);

void writeStrings(int fileDescriptor, char** strings);

void time_format(struct timeval tv, char** logMessage);

void freeLogPointers (char** logMessage);

void dataToString(pid_t activepids[], char* name[], struct timeval stop[], int size, char* str);
struct Node* pidIs(struct Node* head, pid_t pid);
struct Node* insertAtBeginning(struct Node* head, pid_t pid,struct timeval time, char *name);
struct Node* checkList(struct Node* head, pid_t pid,char *name, struct timeval tempo);
void removeNode(struct Node* head, pid_t pid);
struct Node* createNode(pid_t pid, char *name, struct timeval time);

