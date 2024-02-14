//Hexadecimal bucket sort for floats
#include <stdio.h>
#include <stdlib.h>
int main(){
    // Read the amount of floats to sort, accepting only digit inputs
    int i;
    char ch;
    int count = 0;
    int valid = 1;
    printf("How many floats to sort: "); // Comment out for bash script
    do {
        ch = getchar();
        if (ch >= 48 && ch <= 57) {
            count = count * 10 + (ch - 48);
        }
        else if (ch == '\n' || ch == EOF){
            if(valid == 0){
                printf("Invalid amount\n");
                printf("How many integers to sort: ");
                count = 0;
                valid = 1;
            }
            else
                break;
        }
        else{
            valid = 0;
        }
    } while (1); 

    // Allocate memory to values and buffer using malloc/calloc
    float *values = (float *)malloc(count*sizeof(float));
    float *buffer = (float *)calloc(count, sizeof(float));
    // Read an amount of floats based on count
    for(i=0; i<count; i++){
        printf("Enter a float: "); // Comment out for bash script
        if(scanf("%f", values+i) != 1){
            printf ("Error: Invalid input");
            return 1;
        }
    }

    int counter[16];
    // Array of pointers to mark where to put values in buffer array
    float *offset[16];
    int round;
    int bucket_index;
    float *temp;
    // 4B float = 32bit and each hex is 4bit so 32/4 = 8 hex digits per float
    // Iterate through the 8 hex digits for each float
    for(round = 0; round < 8; round++){
        // Reset counter and offset each round
        for(i=0;i<16;i++){
            counter[i] = 0;
        }
        offset[0] = buffer;
        /*
        for(i=0;i<16;i++){
            offset[i] = buffer;
        }*/
        // Increment count of bucket associated to hex digit
        for (i=0; i<count; i++){
            // Typecast each float to an unsigned int and apply mask
            bucket_index=(*((unsigned int *)values+i)>>(round*4))&0XF;
            counter[bucket_index]++;
        }
        // Figure out address to store each bucket(pointer of buffer+offset)
        for(i=1; i<16; i++){
            offset[i]=offset[i-1]+counter[i-1];
        }
        // Store value in buffer based on buffer offset of its hex digit(bucket)
        for(i=0; i<count; i++){
            // Typecast each float to an unsigned int and apply mask
            bucket_index=(*((unsigned int *)values+i)>>(round*4))&0XF;
            // Store sorted floats in buffer based on address indicated by array of pointers(offset)
            *offset[bucket_index]++ = values[i];
        }
        // Swap pointers value and buffer arrays(pointers)
        temp = values;
        values = buffer;
        buffer = temp;
    }
    // Swap negative and positive parts of array so the integers are sorted as signed instead of unsigned
    // Set buffer equal to values instead of creating a new copy array for values
    for(i=0;i<count;i++){
        buffer[i]=values[i];
    }
    int neg_index;
    for(neg_index = 0; buffer[neg_index]>=0.0; neg_index++);
    int j = 0;
    for(i=count-1; i>=neg_index; i--, j++){
        values[j] = buffer[i];
    }
    for(i=0; i<neg_index; i++, j++){
        values[j] = buffer[i];
    }
    // Print out sorted array
    printf("\nValues:\n"); // Comment out for bash script
    for(i=0; i<count; i++){
        printf("%.2f\n", values[i]);
    }
    // Free memory allocated to value and buffer arrays
    free(values);
    free(buffer);
    return 0;
}