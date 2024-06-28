#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include "../include/commander.h"
#include "../include/process_manager.h"
#include "../include/reporter.h"
#include "../include/processo_simulado.h"

#define ESCRITA 1
#define LEITURA 0

int commander(ProcessManager *Pm) {
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe falhou !\n");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork falhou !\n");
        return 1;
    }

    if (pid > 0) {
        close(fd[LEITURA]);
        FILE *stream;
        stream = fdopen(fd[ESCRITA], "w"); 
        
        char comando;
        while (1) {
            scanf(" %c", &comando);
            fprintf(stream, "%c", comando);
            fflush(stream); 
            
            if (comando == 'T'){
                break;
            }
            sleep(1);
        }
        fclose(stream); 
        close(fd[ESCRITA]);
    } else {
        close(fd[ESCRITA]);

        char comando;
        while(read(fd[LEITURA], &comando, sizeof(char)) > 0)
            process_manager(Pm, comando);
        
        close(fd[LEITURA]);
    }

    return 0;
}