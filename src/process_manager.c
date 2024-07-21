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
    Pm->tRM.tempoRetornoTotal.t = 0;
    Pm->tRM.numProcessosFinalizados = 0;

    int resul = inicia_processo(Pm, "init", 0, UINT_MAX);
    
    if(resul)
        return NULL;

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

    indiceProcessCpuNaPcb = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->cpu->idProcessoAtual);
    copia_processo(&Pm->pcb->P[indiceProcessCpuNaPcb], Pm->cpu->p);
    copia_processo(&Pm->cpu->p, Pm->pcb->P[indice]);

    Pm->pE.numProcessos = remove_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->idProcessoAtual);
    Pm->pP.numProcessos = insere_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->idProcessoAtual);

    Pm->cpu->idProcessoAtual = Pm->pcb->P[indice]->idProcesso;

    Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->idProcessoAtual);
    Pm->pE.numProcessos = insere_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->idProcessoAtual);

    return 0;
}
unsigned troca_de_imagem(ProcessManager *Pm, unsigned idProcesso){
    unsigned indice = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, idProcesso);
    if(indice == MAX_PROCESSOS)
        return 1;

    copia_processo(&Pm->cpu->p, Pm->pcb->P[indice]);

    Pm->pE.numProcessos = remove_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->idProcessoAtual);

    Pm->cpu->idProcessoAtual = Pm->pcb->P[indice]->idProcesso;

    Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->cpu->idProcessoAtual);
    Pm->pE.numProcessos = insere_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->idProcessoAtual);

    return 0;
}
unsigned cria_processo_filho(ProcessManager *Pm, struct Processo *pPai){
    if(pPai == NULL)
        return MAX_PROCESSOS;

    srand(time(NULL));
    
    unsigned id = rand() % 1000000;
    while(pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, id) != MAX_PROCESSOS)
        id = rand() % 1000000;

    Pm->pcb->P[Pm->pcb->numProcessos] = NULL;
    copia_processo(&Pm->pcb->P[Pm->pcb->numProcessos], pPai);

    Pm->pcb->P[Pm->pcb->numProcessos]->idProcesso = id;
    Pm->pcb->P[Pm->pcb->numProcessos]->idPai = pPai->idProcesso;
    Pm->pcb->P[Pm->pcb->numProcessos]->prioridade = Pm->pcb->P[Pm->pcb->numProcessos - 1]->prioridade + 1;
    Pm->pcb->numProcessos++; 

    return Pm->pcb->numProcessos - 1;
}

unsigned encontra_index_maior_prioridade(ProcessManager *Pm){
    unsigned prio = 0;
    unsigned indiceProxProcesso = MAX_PROCESSOS;

    for(unsigned pAtual = 0, indiceAtual = 0; pAtual < Pm->pP.numProcessos; pAtual ++){
        indiceAtual = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->pP.idProcessos[pAtual]);

        if(Pm->pcb->P[indiceAtual]->prioridade >= prio){
            prio = Pm->pcb->P[indiceAtual]->prioridade;
            indiceProxProcesso = indiceAtual;
        }
    }

    return indiceProxProcesso;
}

void escalonamento(ProcessManager *Pm){

    unsigned indiceProxProcesso = encontra_index_maior_prioridade(Pm);

    if(indiceProxProcesso == MAX_PROCESSOS){
        return;
    }

    if(Pm->cpu->p == NULL){   
        if(!Pm->pcb->numProcessos)
            return;
        if(Pm->pcb->P[indiceProxProcesso]->ultimaInstruExec >= Pm->pcb->P[indiceProxProcesso]->posiUltimaInstru)
            Pm->pcb->numProcessos = finaliza_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->cpu->idProcessoAtual);
        troca_de_imagem(Pm, Pm->pcb->P[indiceProxProcesso]->idProcesso);
    }else
        troca_de_contexto(Pm, Pm->pcb->P[indiceProxProcesso]->idProcesso);
}

void bloqueia_processo_cpu(ProcessManager *Pm){
    Pm->pE.numProcessos = remove_num_vet(Pm->pE.idProcessos, Pm->pE.numProcessos, Pm->cpu->idProcessoAtual);
    Pm->pB.numProcessos = insere_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->cpu->idProcessoAtual); 

    unsigned indicePcbProcesso = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->cpu->idProcessoAtual);

    copia_processo(&Pm->pcb->P[indicePcbProcesso], Pm->cpu->p);
    destroi_processo(Pm->cpu->p);
    Pm->cpu->p = NULL;
}

void process_manager(ProcessManager *Pm, char comando){
    printf("--- COMANDO %c ---\n", comando);
    switch (comando){
        case 'Q':
            if(Pm->cpu->p == NULL){
                if(!Pm->pP.numProcessos)
                    return;
                escalonamento(Pm);
            }
            unsigned retornoInstru = executar_instrucao_processo(&Pm->cpu->p, Pm->cpu->p->ultimaInstruExec);
            if(retornoInstru == 1){

                if(Pm->pcb->numProcessos == MAX_PROCESSOS){
                    return;
                }

                unsigned indiceFilho = cria_processo_filho(Pm, Pm->cpu->p);
                Pm->pcb->P[indiceFilho]->ultimaInstruExec = Pm->cpu->p->ultimaInstruExec + 1;
                Pm->pcb->P[indiceFilho]->tempoInicio = Pm->tempo.t;
                Pm->pcb->P[indiceFilho]->totalCpuUsada = 0;
                Pm->cpu->p->ultimaInstruExec += Pm->cpu->p->programa[Pm->cpu->p->ultimaInstruExec].valor;

                Pm->pP.numProcessos = insere_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->pcb->P[indiceFilho]->idProcesso);

                troca_de_contexto(Pm, Pm->pcb->P[indiceFilho]->idProcesso);

                Pm->pP.numProcessos = remove_num_vet(Pm->pP.idProcessos, Pm->pP.numProcessos, Pm->pcb->P[indiceFilho]->idPai);
                Pm->pB.numProcessos = insere_num_vet(Pm->pB.idProcessos, Pm->pB.numProcessos, Pm->pcb->P[indiceFilho]->idPai); 
                
                Pm->tempo.t ++;

                break;
            }else if(retornoInstru == 2){
                bloqueia_processo_cpu(Pm);
            }else if(retornoInstru == 3){
                unsigned ind = pesquisa_processo(Pm->pcb->P, Pm->pcb->numProcessos, Pm->cpu->idProcessoAtual);
                Pm->tRM.numProcessosFinalizados ++;
                Pm->tRM.tempoRetornoTotal.t += Pm->tempo.t - Pm->pcb->P[ind]->tempoInicio;
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
        break;
        case 'P':
            printf("--------\n\n\n\n");
            if(Pm->cpu->p != NULL)
                printa_processo(*Pm->cpu->p);
            process_reporter(Pm);
            printf("--------\n\n\n\n");

            return;
        break;
        case 'T':
            !Pm->tRM.numProcessosFinalizados? printf("--- Não foi possivel exibir o tempo de retorno médio pelo motivo de nenhum processo ter chegado ao fim ---\n") : 
                                              printf("--- Tempo de retorno médio : %.2lf ---\n--- Total Processos Terminados : %u ---\n", ((double) Pm->tRM.tempoRetornoTotal.t / (double) Pm->tRM.numProcessosFinalizados), Pm->tRM.numProcessosFinalizados);
        break;
    }
}