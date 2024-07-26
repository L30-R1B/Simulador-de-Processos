#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include "../include/commander.h"
#include "../include/process_manager.h"
#include "../include/reporter.h"
#include "../include/processo_simulado.h"

void printa_info_processo(struct Processo *P){
    if(P == NULL)
        return;
    printf("\tid: ");
    if(!P->idProcesso)
        printf(" ZERO , ");
    else
        printf("%.6u, ", P->idProcesso);
    printf("idpai: ");
    if(P->idPai == UINT_MAX)
        printf("NENHUM, ");
    else
        printf("%.6u, ", P->idPai);
    printf("prioridade: %.8u, ", P->prioridade);
    printf("valor: ");
    if(!P->variavel)
        printf("0000000000, ");
    else
        printf("%.10d, ", P->variavel);
    printf("tempo inicio: ");
    if(!P->tempoInicio)
        printf("000000, ");
    else
        printf("%.6u, ", P->tempoInicio);
    printf("CPU usada até agora: ");
    if(!P->totalCpuUsada)
        printf("000000\n");
    else 
        printf("%.6u\n", P->totalCpuUsada);

}


void process_reporter(ProcessManager *Pm){
    printf("TOTAL DE PROCESSOS : %u\n", Pm->pcb->numProcessos);
    printf("*****************************************************************************************************************************\n");
    printf("ESTADO DO SISTEMA :\n");
    printf("*****************************************************************************************************************************\n");
    printf("TEMPO ATUAL : %u\n", Pm->tempo.t);
    printf("PROCESSO EXECUTANDO:\n");
    if(Pm->cpu->p != NULL)
        printa_info_processo(Pm->cpu->p);
    else
        printf("\t--- NULL ---\n");
    printf("::::\n");
    printf("FILA DE PROCESSOS BLOQUEADOS:\n");
    unsigned indice;
    if(!Pm->pB.numProcessos)
        printf("\t--- NULL ---\n");

    for(unsigned pBAtual = 0; pBAtual < Pm->pB.numProcessos; pBAtual ++){
        indice = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->pB.idProcessos[pBAtual]);
        if(indice ==  MAX_PROCESSOS)
            continue;
        printa_info_processo(Pm->pcb->P[indice]);
    }
    printf("::::\n");
    printf("PROCESSOS PRONTOS:\n");
    if(!Pm->pP.numProcessos){
        printf("\t--- NULL ---\n");
    }
    for(unsigned pPAtual = 0; pPAtual < Pm->pP.numProcessos; pPAtual ++){
        indice = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->pP.idProcessos[pPAtual]);
        if(indice ==  MAX_PROCESSOS)
            continue;
        printa_info_processo(Pm->pcb->P[indice]);
    }
    printf("*****************************************************************************************************************************\n");
}