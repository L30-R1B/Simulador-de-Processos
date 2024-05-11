#include <stdio.h>
#include "../include/vetor.h"

#define MAX_PROCESSOS 50

unsigned pesquisa_num_vet(unsigned *vetor, unsigned tamanho, unsigned num){
    for(unsigned i = 0; i < tamanho; i ++)
        if(vetor[i] == num)
            return i;
    return MAX_PROCESSOS;
}
unsigned insere_num_vet(unsigned *vetor, unsigned tamanho, unsigned num){
    if(pesquisa_num_vet(vetor, tamanho, num) != MAX_PROCESSOS)
        return tamanho;
    vetor[tamanho] = num;
    return tamanho + 1;
}
unsigned remove_num_vet(unsigned *vetor, unsigned tamanho, unsigned num){
    unsigned indice = pesquisa_num_vet(vetor, tamanho, num);
    if(indice == MAX_PROCESSOS)
        return tamanho;
    for(unsigned i = indice; i < tamanho - 1; i ++)
        vetor[i] = vetor[i + 1];
    return tamanho - 1;
}
void printa_vetor(unsigned *vetor, unsigned tamanho){
    printf("[");
    for(unsigned i = 0; i < tamanho; i ++){
        printf(" %u", vetor[i]);
        if(i < tamanho - 1)
            printf(",");
    }
    printf(" ]\n");
}