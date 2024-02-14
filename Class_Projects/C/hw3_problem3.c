//Count total lines of files in a directory
// ./hw3_problem3 TestDir
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
int traverse(char *pathName){
    // Make sure lines is static so recursion does not reset it
    static int lines = 0;

    struct stat statBuf;
    mode_t mode;
    DIR *pDir;
    struct dirent *pEnt;
    char fileName[1024];
    if (stat(pathName, &statBuf) == -1) {
        fprintf( stderr, "Cannot stat %s \n", pathName );
        return -1;
    }
    mode = statBuf.st_mode;
    // If regular file
    if(S_ISREG(mode)){
        // Check if the filename extension is .txt
        char *extension = strchr(pathName, '.');
        // Open each .txt file
        if(strcmp(extension, ".txt") == 0){
            FILE* txt_file;
            txt_file = fopen(pathName, "r");
            char c;
            // Each \n and EOF counts as a line
            while(!feof(txt_file)){
                c = fgetc(txt_file);
                if(c == '\n' || c == EOF){
                    lines++;
                }
            }
            fclose(txt_file);
        }
    }
    // If directory, traverse each element in the directory
    else if(S_ISDIR(mode)){
        pDir=opendir(pathName);
        while((pEnt = readdir(pDir)) != NULL) {
            if (strcmp(pEnt->d_name, ".") != 0 && strcmp( pEnt->d_name, "..") != 0){
                sprintf( fileName, "%s/%s", pathName, pEnt->d_name );
                printf("%s:%s\n", pathName, pEnt->d_name );
                traverse(fileName);
            }
        }
        closedir(pDir);
    }
    return lines;
}
int main(){
    // Read a pathname from user input
    char dirname[1000];
    printf("Enter a directory name: ");
    scanf("%s", dirname);
    int lines = traverse(dirname);
    // Print the directory if traverse is successful
    if(lines != -1){
        printf("Total Lines: %d", lines);
    }
}