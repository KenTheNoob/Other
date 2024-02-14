#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
void traverse(char *pathName ) {/* traverse a directory */
    struct stat statBuf;
    mode_t mode;
    int result;
    char fileName[1024];
    result = stat(pathName, &statBuf); /* Obtain file status */
    if ( result == -1 ) { /* Status was not available */
        fprintf( stderr, "Cannot stat %s \n", pathName );
        return;
    }
    mode = statBuf.st_mode; /* Mode of file */
    if(S_ISREG(mode)){ /* Regular file */
        //printf("%s: size %lu bytes, mod. time = %s",pathName, statBuf.st_size,asctime(localtime(&statBuf.st_mtime)));
        printf("%s\n",pathName);
    }
    else if (S_ISDIR(mode)){ /* Directory */
        //printf("Entering directory %s\n", pathName);
        DIR *pDir;
        struct dirent *pEnt;
        pDir=opendir(pathName); /* Open for reading */
    while((pEnt = readdir(pDir)) != NULL){
        if ( strcmp(pEnt->d_name, ".") != 0 && strcmp( pEnt->d_name, "..") != 0){
            sprintf( fileName, "%s/%s", pathName, pEnt->d_name );
            traverse(fileName);
        }
    }
    closedir(pDir);
    }
}
int main(int argc, char **argv) {
    char dirname[1000];
    printf("Enter a directory name: ");
    scanf("%s", dirname);
    traverse(dirname);
}