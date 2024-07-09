#ifndef _process_manager_h_
#define _process_manager_h_

#include "../include/processo_simulado.h"

/**
 * @brief Estrutura que representa a CPU e o processo atualmente em execução.
 *
 * @param p Um ponteiro para a estrutura Processo atualmente em execução na CPU.
 * @param idProcessoAtual O íd do processo atual.
 */
typedef struct {
    struct Processo *p;
    unsigned idProcessoAtual;
}CPU;
/**
 * @brief Estrutura que representa a tabela de processos (PCB - Process Control Block).
 *
 * @param P Um array de ponteiros para estruturas Processos, representando os processos.
 * @param numProcessos O número total de processos atualmente na tabela.
 */
typedef struct {
    Processos *P[MAX_PROCESSOS];
    unsigned numProcessos;
}TabelaPcb;
/**
 * @brief Estrutura que representa o estado dos processos no sistema.
 *
 * @param idProcessos Um array de IDs de processos.
 * @param numProcessos O número total de processos nesse estado.
 */
typedef struct Estados{
    unsigned idProcessos[MAX_PROCESSOS];
    unsigned numProcessos;
}ProcessosProntos, ProcessosBloqueados, ProcessosExecutando;

/**
 * @brief Estrutura que representa o tempo no sistema.
 *
 * @param t O valor do tempo.
 */
typedef struct {
    unsigned t;
}Tempo;

/**
 * @brief Estrutura que representa o tempo de retorno medio do sistema.
 *
 * @param tempoRetornoTota O valor do tempo de retorno total.
 * @param numProcessosFinalizados Quantidade de processos finalizados até o momento.
 */
typedef struct {
    Tempo tempoRetornoTotal;
    unsigned numProcessosFinalizados;
}TempoRetornoMedio;

/**
 * @brief Estrutura que gerencia os processos no sistema.
 *
 * A estrutura `ProcessManager` centraliza o gerenciamento de processos no sistema,
 * incluindo o tempo do sistema, a CPU, a tabela de processos (PCB), os estados
 * dos processos (prontos, bloqueados e executando) e o tempo de retorno médio.
 *
 * @param tempo A estrutura Tempo que representa o tempo no sistema.
 * @param cpu Um ponteiro para a estrutura CPU que representa a CPU e o processo atualmente em execução.
 * @param pcb Um ponteiro para a estrutura TabelaPcb que representa a tabela de processos.
 * @param pP A estrutura ProcessosProntos que contém os IDs dos processos prontos para execução.
 * @param pB A estrutura ProcessosBloqueados que contém os IDs dos processos bloqueados.
 * @param pE A estrutura ProcessosExecutando que contém os IDs dos processos atualmente em execução.
 * @param tRM Estrutura TempoRetornoMedio que armazena o tempo de retorno médio.
 */
typedef struct {
    Tempo tempo;
    CPU *cpu;
    TabelaPcb *pcb;
    ProcessosProntos pP;
    ProcessosBloqueados pB;
    ProcessosExecutando pE;
    TempoRetornoMedio tRM;
}ProcessManager;

/**
 * @brief Cria e inicializa um novo gerenciador de processos.
 *
 * Esta função aloca memória e inicializa um novo gerenciador de processos (`ProcessManager`).
 * A estrutura gerenciada inclui a configuração do tempo, da CPU, da tabela de processos
 * e dos estados dos processos (prontos, bloqueados e executando). O gerenciador de processos
 * criado é retornado como um ponteiro.
 *
 * @return Um ponteiro para a nova estrutura `ProcessManager` inicializada.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      // Usa o gerenciador de processos...
 *      // Libera a memória do gerenciador de processos quando não for mais necessário...
 */
ProcessManager *cria_process_manager();

/**
 * @brief Destroi e libera a memória de um gerenciador de processos.
 *
 * Esta função libera todos os recursos alocados dinamicamente pelo gerenciador de processos (`ProcessManager`),
 * incluindo a CPU, a tabela de processos e quaisquer outras estruturas associadas. Após a chamada desta função,
 * o ponteiro para o gerenciador de processos fornecido não é mais válido.
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que será destruída e liberada da memória.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      // Usa o gerenciador de processos...
 *      destroi_process_manager(pm);
 */
