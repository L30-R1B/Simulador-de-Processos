#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <limits.h>
#include "../include/commander.h"
#include "../include/process_manager.h"
#include "../include/reporter.h"
#include "../include/processo_simulado.h"

int main(){
    ProcessManager *Pm = cria_process_manager();

    if(Pm == NULL){
        printf("ARQUIVO DE TEXTO \"init\" NÃO ENCONTRADO NO MESMO DIRETÓRIO DO PROGRAMA ! ! !");
        return 1;
    }

    commander(Pm);
    
    destroi_process_manager(Pm);

    return 0;
}