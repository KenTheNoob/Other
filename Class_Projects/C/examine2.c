//Prints out memory addresses of program arguments
#include <stdio.h>
#include <ctype.h>
int main(int argc, char *argv[]){
	// This file still works for machines that use 4B pointers or does not store argument strings adjacently(more complex code)
	unsigned char *p;
	// pointers can be 8B or 4B depending on the machine
	int ptrsize = sizeof(void *);
	int i;
	int j;
	unsigned int c;
	// argv variable
	printf("argv\t| ");
	// p is the address of argv. Argv is a pointer containing the address of argv[0], the first element of an array of pointers
	p = (unsigned char *)&argv;
	// loop through each byte contained in argv which are the bytes of the address of argv[0]
	for (i=ptrsize-1; i>=0; i--){
		printf("%02hhx ", *(p+i));
	}
	printf(" | %p\n\n", p);
	// array of pointers
	for(i=0; i<argc; i++){
		printf("argv[%d]\t| ", i);
		// p is the address of argv[i]. Argv[i] is a pointer containing the address of the first character of an array of characters(a null terminated string)
		p = (unsigned char *)&argv[i];
		// loop through each byte contained in argv[i] which are the bytes of the address of the first character of argument i(a string)
		for (j=ptrsize-1; j>=0; j--){
			printf("%02hhx ", *(p+j));
		}
		printf(" | %p\n", p);
	}
	printf("\n");
	// command line parameters
	// keeps track of the last line printed
	unsigned char *prev;
	// keeps track of when to stop printing an argument
	unsigned char *end;
	// if each argv[i] is adjacent in memory, there is no need to loop through each argument since incrementing past string argv[i] becomes the start of string argv[i+1](giving you the next argument automatically)
	// in linux, each argv[i] is adjacent in memory, in windows, you need to recalculate starting address of each argument since they are not adjacent
	for(i=0; i<argc; i++){
		// p is the address of the first character of the ith argument
		// start before the first byte in the ith command line argument
        	p = (unsigned char *)argv[i];
		// move the pointer to the closest lower address(8B/4B aligned)
		p = p-(unsigned long int)p%ptrsize;
		// if the line has already been printed, start printing from the next line
		if(p == prev){
			p += ptrsize;
		}	
		// end after the last byte in the ith command line argument(8B/4B aligned)
		end = (unsigned char *)argv[i];
		for(;end[0] != '\0';end++);
		// loop until the argument has been fully printed out
		while(p<=end){
			printf("\t| ");
			// print out a line at a time (8B/4B)
			for(j=ptrsize-1; j>=0; j--){
				c = p[j];
				printf("%02x(", c);
				// show escape sequences if the character is not printable
				if(isprint(c)){
					printf("%c) ", c);
				}
				else{
					printf("\\%d) ", c);
				}
			}
			printf("| %p\n", p);
			// increment p by 8B/4B to move to the next line
			p += ptrsize;
		}
		// record the last line printed
		prev = p-ptrsize;
	}
	return 0;
}