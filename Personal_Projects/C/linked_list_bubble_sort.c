//Asks for integers until an invalid one is provided and sorts them
#include <stdio.h>
#include <stdlib.h>
struct node {
	int data;
	struct node *next;
};
struct node * node_head = NULL;
struct node * prev = NULL;
int main(){
    char ch;
    int integer = 0;
    int digit = 0;
    int sign = 1;
    int valid = 1;
    printf("Enter an integer: ");
    do {
        ch = getchar();
        if (ch >= 48 && ch <= 57) {
            integer = integer * 10 + (ch - 48);
            digit++;
        }
        else if (ch == '\n' || ch == EOF){
            if(valid == 0){
                integer = 0;
                digit = 0;
                valid = 1;
                sign = 1;
                break;
            }
            else{
                struct node* n = (struct node *) malloc(sizeof(struct node));
                n->next = NULL;
                n->data = sign*integer;
                if(node_head == NULL){
                    node_head = n;
                }
                if(prev != NULL){
                    prev->next = n;
                }
                prev = n;
                integer = 0;
                digit = 0;
                sign = 1;
                printf("Enter an integer: ");
            }
        }
        else{
            if (ch == 45 && digit == 0){
                digit++;
                sign = -1;
            }
            else if (ch == 43 && digit == 0){
                digit++;
            }
            else
                valid = 0;
        }
    } while (1);
    struct node * curr_state = node_head;
    int nodes = 0;
    while(curr_state->next != NULL){
        nodes++;
        curr_state = curr_state->next;
    }
    printf("Nodes: %d\n", nodes);
    struct node * prev = NULL;
    struct node * dummy = NULL;
    for(int i = 0; i<nodes; i++){
        for(int j = 0; j<nodes-i; j++){
            prev = NULL;
            curr_state = node_head;
            for(int k = 0; k<j; k++){
                prev = curr_state;
                curr_state = curr_state->next;
            }
            //printf("swap %d and %d\n", curr_state->data, curr_state->next->data);
            if(curr_state->data > curr_state->next->data){
                if(prev == NULL){
                    dummy = curr_state->next;
                    curr_state->next = curr_state->next->next;
                    dummy->next = curr_state;

                    node_head = dummy;
                }
                else{
                    dummy = curr_state->next;
                    curr_state->next = curr_state->next->next;
                    dummy->next = curr_state;

                    prev->next = dummy;
                }
            }
        }
    }

    //struct node * curr_state = node_head;
    curr_state = node_head;
    printf("Sorted Data:\n");
    while(curr_state != NULL){
        printf("%d ", curr_state->data);
        curr_state = curr_state->next;
    }
}