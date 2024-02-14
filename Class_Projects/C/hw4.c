//Hexadecimal bucket sort integers
#include <stdio.h>
int main(){
    // Read the amount of characters to sort, accepting only digit inputs(next step line 29)
    int i;
    char ch;
    int count = 0;
    int valid = 1;
    printf("How many integers to sort: "); // Comment out for bash script
    do {
        ch = getchar();
        if (ch >= 48 && ch <= 57) {
            count = count * 10 + (ch - 48);
        }
        else if (ch == '\n' || ch == EOF){
            if(count > 100 || valid == 0){
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
    
    // Read only valid integers(next step line 73)
    int values[100];
    int integer = 0;
    int digit = 0;
    int sign = 1;
    for(i=0; i<count; i++){
        printf("Enter an integer: "); // Comment out for bash script
        do {
            ch = getchar();
            if (ch >= 48 && ch <= 57) {
                integer = integer * 10 + (ch - 48);
                digit++;
            }
            else if (ch == '\n' || ch == EOF){
                if(valid == 0){
                    printf("Invalid integer\n");
                    printf("Enter an integer: ");
                    integer = 0;
                    digit = 0;
                    valid = 1;
                    sign = 1;
                }
                else{
                    values[i] = sign*integer;
                    integer = 0;
                    digit = 0;
                    sign = 1;
                    break;
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
    }

    int buffer[100];
    int counter[16];
    int offset[16];
    int round;
    int bucket_index;
    unsigned int *temp;

    // Create pointers for values and buffer so that the arrays can be swapped
    unsigned int *p=values, *q=buffer;
    // 4B integer = 32bit and each hex is 4bit so 32/4 = 8 hex digits per integer
    // Iterate through the 8 hex digits for each integer
    for(round = 0; round < 8; round++){
        // Reset counter and offset each round
        for(i=0;i<16;i++){
            counter[i] = 0;
        }
        /*
        for(i=0;i<16;i++){
            offset[i] = 0;
        }*/
        offset[0] = 0;
        // Increment count of bucket associated to hex digit
        for (i=0; i<count; i++){
            bucket_index=(p[i]>>(round*4))&0XF;
            counter[bucket_index]++;
        }
        // Figure out buffer offset of each bucket
        for(i=1; i<16; i++){
            offset[i]=offset[i-1]+counter[i-1];
        }
        // Store value in buffer based on buffer offset of its hex digit(bucket)
        for(i=0; i<count; i++){
            bucket_index=(p[i]>>(round*4))&0XF;
            q[offset[bucket_index]++] = p[i];
        }
        // Swap pointers p and q(value and buffer array);
        temp = p;
        p = q;
        q = temp;
    }
    // Swap negative and positive parts of array so the integers are sorted as signed instead of unsigned
    // Since buffer is also sorted(as unsigned ints), we can just use it to put values sorted(as signed ints) into values array(instead of creating another array)
    int neg_index;
    for(neg_index = 0; buffer[neg_index]>=0; neg_index++);
    int j = 0;
    for(i=neg_index; i<count; i++, j++){
        values[j] = buffer[i];
    }
    for(i=0; i<neg_index; i++, j++){
        values[j] = buffer[i];
    }
    // Print out sorted array
    printf("Values:\n"); // Comment out for bash script
    for(i=0; i<count; i++){
        printf("%d\n", values[i]);
    }
    return 0;
}