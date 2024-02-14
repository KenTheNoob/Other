#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 4096

// Define a structure for a stack to implement DFS
struct Stack {
    char* paths[MAX_PATH_LENGTH];
    int top;
};

// Function to initialize the stack
void initializeStack(struct Stack* s) {
    s->top = -1;
}

// Function to check if the stack is empty
bool isStackEmpty(struct Stack* s) {
    return s->top == -1;
}

// Function to push a path onto the stack
void push(struct Stack* s, const char* path) {
    if (s->top == MAX_PATH_LENGTH - 1) {
        fprintf(stderr, "Stack is full. Cannot push any more paths.\n");
    } else {
        s->top++;
        s->paths[s->top] = strdup(path);
    }
}

// Function to pop a path from the stack
char* pop(struct Stack* s) {
    if (isStackEmpty(s)) {
        fprintf(stderr, "Stack is empty. Cannot pop.\n");
        return NULL;
    } else {
        char* path = s->paths[s->top];
        s->top--;
        return path;
    }
}

// Function to perform non-recursive depth-first search
void dfsSearch(const char* startDir) {
    struct Stack s;
    initializeStack(&s);

    push(&s, startDir);

    while (!isStackEmpty(&s)) {
        char* currentDir = pop(&s);
        //printf("Directory: %s\n", currentDir);

        DIR* dir = opendir(currentDir);
        if (dir == NULL) {
            perror("opendir");
            continue;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;  // Skip "." and ".."
            }

            char path[MAX_PATH_LENGTH];
            snprintf(path, sizeof(path), "%s/%s", currentDir, entry->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == -1) {
                perror("stat");
                continue;
            }

            if (S_ISDIR(statbuf.st_mode)) {
                push(&s, path);
            } else if (S_ISREG(statbuf.st_mode)) {
                printf("File: %s\n", path);
            }
        }

        closedir(dir);
        free(currentDir);
    }
}

int main(int argc, char* argv[]) {
    char dirname[1000];
    printf("Enter a directory name: ");
    scanf("%s", dirname);
    dfsSearch(dirname);
    return 0;
}
