#ifndef _process_manager_h_
#define _process_manager_h_

#include "../include/processo_simulado.h"

typedef struct {
    struct Processo *p;
    unsigned indiceProcessoAtual;
}CPU;

typedef struct {
    Processos *P[MAX_PROCESSOS];
    unsigned numProcessos;
}TabelaPcb;

typedef struct Estados{
    unsigned idProcessos[MAX_PROCESSOS];
    unsigned numProcessos;
}ProcessosProntos, ProcessosBloqueados, ProcessosExecutando;

typedef struct {
    unsigned t;
}Tempo;

typedef struct {
    Tempo tempo;
    CPU *cpu;
    TabelaPcb *pcb;
    ProcessosProntos pP;
    ProcessosBloqueados pB;
    ProcessosExecutando pE;
}ProcessManager;

ProcessManager *cria_process_manager();
void destroi_process_manager(ProcessManager *Pm);
int inicia_processo(ProcessManager *Pm, char *arqNome, unsigned idProcesso, unsigned idPai);
unsigned troca_de_contexto(ProcessManager *Pm, unsigned idProcesso);
unsigned troca_de_imagem(ProcessManager *Pm, unsigned idProcesso);
void process_manager(ProcessManager *Pm, char comando);

#endif