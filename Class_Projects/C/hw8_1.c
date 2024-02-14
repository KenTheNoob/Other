//Parallel processing int sort
//Only on linux
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv){
	int rank, numprocs, i, num_ints;
	int *integers = NULL;
	FILE *fptr = NULL;
	char path[1000];
	int counts[1000];
	for(i = 0; i<1000; i++){
		counts[i] = 0;
	}
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	// Code for master
	if (rank == 0){
		// enter path ./file5kvalues
		scanf("%s",path);
		fptr = fopen(path, "rb");
		if(fptr == NULL){
			printf("File \"%s\" cannot be opened", path);
			return 1;
		}
		// Figure out how many ints to sort so you know how much space to allocate to int array
		fseek(fptr, 0, SEEK_END);
		num_ints = ftell(fptr)/4;
		rewind(fptr);
		integers = (int *)malloc(num_ints*sizeof(int));
		// Read integers from file into int array
		for(i = 0; i<num_ints; i++){
			fread(integers+i, sizeof(int), 1, fptr);
		}
		fclose(fptr);
	}
	// Broadcast num_ints to workers so each worker can initialize the int array
	MPI_Bcast(&num_ints, 1 , MPI_INT, 0, MPI_COMM_WORLD);
	if(rank > 0){
		integers = (int *)malloc(num_ints*sizeof(int));
	}
	// Broadcast the int array to each worker
	MPI_Bcast(integers, num_ints, MPI_INT, 0, MPI_COMM_WORLD);
	// Each process counts sorts a chunk of the int array
	int partial_counts[1000];
	for(i = 0; i<1000; i++){
		partial_counts[i] = 0;
	}
	// Assume number of integers is divisible by number of processors(no dealing with remainder)
	for(i=num_ints/numprocs*rank; i<num_ints/numprocs*(rank+1); i++) {
		partial_counts[integers[i]]++;
	}
	// Combine partial counts from workers into total count for master
	MPI_Reduce(partial_counts, counts, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	// Code for master to write to fifle
	if(rank == 0){
		fptr = fopen(path, "wb");
		int j;
		for(i=0; i<1000; i++){
			for(j = 0; j<counts[i]; j++){
				fwrite(&i, sizeof(int), 1, fptr);
			}
		}
		fclose(fptr);	
	}
	free(integers);
	MPI_Finalize();
	return 0;
}
