#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include "../include/commander.h"
#include "../include/process_manager.h"
#include "../include/reporter.h"
#include "../include/processo_simulado.h"

struct Processo *cria_processo(unsigned idProcesso, unsigned idPai){
    struct Processo *P;

    P = (struct Processo*) malloc(sizeof(struct Processo));
    P->prioridade = 0;
    P->idProcesso = idProcesso;
    P->idPai = idPai;
    P->ultimaInstruExec= 0;
    P->posiUltimaInstru = 0;
    P->totalCpuUsada = 0;
    P->tempoInicio = 0;
    P->variavel = 0;

    for(unsigned instrAtual = 0; instrAtual < MAX_INSTRUCOES; instrAtual ++){
        P->programa[instrAtual].instrucao = '\0';
        P->programa[instrAtual].nomeArq[0] = '\0';
        P->programa[instrAtual].valor = 99999999;
    }

    return P;
}
void printa_processo(struct Processo P){
    printf("____________________________\n");
    printf("Id pai : ");
    if(P.idPai == UINT_MAX)
        printf("NULO\n");
    else
        printf("%u\n", P.idPai);
    printf("Id processo : %u\n", P.idProcesso);
    printf("Variável : %d\n", P.variavel);
    if(P.ultimaInstruExec)
        printf("Instruções executadas :\n");
    for(unsigned instrAtual = 0; instrAtual < P.ultimaInstruExec; instrAtual ++){
        printf("\t%c", P.programa[instrAtual].instrucao);
        if(P.programa[instrAtual].valor != 99999999)
            printf(" %d", P.programa[instrAtual].valor);
        if(P.programa[instrAtual].nomeArq[0] != '\0')
            printf(" %s", P.programa[instrAtual].nomeArq);
        printf("\n");
    }
    if(P.posiUltimaInstru)
        printf("Instruções não executadas :\n");
    for(unsigned instrAtual = P.ultimaInstruExec; instrAtual < P.posiUltimaInstru; instrAtual ++){
        printf("\t%c", P.programa[instrAtual].instrucao);
        if(P.programa[instrAtual].valor != 99999999)
            printf(" %d", P.programa[instrAtual].valor);
        if(P.programa[instrAtual].nomeArq[0] != '\0')
            printf(" %s", P.programa[instrAtual].nomeArq);
        printf("\n");
    }
    printf("____________________________\n");
}
void insere_instru(struct Processo *P, struct Instrucao I){
    P->programa[P->posiUltimaInstru].instrucao = I.instrucao;
    strcpy(P->programa[P->posiUltimaInstru].nomeArq, I.nomeArq);
    P->programa[P->posiUltimaInstru].valor = I.valor;
    P->posiUltimaInstru ++;
}
void destroi_processo(struct Processo *P){
    if(P == NULL)
        return;
    free(P);
    P = NULL;
}
void inserir_instrucoes_arq(struct Processo *P, char *arqNome){
    FILE *f = fopen(arqNome, "r");
    if(f == NULL){
        printf("NULO\n\n\n\n");
        return;
    }
    char instru;
    while (fscanf(f, "%c", &instru) != EOF){
        if(instru == '\n')
            continue;
        P->programa[P->posiUltimaInstru].instrucao = instru;
        switch (instru){
            case 'S':
                fscanf(f, "%d", &P->programa[P->posiUltimaInstru].valor);
            break;
            case 'A':
                fscanf(f, "%d", &P->programa[P->posiUltimaInstru].valor);
            break;
            case 'D':
                fscanf(f, "%d", &P->programa[P->posiUltimaInstru].valor);
            break;
            case 'F':
                fscanf(f, "%d", &P->programa[P->posiUltimaInstru].valor);
            break;
            case 'R':
                fscanf(f, "%c", &instru);
                fgets(P->programa[P->posiUltimaInstru].nomeArq, 50, f);
                P->programa[P->posiUltimaInstru].nomeArq[strcspn(P->programa[P->posiUltimaInstru].nomeArq, "\n")] = 0;
            break;
            default:
            break;
        }
        P->posiUltimaInstru ++;
    }
    fclose(f);
}
int executar_instrucao_processo(struct Processo **P, unsigned numInstru){
    if(P == NULL)
        return 2;

    (*P)->totalCpuUsada ++;
    if(numInstru >= (*P)->posiUltimaInstru)
        (*P)->programa[numInstru].instrucao = 'E';

    (*P)->ultimaInstruExec = numInstru;
    switch ((*P)->programa[numInstru].instrucao){
        case 'S':
            (*P)->variavel = (*P)->programa[numInstru].valor;
        break;
        case 'A':
            (*P)->variavel += (*P)->programa[numInstru].valor;
        break;
        case 'D':
            (*P)->variavel -= (*P)->programa[numInstru].valor;
        break;
        case 'B':
            (*P)->ultimaInstruExec ++;
            return 2;
        break;
        case 'E':
            destroi_processo((*P));
            (*P) = NULL;
            return 3;
        break;
        case 'F':
            return 1;
        break;
        case 'R':
            unsigned prioridade = (*P)->prioridade;
            unsigned idP = (*P)->idProcesso, idPai = (*P)->idPai;
            char nomeArq[50];
            strcpy(nomeArq, (*P)->programa[numInstru].nomeArq);
            destroi_processo(*(P));
            (*P) = cria_processo(idP, idPai);
            (*P)->prioridade = prioridade; 
            inserir_instrucoes_arq(*(P), nomeArq);
            return 0;
        break;
    }
    (*P)->ultimaInstruExec ++;
    return UINT_MAX;
}
unsigned pesquisa_processo(Processos *P[], unsigned numProcessos, unsigned id){
    for(unsigned pAtual = 0; pAtual < numProcessos; pAtual ++){
        if(P[pAtual] != NULL)
            if(P[pAtual]->idProcesso == id)
                return pAtual;
    }
    return MAX_PROCESSOS;
}
void copia_processo(struct Processo **pCopia, struct Processo *pOriginal){
    if(pOriginal == NULL)
        return;
    if(*pCopia != NULL)
        destroi_processo(*pCopia); // Destruir o processo existente se houver
    *pCopia = cria_processo(pOriginal->idProcesso, pOriginal->idPai); // Criar um novo processo e atribuir ao ponteiro
    (*pCopia)->posiUltimaInstru = pOriginal->posiUltimaInstru;
    (*pCopia)->ultimaInstruExec = pOriginal->ultimaInstruExec;
    (*pCopia)->variavel = pOriginal->variavel;
    (*pCopia)->prioridade = pOriginal->prioridade;
    (*pCopia)->tempoInicio = pOriginal->tempoInicio;
    (*pCopia)->totalCpuUsada = pOriginal->totalCpuUsada;
    for(unsigned instrAtual = 0; instrAtual < pOriginal->posiUltimaInstru; instrAtual ++){
        (*pCopia)->programa[instrAtual].instrucao = pOriginal->programa[instrAtual].instrucao;
        strcpy((*pCopia)->programa[instrAtual].nomeArq, pOriginal->programa[instrAtual].nomeArq);
        (*pCopia)->programa[instrAtual].valor = pOriginal->programa[instrAtual].valor;
    }
}
unsigned finaliza_processo(Processos *P[], unsigned numProcessos, unsigned id){
    unsigned indice = pesquisa_processo(P, numProcessos, id);
    if(indice == MAX_PROCESSOS)
        return numProcessos;
    for(unsigned pAtual = indice; pAtual < numProcessos - 1; pAtual ++)
        copia_processo(&P[pAtual], P[pAtual + 1]);
    destroi_processo(P[numProcessos - 1]);
    return numProcessos - 1;
}

