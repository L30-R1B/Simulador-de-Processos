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

    if (pid > 0) { // Pai
        close(fd[LEITURA]); // Fecha a extremidade de leitura do pipe
        FILE *stream;
        stream = fdopen(fd[ESCRITA], "w"); // Abre um stream de escrita no pipe
        
        char comando;
        while (1) {
            // Lê um comando da entrada padrão e o escreve no pipe
            scanf(" %c", &comando);
            fprintf(stream, "%c", comando);
            fflush(stream); 
            
            if (comando == 'T'){
                break;
            }
            sleep(1);
        }
        fclose(stream); // Fecha o stream
        close(fd[ESCRITA]); // Fecha o pipe
    } else { // Filho
        close(fd[ESCRITA]); // Fecha a extremidade de escrita do pipe

        char comando;
        while(read(fd[LEITURA], &comando, sizeof(char)) > 0)
            process_manager(Pm, comando);
        
        close(fd[LEITURA]); // Fecha o pipe
    }

    return 0;
}