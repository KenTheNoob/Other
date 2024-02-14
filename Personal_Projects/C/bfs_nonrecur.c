#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 4096

// Define a structure for a queue to implement BFS
struct Queue {
    char* paths[MAX_PATH_LENGTH];
    int front;
    int rear;
};

// Function to initialize the queue
void initializeQueue(struct Queue* q) {
    q->front = -1;
    q->rear = -1;
}

// Function to check if the queue is empty
bool isQueueEmpty(struct Queue* q) {
    return q->front == -1;
}

// Function to enqueue a path
void enqueue(struct Queue* q, const char* path) {
    if (q->rear == MAX_PATH_LENGTH - 1) {
        fprintf(stderr, "Queue is full. Cannot enqueue any more paths.\n");
    } else {
        if (q->front == -1) {
            q->front = 0;
        }
        q->rear++;
        q->paths[q->rear] = strdup(path);
    }
}

// Function to dequeue a path
char* dequeue(struct Queue* q) {
    if (isQueueEmpty(q)) {
        fprintf(stderr, "Queue is empty. Cannot dequeue.\n");
        return NULL;
    } else {
        char* path = q->paths[q->front];
        q->front++;
        if (q->front > q->rear) {
            q->front = q->rear = -1;
        }
        return path;
    }
}

// Function to perform breadth-first search
void bfsSearch(const char* startDir) {
    struct Queue q;
    initializeQueue(&q);

    enqueue(&q, startDir);

    while (!isQueueEmpty(&q)) {
        char* currentDir = dequeue(&q);
        char path[1024];
        //printf("Directory: %s\n", currentDir);
        DIR* dir = opendir(currentDir);
        if (dir == NULL) {
            perror("opendir");
            continue;
        }
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            sprintf(path, "%s/%s", currentDir, entry->d_name);
            struct stat statBuf;
            stat(path, &statBuf);
            mode_t mode = statBuf.st_mode;
            if (S_ISDIR(mode)) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    enqueue(&q, path);
                }
            } else if(S_ISREG(mode)){
                //printf("File: %s/%s\n", currentDir, entry->d_name);
                printf("%s\n",path);
            }
        }

        closedir(dir);
        free(currentDir);
    }
}

int main(int argc, char* argv[]) {
    /*
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    bfsSearch(argv[1]);
*/
    char dirname[1000];
    printf("Enter a directory name: ");
    scanf("%s", dirname);
    bfsSearch(dirname);
    return 0;
}
