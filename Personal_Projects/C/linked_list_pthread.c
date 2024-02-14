//Creates a linked list using threads
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
long nthreads;
long length;
struct node {
    int data;
    struct node *next;
};
struct node **node;
pthread_mutex_t mutex;
void *thread_func(void *id){
    long tid = *((long *)id);
    long chunk = length/nthreads;
    long start = tid*chunk;
    long end = start+chunk;
    if(tid == nthreads-1){
        end = length;
    }
    struct node *head = NULL;
    struct node *tail = head;
    struct node *prev;
    for(int i = start; i<end; i++){
        tail = malloc(sizeof(struct node));
        tail->data = i;
        tail->next = NULL;
        if(head == NULL){
            head = tail;
            prev = head;
        }
        else{
            prev->next = tail;
            prev = tail;
        }
    }
    pthread_mutex_lock(&mutex);
    node[tid] = head;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main(int argc, char **argv){
    if(argc != 3){
        printf("Invalid arguments. Provide a number of threads and linked list length");
        return(1);
    }
    nthreads = strtol(argv[1], NULL, 10);
    length = strtol(argv[2], NULL, 10);
    if(length < nthreads){
        printf("Invalid arguments. More threads than lenght of linked list");
        return(1);
    }
    pthread_t *thread_array = (pthread_t *)malloc(sizeof(pthread_t)*nthreads);
    long *thread_id = (long *)malloc(sizeof(long)*nthreads);
    node = malloc(sizeof(struct node *)*nthreads);
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i<nthreads; i++){
        thread_id[i] = i;
        pthread_create(&thread_array[i], NULL, thread_func, &thread_id[i]);
    }
    for(int i = 0; i<nthreads; i++){
        pthread_join(thread_array[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    free(thread_array);
    free(thread_id);
    struct node* head = node[0];
    struct node* curr = head;
    for(int i = 1; i<nthreads; i++){
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = node[i];
    }
    curr = head;
    while(curr != NULL){
        printf("%d ", curr->data);
        curr = curr->next;
    }
    return(0);
}