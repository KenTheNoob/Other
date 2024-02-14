//print all binary representation of unsigned ints from 0 to max
#include <stdio.h>
#define max 50
int main() {
    unsigned int number;
    unsigned int i, j, mask;
    number = max;
    for (i=0;i<max+1;i++){
        printf("%u:\t", number);
        mask = 1<<31;
        for (j=0; j<32; j++) {
            printf("%u", (number & mask)==0?0:1);
            mask = mask >> 1;
        }
        /*
        for(int j = 31; j>=0; j--){
            printf("%d", (number>>j)&1);
        }
        */
        printf("\n");
        number--;
    }
    return 0;
}