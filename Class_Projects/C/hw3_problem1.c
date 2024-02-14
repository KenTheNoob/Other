//writes simplified sparse matrix representation of a bin file to a txt file
// ./hw3_problem1 matarix.bin
// matrix.bin is a binary file of integers with first 2 integers being dimensions of the matrix
#include <stdio.h>
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
        // Read the amount of rows and columns from the file
        int row;
        int col;
        fread(&row, sizeof(unsigned int), 1, matrix_file);
        fread(&col, sizeof(unsigned int), 1, matrix_file);
        // Max amount of values for a sparse matrix is elements/2 plus 1 for rounding
        unsigned int values[(col*row)/2+1];
        int column_indices[(col*row)/2+1];
        int row_start[row+1];
        int row_start_i = 0;
        // num stores each read integer and nonzero_i stores how many non zero elements were read
        unsigned int num;
        int nonzero_i = 0;
        // new_row tells whether to assign a nonzero value to row_start
        int new_row = 1;
        int i;
        // Loop through each integer in the matrix. Ignores any bytes after the matrix
        for(i=0; i<row*col; i++){
            fread(&num, sizeof(unsigned int), 1, matrix_file);
            // Allow the next nonzero integer to be stored in row_start every new row
            if(i%col==0 && i!=0){
                printf("\n");
                // If a row has no nonzero integers, the row start index is the same as the previous row
                if(new_row == 1){
                    row_start[row_start_i++] = nonzero_i;
                }
                new_row = 1;
            }
            printf("%d ", num);
            // Store info for each nonzero integer accordingly
            if (num != 0){
                values[nonzero_i] = num;
                column_indices[nonzero_i] = i%col;
                if(new_row == 1){
                    row_start[row_start_i++] = nonzero_i;
                    new_row = 0;
                }
                nonzero_i++;
            }
        }
        row_start[row_start_i] = nonzero_i;
        fclose(matrix_file);

        // Print out arrays for testing purposes else goto line 74
        // Check if next char is EOF
        char ch = fgetc(matrix_file);
        if(ch == EOF){
            printf("\nEnd of matrix\n");
        }
        printf("dimensions %d %d\nvalues ", row, col);
        for(i = 0; i < nonzero_i; i++){
            printf("%d ", values[i]);
        }
        printf("\ncolumn_indices ");
        for(i = 0; i < nonzero_i; i++){
            printf("%d ", column_indices[i]);
        }
        printf("\nrow_start ");
        for(i = 0; i < row+1; i++){
            printf("%d ", row_start[i]);
        }

        FILE *matrix_write = fopen("matrix.txt", "w");
        if (matrix_write == NULL) {
                perror("Error creating file");
                return 1;
        }
        fprintf(matrix_write, "dimensions %d %d\nvalues", row, col);
        for(i = 0; i < nonzero_i; i++){
            fprintf(matrix_write, " %d", values[i]);
        }
        fprintf(matrix_write, "\ncolumn_indicies");
        for(i = 0; i < nonzero_i; i++){
            fprintf(matrix_write, " %d", column_indices[i]);
        }
        fprintf(matrix_write, "\nrow_start");
        for(i = 0; i < row+1; i++){
            fprintf(matrix_write, " %d", row_start[i]);
        }
        fclose(matrix_write);
    }
    return 0;
}