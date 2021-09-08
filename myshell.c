#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>

#define MYSHELL_VERSION "v0.0"
#define MAGENTA_COLOR_TERM "\x1B[35m"
#define DEFAULT_COLOR_TERM "\x1B[0m"
#define COMMAND_MAX_CHAR 4096
#define COMMAND_MAX_WORD 100

static void print_myshell();
static void print_header();
static int read_command(char** command, char*** args, int* arg_number);

int main() {    
    print_header();
    print_myshell();

    char* command = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
    char** args = (char**) malloc(COMMAND_MAX_WORD * sizeof(char*));
    int arg_number = 0;

    int ret = read_command(&command, &args, &arg_number);
    if(ret >= 0) {
        printf ("command:%s arg_number:%d\n", command, arg_number);
        for(int i = 0; i < arg_number; i++) 
            printf("arg%d = %s\n", i, args[i]);
    } 
    else {
        printf("-1\n");
    }

    return 0;
}

void print_myshell() {
    printf("%s", MAGENTA_COLOR_TERM);
    printf("myshell");
    printf("%s", DEFAULT_COLOR_TERM);
    printf("> ");
}

void print_header() {
    printf("MyShell %s\n", MYSHELL_VERSION);
    printf("Authors: André Paiva, Ariel Andrade, Natan Moura, Pedro Correia\n\n");    
}

int read_command(char** command, char*** args, int* arg_number) {
    char* str = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
    char* arg = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
    int word_counter = 0;

    fgets(str, COMMAND_MAX_CHAR, stdin);    
       
    arg = strtok(str, " \t\n");  
    if(arg == NULL){        
        fflush(stdout);
        free(str);
        free(arg);
        return -1;
    }
    else {
        do {
            strcpy(*command, arg);
            arg = strtok(NULL, " \t\n");
            if(arg != NULL) {
                (*args)[word_counter] = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
                strcpy((*args)[word_counter], arg);
                word_counter++;
                (*args)[word_counter] = NULL;
            }
        } while (arg != NULL);
    }
    *arg_number = word_counter;
    fflush(stdout);
    free(str);
    free(arg);
    return 0;   
}

