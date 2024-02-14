//Parallel processing PI approximation
//Only on linux
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
long nthreads, terms; // Decided to make variables global so I don't have to pass structures into functions
long double pi = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *thread_function(void * id){
	// Calculate start and end of the chunk for the thread
	long *myid = (long *)id;
	long chunk = terms/nthreads;
	// Start summation from 1 not 0
	long start = (*myid)*chunk+1;
	long end = start+chunk;
	// include last term
	if ((*myid) == nthreads - 1){
		end = terms+1;
	}
	// Calculate the partial_sum
	long double partial_sum = 0.0;
	long double x;
	for(long i = start; i < end; i++) {
		x = ((double)i - 0.5)/terms;
		partial_sum += 4.0/(1.0 + x*x);
	}
	// Prevent race condition
	pthread_mutex_lock(&mutex);
	pi += partial_sum;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
int main(int argc, char *argv[])
{
	int i;
	// Value of PI to compare approximation to
	long double PI = 3.14159265358979323846264338327950288419716939937510;
	if (argc !=3) {
		fprintf(stderr, "usage: %s <m> <n>\n", argv[0]);
		fprintf(stderr, "where <m> is the number of theads and <n> is the number of terms\n");
		return 1;
	}
	nthreads = strtol(argv[1], NULL, 10);
	terms = strtol(argv[2], NULL, 10);
	// Create threads
	pthread_t *thread_array = malloc(nthreads * sizeof(pthread_t));
	long *thread_ids = malloc(nthreads * sizeof(long));
	for (i = 0; i < nthreads; i++) {
		thread_ids[i] = i;
		pthread_create(&thread_array[i], NULL, thread_function, &thread_ids[i]);
	}
	// Join threads
	for (i = 0; i < nthreads; i++){
		pthread_join(thread_array[i], NULL);
	}
	// Print out approximation
	pi/=(terms);
	printf("pi is approximately %.22Lf, Error is %.22Lf\n",pi, fabsl(pi - PI));
	free(thread_array);
	free(thread_ids);
	return 0;
}
