#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <signal.h>

#define MYSHELL_VERSION "v0.0"
#define CYAN_COLOR_TERM "\x1B[36;1m"
#define YELL_COLOR_TERM "\x1B[33m"
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
static void stop_proc();
static void kill_proc();
static void continue_proc();

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
                //printf("TODO: Implementar comando %s.\n", command);
                stop_proc(args);
            }
            else if(strcmp(command, "continue") == 0) {
                //printf("TODO: Implementar comando %s.\n", command);
                continue_proc(args);
            }
            else if(strcmp(command, "kill") == 0) {
                //printf("TODO: Implementar comando %s.\n", command);
                kill_proc(args);
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
    printf("%s", YELL_COLOR_TERM);
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


void print_status(int wstatus, int cpid) {
    if (WIFEXITED(wstatus)) {  
            print_myshell_return();
            switch (wstatus) {
                case 0: printf("processo %d foi finalizado com status 0. Executado com sucesso.\n",cpid); break;
                case 1: printf("processo %d foi finalizado com status 1. Finalizado através de hangup.\n",cpid); break;
            }
        }   
        else if (WIFSIGNALED(wstatus)) {
            switch(wstatus) {
                case 9: printf("processo %d finalizou de forma anormal com sinal 9: foi executado um kill no processo.\n", cpid); break;
                case 136: printf("processo %d finalizou de forma anormal com sinal 8: ocorreu um problema de ponto flutuante ou divisão por zero.\n", cpid); break; //128 + 8
                case 132: printf("processo %d finalizou de forma anormal com sinal 4: o programa executou uma instrução ilegal.\n", cpid); break; //128 + 4
                case 139: printf("processo %d finalizou de forma anormal com sinal 11: segmentation fault, o programa tentou acessar uma referência de memória inválida.\n", cpid); break; //128 + 11
                case 134: printf("processo %d finalizou de forma anormal com sinal 6: o programa foi abortado.\n", cpid); break; //128 + 6
            }
            
        } else if (WIFSTOPPED(wstatus)) {
            printf("processo %d foi parado pelo sinal %d\n", cpid, WSTOPSIG(wstatus));
        }
}

void wait_proc(){
    int wstatus;
    int cpid = wait(&wstatus);
    if(cpid > 0){
        print_status(wstatus, cpid);
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
        print_status(wstatus, cpid);
    }                 
}

void stop_proc(char **argv){
    pid_t stop_pid = atoi(argv[0]);
    if(kill(stop_pid, SIGSTOP) == -1){
        print_myshell_return();
        printf("Parada de processo falhou\n");
    }
}

void kill_proc(char **argv){
    pid_t stop_pid = atoi(argv[0]);
    if(kill(stop_pid, SIGKILL) == -1){
        print_myshell_return();
        printf("Kill de processo falhou\n");
    }
}

void continue_proc(char **argv){
    pid_t stop_pid = atoi(argv[0]);
    if(kill(stop_pid, SIGCONT) == -1){
        print_myshell_return();
        printf("Continue de processo falhou\n");
    }
}