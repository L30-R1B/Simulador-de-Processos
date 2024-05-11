#ifndef _processo_simulado_h_
#define _processo_simulado_h_

#define MAX_INSTRUCOES 100
#define MAX_PROCESSOS 50

typedef struct Instrucao{
    char instrucao;
    int valor;
    char nomeArq[50];
} Instrucoes;

typedef struct Processo{
    unsigned prioridade;

    Instrucoes programa[MAX_INSTRUCOES]; 
    
    unsigned idPai;
    unsigned idProcesso;
        
    int variavel;           
    
    unsigned ultimaInstruExec;
    unsigned posiUltimaInstru;

    unsigned tempoInicio;
    unsigned totalCpuUsada;
} Processos;

void inserir_instrucoes_arq(struct Processo *P, char *arqNome);
struct Processo *cria_processo(unsigned idProcesso, unsigned idPai);
void printa_processo(struct Processo P);
void insere_instru(struct Processo *P, struct Instrucao I);
void destroi_processo(struct Processo *P);
int executar_instrucao_processo(struct Processo **P, unsigned numInstru);
unsigned pesquisa_processo(Processos *P[], unsigned numProcessos, unsigned id);
void copia_processo(struct Processo **pCopia, struct Processo *pOriginal);
unsigned finaliza_processo(Processos *P[], unsigned numProcessos, unsigned id);

#endif