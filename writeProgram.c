/**
 * C program to create a file and write data into file.
 */

#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE 1000

int main()
{
    /* Variable to store user content */
    char data[DATA_SIZE];

    /* File pointer to hold reference to our file */
    FILE * fPtr;


    /* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
while(1==1) {
fPtr = fopen("file1.txt", "a");
    sleep(1);
    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }



    /* Write data to file */
    fputs("teste\n", fPtr);


    /* Close file to save file data */
    fclose(fPtr);

}

    return 0;
}
