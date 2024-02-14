//Print hex, dec, and bytes of integers in a binary file
#include <stdio.h>
#include <ctype.h>
int main(int argc, char **argv){
    // Open the matrix file provided as a command line argument
    FILE* matrix_file;
    matrix_file = fopen(argv[1], "rb");
    // End the program if the file cannot be opened
    if (matrix_file == NULL) {
        printf("File cannot be opened");
        return 1;
    }
    else{
        unsigned int dec;
        unsigned int byte1;
        unsigned int byte2;
        unsigned int byte3;
        unsigned int byte4;
        int c;
        while(!feof(matrix_file)){
            // Read 4 bytes as an integer
            fread(&dec, sizeof(unsigned int), 1, matrix_file);
            printf("Integer: Hex: 0x%08X Dec: %d\n", dec, dec);
            // Move the file offset back 4 bytes
            fseek(matrix_file, -4, SEEK_CUR);
            // Read the bytes again 1 byte at a time
            byte4 = fgetc(matrix_file);
            byte3 = fgetc(matrix_file);
            byte2 = fgetc(matrix_file);
            byte1 = fgetc(matrix_file);
            printf("Byte 1: Hex: 0x%02X Char: ", byte1);
            if(isprint(byte1)){
                printf("%c\n", byte1);
            }
            else{
                printf("\\%d\n", byte1);
            }
            printf("Byte 2: Hex: 0x%02X Char: ", byte2);
            if(isprint(byte2)){
                printf("%c\n", byte2);
            }
            else{
                printf("\\%d\n", byte2);
            }
            printf("Byte 1: Hex: 0x%02X Char: ", byte3);
            if(isprint(byte3)){
                printf("%c\n", byte3);
            }
            else{
                printf("\\%d\n", byte3);
            }
            printf("Byte 4: Hex: 0x%02X Char: ", byte4);
            if(isprint(byte4)){
                printf("%c\n", byte4);
            }
            else{
                printf("\\%d\n", byte4);
            }
            // Since the file reads 4 bytes integers at a time, it may read EOF as int 0 so check for EOF
            if(fgetc(matrix_file) == EOF){
                break;
            }
            else{
                fseek(matrix_file, -1, SEEK_CUR);
            }
        }
    }
    fclose(matrix_file);
    return 0;
}