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

    commander(Pm);
    
    destroi_process_manager(Pm);
}