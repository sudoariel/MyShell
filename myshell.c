#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdint.h>

#define MYSHELL_VERSION "v0.0"
#define CYAN_COLOR_TERM "\x1B[36;1m"
#define DEFAULT_COLOR_TERM "\x1B[0m"
#define COMMAND_MAX_CHAR 4096
#define COMMAND_MAX_WORD 100

static void print_myshell();
static void print_myshell_return();
static void print_header();
static int read_command(char** command, char*** args, int* arg_number);
static void run(char** args);
static void start(char** args);
static void wait_proc();

int main() {    
    print_header();

    do {
        // Imprime o myshell> com a cor em destaque
        print_myshell();  

        // Alocação dinâmica das variáveis para armazenar comando e argumentos
        char* command = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
        char** args = (char**) malloc(COMMAND_MAX_WORD * sizeof(char*));
        int arg_number = 0;

        // Realiza a leitura da linha
        int ret = read_command(&command, &args, &arg_number);
        if(ret == 0) {
            // FOR DEBUGGING
            /*
            printf ("command: %s arg_number: %d\n----args----\n", command, arg_number);
            for(int i = 0; i < arg_number; i++) 
                printf("arg%d = %s\n", i, args[i]);
            */

            // Verifica comando e executa sua função específica
            if(strcmp(command, "wait") == 0) {
                //printf("TODO: Implementar comando %s.\n", command);
                wait_proc();
            }
            else if(strcmp(command, "start") == 0) {
                //printf("TODO: Implementar comando %s.\n", command);
                start(args);
            }
            else if(strcmp(command, "run") == 0) {
                //printf("TODO: Implementar comando %s.\n", command);
                run(args);
            }
            else if(strcmp(command, "stop") == 0) {
                printf("TODO: Implementar comando %s.\n", command);
            }
            else if(strcmp(command, "continue") == 0) {
                printf("TODO: Implementar comando %s.\n", command);
            }
            else if(strcmp(command, "kill") == 0) {
                printf("TODO: Implementar comando %s.\n", command);
            }
            else if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
                for(int i = 0; i < arg_number; i++) 
                    free(args[i]);
                free(args);
                return 0;
            }
            else {
                printf("Comando desconhecido: %s\n", command);
            }         
        } 
        else if(ret == -1){
            continue;
        }
        else {
            // EOF ret == 1
            for(int i = 0; i < arg_number; i++) 
                free(args[i]);
            free(args);
            return 0;
        }

        // Liberando memória alocada das variáveis utilizadas nessa iteração.
        for(int i = 0; i < arg_number; i++) 
            free(args[i]);
        free(command);
    } while (true);    
    return 0;
}

void print_myshell() {
    printf("%s", CYAN_COLOR_TERM);
    printf("myshell");
    printf("%s", DEFAULT_COLOR_TERM);
    printf("> ");
}

void print_myshell_return(){
    printf("%s", CYAN_COLOR_TERM);
    printf("myshell: ");   
    printf("%s", DEFAULT_COLOR_TERM);
}

void print_header() {
    printf("MyShell %s\n", MYSHELL_VERSION);
    printf("Authors: André Paiva, Ariel Andrade, Natan Moura, Pedro Correia\n\n");    
}

int read_command(char** command, char*** args, int* arg_number) {
    char* str = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
    char* arg = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
    char* cmd = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
    int word_counter = 0;

    if(fgets(str, COMMAND_MAX_CHAR, stdin) == NULL)
        return 1;    
    fflush(stdout);

    cmd = strtok(str, " \t\n");  
    if(cmd == NULL){        
        free(str);
        free(arg);
        return -1;
    }
    else {
        do {            
            arg = strtok(NULL, " \t\n");
            if(arg != NULL) {
                (*args)[word_counter] = (char*) malloc(COMMAND_MAX_CHAR * sizeof(char));
                strcpy((*args)[word_counter], arg);
                word_counter++;
                (*args)[word_counter] = NULL;
            }
        } while (arg != NULL);
    }
    strcpy(*command, cmd);
    *arg_number = word_counter;
    free(str);
    free(arg);
    return 0;   
}

void wait_proc(){
    int wstatus;
    int cpid = wait(&wstatus);
    if(cpid > 0){
        if (WIFEXITED(wstatus)) {  
            print_myshell_return();
            printf("processo %d foi finalizado com status %d.\n",cpid,WEXITSTATUS(wstatus));
        }   
        //To do: Tratar mais erros
        else if (WIFSIGNALED(wstatus)) {
            printf("processo %d finalizou de forma anormal com sinal %d, killed.\n", cpid, WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
            printf("processo %d foi parado pelo sinal %d\n", cpid, WSTOPSIG(wstatus));
        }
    }
    else{
        print_myshell_return();
        printf("não há processos restantes.\n");
    }
}

void start(char **argv){
    pid_t cpid;
    cpid = fork();
    if (cpid == -1) {
        exit(EXIT_FAILURE);
    }
    else if (cpid == 0) {  
        int s = execvp(argv[0],argv);
        if(s < 0) exit(EXIT_FAILURE);
        
    }
    else{        
        print_myshell_return();
        printf("processo %d foi iniciado.\n",cpid); 
    }
}

void run(char **argv){ 
    pid_t cpid;
    int wstatus;
    cpid = fork();
    if (cpid == -1) {
        exit(EXIT_FAILURE);
    }
    else if (cpid == 0) {   
       int s = execvp(argv[0],argv);
       perror(argv[0]);
       if(s < 0) exit(EXIT_FAILURE);
       else exit(EXIT_SUCCESS);
    }
    else {     
        waitpid(cpid, &wstatus, WUNTRACED);
        if (WIFEXITED(wstatus)) {  
            print_myshell_return();
            printf("processo %d foi finalizado com status %d.\n",cpid,WEXITSTATUS(wstatus));
        }   
        //To do: Tratar mais erros
        else if (WIFSIGNALED(wstatus)) {
            printf("processo %d finalizou de forma anormal com sinal %d, killed.\n", cpid, WTERMSIG(wstatus));
        } else if (WIFSTOPPED(wstatus)) {
            printf("processo %d foi parado pelo sinal %d\n", cpid, WSTOPSIG(wstatus));
        }
    }                 
}