void destroi_process_manager(ProcessManager *Pm);
/**
 * @brief Cria um processo com o conteúdo de um arquivo como script.
 *
 * Esta função cria um novo processo no gerenciador de processos (`ProcessManager`) e inicializa
 * seu script com o conteúdo de um arquivo especificado por `arqNome`. O processo é identificado
 * pelos IDs `idProcesso` e `idPai`. O novo processo é adicionado à tabela de processos (PCB) e
 * aos estados apropriados no gerenciador de processos.
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que gerencia os processos.
 * @param arqNome O nome do arquivo que contém o script para o novo processo.
 * @param idProcesso O ID do novo processo.
 * @param idPai O ID do processo pai.
 * @return Um valor inteiro indicando o sucesso ou falha na criação do processo.
 *         Retorna 0 em caso de sucesso, ou um valor negativo em caso de falha.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      int resultado = inicia_processo(pm, "init", 1, 0);
 *      if (resultado == 0) {
 *          // Processo iniciado com sucesso
 *      } else {
 *          // Falha na criação do processo
 *      }
 */
int inicia_processo(ProcessManager *Pm, char *arqNome, unsigned idProcesso, unsigned idPai);
/**
 * @brief Realiza a troca de contexto para um novo processo no gerenciador de processos.
 *
 * Esta função realiza a troca de contexto no gerenciador de processos (`ProcessManager`)
 * para o processo identificado por `idProcesso`. A troca de contexto envolve salvar o
 * estado do processo atualmente em execução e carregar o estado do novo processo.
 * A função retorna 0 caso a troca seja feita e 1 caso contrário.
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que gerencia os processos.
 * @param idProcesso O ID do processo para o qual será realizada a troca de contexto.
 * @return 0 caso seja feita a troca e 1 em caso de erro.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      // Inicializa e adiciona processos ao gerenciador...
 *      unsigned resul = troca_de_contexto(pm, 2);
 *      if (!resul) {
 *          // Troca de contexto bem-sucedida
 *      } else {
 *          // Processo com o ID especificado não foi encontrado
 *      }
 */
unsigned troca_de_contexto(ProcessManager *Pm, unsigned idProcesso);
/**
 * @brief Troca a imagem de execução para um novo processo no gerenciador de processos.
 *
 * Esta função realiza a troca da imagem de execução (código e dados) no gerenciador de processos (`ProcessManager`)
 * para o processo identificado por `idProcesso`. A troca de imagem envolve carregar o contexto do novo processo
 * e atualizar o estado da CPU para refletir a nova imagem de execução. A função retorna 0 em caso de sucesso
 * e 1 em caso de falha (por exemplo, se o processo com o ID especificado não for encontrado).
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que gerencia os processos.
 * @param idProcesso O ID do processo para o qual será realizada a troca de imagem.
 * @return 0 se a troca de imagem for bem-sucedida, ou 1 se ocorrer uma falha.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      // Inicializa e adiciona processos ao gerenciador...
 *      unsigned resultado = troca_de_imagem(pm, 2);
 *      if (!resultado) {
 *          // Troca de imagem bem-sucedida
 *      } else {
 *          // Falha na troca de imagem
 *      }
 */
unsigned troca_de_imagem(ProcessManager *Pm, unsigned idProcesso);
/**
 * @brief Realiza o escalonamento dos processos no gerenciador de processos.
 *
 * Esta função implementa a política de escalonamento de processos no gerenciador de processos (`ProcessManager`).
 * O processo com a maior prioridade é selecionado para execução. A função identifica o processo com a maior
 * prioridade dentre os prontos para execução e realiza a troca de contexto para esse processo.
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que gerencia os processos.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      // Inicializa e adiciona processos ao gerenciador...
 *      escalonamento(pm);
 *      // O processo com a maior prioridade é selecionado para execução
 */
void escalonamento(ProcessManager *Pm);
/**
 * @brief Gerencia o sistema de processos com base nos comandos recebidos.
 *
 * Esta função executa diferentes ações no gerenciador de processos (`ProcessManager`) com base no comando fornecido.
 * Os comandos disponíveis são:
 * - 'Q': Indica o fim de uma unidade de tempo.
 * - 'U': Desbloqueia o primeiro processo na fila de bloqueados.
 * - 'P': Imprime o estado atual do sistema.
 * - 'T': Imprime o tempo de retorno médio e finaliza o simulador (este comando aparece apenas uma vez, como o último comando).
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que gerencia os processos.
 * @param comando O caractere que indica o comando a ser executado.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      process_manager(pm, 'Q'); // Fim de uma unidade de tempo
 *      process_manager(pm, 'U'); // Desbloqueia o primeiro processo na fila de bloqueados
 *      process_manager(pm, 'P'); // Imprime o estado atual do sistema
 *      process_manager(pm, 'T'); // Imprime o tempo de retorno médio e finaliza o simulador
 */
void process_manager(ProcessManager *Pm, char comando);

#endif
