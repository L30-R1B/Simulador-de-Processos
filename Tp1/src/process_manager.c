#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include "../include/commander.h"
#include "../include/process_manager.h"
#include "../include/reporter.h"
#include "../include/processo_simulado.h"
#include "../include/vetor.h"

ProcessManager *cria_process_manager(){
    ProcessManager *Pm = (ProcessManager*) malloc(sizeof(ProcessManager));
    Pm->pcb = (TabelaPcb*) malloc(sizeof(TabelaPcb));
    Pm->pcb->numProcessos = 0;
    
    Pm->cpu = (CPU*) malloc(sizeof(CPU));
    Pm->cpu->p = NULL;

    Pm->pP.numProcessos = 0;
    Pm->pE.numProcessos = 0;
    Pm->pB.numProcessos = 0;
    Pm->tempo.t = 0;

    inicia_processo(Pm, "init", 0, UINT_MAX);
    troca_de_imagem(Pm, 0);

    return Pm;
}
void destroi_process_manager(ProcessManager *Pm){
    if(Pm == NULL)
        return;
    for(unsigned pAtual = 0; pAtual < Pm->pcb->numProcessos; pAtual ++)
        destroi_processo(Pm->pcb->P[pAtual]);
    free(Pm->pcb);
    destroi_processo(Pm->cpu->p);
    free(Pm->cpu);
    free(Pm);
}
int inicia_processo(ProcessManager *Pm, char *arqNome, unsigned idProcesso, unsigned idPai){
    if(Pm->pcb->numProcessos == MAX_PROCESSOS)
        return 1;
    Pm->pcb->P[Pm->pcb->numProcessos] = cria_processo(idProcesso, idPai);
    inserir_instrucoes_arq(Pm->pcb->P[Pm->pcb->numProcessos], arqNome);
    if(!Pm->pcb->P[Pm->pcb->numProcessos]->posiUltimaInstru)
        return 1;
    Pm->pcb->numProcessos ++;
    return 0;
}
unsigned troca_de_contexto(ProcessManager *Pm, unsigned idProcesso){
    unsigned indice = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, idProcesso);
    unsigned indiceProcessCpuNaPcb;
    
    if(indice == MAX_PROCESSOS)
        return 1;
    if(Pm->cpu->p == NULL){
        copia_processo(&Pm->cpu->p, Pm->pcb->P[indice]);
    }else{
        indiceProcessCpuNaPcb = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->cpu->p->idProcesso);
        copia_processo(&Pm->pcb->P[indiceProcessCpuNaPcb], Pm->cpu->p);
        copia_processo(&Pm->cpu->p, Pm->pcb->P[indice]);
    }
    Pm->pE.numProcessos = remove_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->indiceProcessoAtual);
    if(pesquisa_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->cpu->indiceProcessoAtual) == MAX_PROCESSOS)
        Pm->pP.numProcessos = insere_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->indiceProcessoAtual);

    Pm->cpu->indiceProcessoAtual = Pm->pcb->P[indice]->idProcesso;
    Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->indiceProcessoAtual);
    Pm->pE.numProcessos = insere_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->indiceProcessoAtual);
    return 0;
}
unsigned troca_de_imagem(ProcessManager *Pm, unsigned idProcesso){
    unsigned indice = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, idProcesso);
    if(indice == MAX_PROCESSOS)
        return 1;
    copia_processo(&Pm->cpu->p, Pm->pcb->P[indice]);
    Pm->pE.numProcessos = remove_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->indiceProcessoAtual);
    Pm->cpu->indiceProcessoAtual = Pm->pcb->P[indice]->idProcesso;
    Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->indiceProcessoAtual);
    Pm->pE.numProcessos = insere_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->indiceProcessoAtual);
    return 0;
}
unsigned cria_processo_filho(ProcessManager *Pm, struct Processo *pPai){
    if(pPai == NULL)
        return MAX_PROCESSOS;
    else if(Pm->pcb->numProcessos == MAX_PROCESSOS)
        return MAX_PROCESSOS;
    unsigned id = rand() % 1000000;
    while(pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, id) != MAX_PROCESSOS)
        id = rand() % 1000000;
    Pm->pcb->P[Pm->pcb->numProcessos] = NULL;
    copia_processo(&Pm->pcb->P[Pm->pcb->numProcessos], pPai);
    Pm->pcb->P[Pm->pcb->numProcessos]->idProcesso = id;
    Pm->pcb->P[Pm->pcb->numProcessos]->idPai = pPai->idProcesso;
    Pm->pcb->P[Pm->pcb->numProcessos]->prioridade += pPai->prioridade + (Pm->pcb->numProcessos + 1);
    Pm->pcb->numProcessos++; 

    return Pm->pcb->numProcessos - 1;
}
void escalonamento(ProcessManager *Pm){
    
    unsigned prio = 0;
    unsigned indiceProxProcesso = MAX_PROCESSOS;

    if(Pm->cpu->p == NULL){
        unsigned id = Pm->cpu->indiceProcessoAtual;
        Pm->pcb->numProcessos = finaliza_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->cpu->indiceProcessoAtual);
        for(unsigned pAtual = 0; pAtual < Pm->pcb->numProcessos; pAtual ++){
            if(Pm->pcb->P[pAtual]->prioridade >= prio && 
                pesquisa_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->pcb->P[pAtual]->idProcesso) == MAX_PROCESSOS){
                    prio = Pm->pcb->P[pAtual]->prioridade;
                    indiceProxProcesso = pAtual;
            }
        }
        if(indiceProxProcesso != MAX_PROCESSOS){
            troca_de_imagem(Pm, Pm->pcb->P[indiceProxProcesso]->idProcesso);
            Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, id);
        }
        return;
    }


    for(unsigned pAtual = 0; pAtual < Pm->pcb->numProcessos; pAtual ++){
        if(Pm->pcb->P[pAtual]->idProcesso == Pm->cpu->p->idProcesso)
            continue;
        if(Pm->pcb->P[pAtual]->prioridade >= prio
            && pesquisa_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->pcb->P[pAtual]->idProcesso) == MAX_PROCESSOS){
                prio = Pm->pcb->P[pAtual]->prioridade;
                indiceProxProcesso = pAtual;
        }
    }
    if(indiceProxProcesso == MAX_PROCESSOS){
        return;
    }
    troca_de_contexto(Pm, Pm->pcb->P[indiceProxProcesso]->idProcesso);
    Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->indiceProcessoAtual);
}
void process_manager(ProcessManager *Pm, char comando){
    switch (comando){
        case 'Q':
            if(Pm->cpu->p == NULL){
                if(!Pm->pcb->numProcessos)
                    return;
                escalonamento(Pm);
                break;
            }
            unsigned retornoInstru = executar_instrucao_processo(&Pm->cpu->p, Pm->cpu->p->ultimaInstruExec);
            if(retornoInstru == 1){
                unsigned indiceFilho = cria_processo_filho(Pm, Pm->cpu->p);
                Pm->pcb->P[indiceFilho]->ultimaInstruExec = Pm->cpu->p->ultimaInstruExec + 1;
                Pm->pcb->P[indiceFilho]->tempoInicio = Pm->tempo.t;
                Pm->pcb->P[indiceFilho]->totalCpuUsada = 0;
                Pm->cpu->p->ultimaInstruExec += Pm->cpu->p->programa[Pm->cpu->p->ultimaInstruExec].valor;
                Pm->pP.numProcessos = insere_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->pcb->P[indiceFilho]->idProcesso);
            }else if(retornoInstru == 2){
                Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->indiceProcessoAtual);
                Pm->pB.numProcessos = insere_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->cpu->indiceProcessoAtual);                
            }else if(!retornoInstru)
                Pm->cpu->p->tempoInicio = Pm->tempo.t;
            
            Pm->tempo.t ++;
            escalonamento(Pm);
        break;
        case 'U':
            if(Pm->pB.numProcessos){
                Pm->pP.numProcessos = insere_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->pB.idProcessos[0]);
                Pm->pB.numProcessos = remove_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->pB.idProcessos[0]);
            }
            escalonamento(Pm);
        break;
        case 'P':
            printf("--------\n\n\n\n");
            if(Pm->cpu->p != NULL)
                printa_processo(*Pm->cpu->p);
            process_reporter(Pm);
            printf("--------\n\n\n\n");
        break;
        case 'T':
            printf("--------\n\n\n\n");
            if(Pm->cpu->p != NULL)
                printa_processo(*Pm->cpu->p);
            process_reporter(Pm);
            printf("--------\n\n\n\n");
        break;
    }
}