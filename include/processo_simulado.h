#ifndef _processo_simulado_h_
#define _processo_simulado_h_

/**
 * @brief Número máximo de instruções que um processo pode conter.
 */
#define MAX_INSTRUCOES 100
/**
 * @brief Número máximo de processos que o gerenciador pode controlar simultaneamente.
 */
#define MAX_PROCESSOS 50

/**
 * @brief Estrutura que representa uma instrução em um programa de processo.
 *
 * @param instrucao Caractere representando o tipo de instrução.
 * @param valor Valor associado à instrução.
 * @param nomeArq Nome do arquivo associado à instrução (se aplicável).
 */
typedef struct Instrucao{
    char instrucao;
    int valor;
    char nomeArq[50];
} Instrucoes;

/**
 * @brief Estrutura que representa um processo no sistema.
 *
 * @param prioridade Prioridade do processo.
 * @param programa Array de instruções que compõem o programa do processo.
 * @param idPai ID do processo pai (se aplicável).
 * @param idProcesso ID único do processo.
 * @param variavel Variável geral para uso do processo.
 * @param ultimaInstruExec Número da última instrução executada.
 * @param posiUltimaInstru Posição da última instrução do programa.
 * @param tempoInicio Tempo de início do processo.
 * @param totalCpuUsada Tempo total de CPU utilizado pelo processo.
 */
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

/**
 * @brief Transfere as instruções de um arquivo de texto para um processo.
 *
 * Esta função lê as instruções de um arquivo de texto, se ele existir, e
 * as insere na estrutura do processo especificado. Cada linha do arquivo
 * deve conter uma instrução que será convertida e armazenada no array de
 * instruções do processo. A função não retorna nenhum valor, mas modifica
 * a estrutura do processo fornecida como argumento.
 *
 * @param P Um ponteiro para a estrutura Processo onde as instruções serão inseridas.
 * @param arqNome O nome do arquivo de texto que contém as instruções.
 *
 * @example
 * Exemplo de uso:
 *      Processos proc;
 *      inserir_instrucoes_arq(&proc, "init");
 */
void inserir_instrucoes_arq(struct Processo *P, char *arqNome);

/**
 * @brief Cria um processo inicialmente vazio.
 *
 * Esta função aloca e inicializa um novo processo com os IDs fornecidos. O processo
 * criado terá o array de instruções vazio e os demais campos inicializados com valores
 * padrão. A função retorna um ponteiro para a nova estrutura de processo criada.
 *
 * @param idProcesso O ID único do novo processo.
 * @param idPai O ID do processo pai (se aplicável).
 * @return Um ponteiro para a nova estrutura Processo criada.
 *
 * @example
 * Exemplo de uso:
 *      unsigned idProcesso = 1;
 *      unsigned idPai = 0; // Processo raiz
 *      struct Processo *novoProcesso = cria_processo(idProcesso, idPai);
 */
struct Processo *cria_processo(unsigned idProcesso, unsigned idPai);

/**
 * @brief Imprime informações detalhadas sobre um processo.
 *
 * Esta função exibe todas as informações relevantes de um processo, incluindo
 * sua prioridade, ID do processo, ID do processo pai, variável geral, última
 * instrução executada, posição da última instrução executada, tempo de início,
 * e tempo total de CPU utilizado. Além disso, imprime todas as instruções do
 * programa do processo.
 *
 * @param P A estrutura Processo cujas informações serão impressas.
 *
 * @example
 * Exemplo de uso:
 *      Processos proc;
 *      // Inicializa e configura o processo...
 *      printa_processo(proc);
 */
void printa_processo(struct Processo P);

/**
 * @brief Insere uma instrução no programa de um processo.
 *
 * Esta função insere uma instrução no programa de um processo específico.
 * A instrução fornecida é adicionada ao final do programa existente do processo.
 *
 * @param P Um ponteiro para a estrutura Processo onde a instrução será inserida.
 * @param I A instrução que será inserida no programa do processo.
 *
 * @example
 * Exemplo de uso:
 *      Processos *proc = cria_processo(1, 0);
 *      Instrucoes inst;
 *      // Inicializa e configura a instrução...
 *      insere_instru(&proc, inst);
 */
void insere_instru(struct Processo *P, struct Instrucao I);

/**
 * @brief Libera a memória alocada para um processo.
 *
 * Esta função libera todos os recursos alocados dinamicamente para o processo
 * especificado, incluindo seu programa de instruções, se aplicável. Após a chamada
 * desta função, o ponteiro para o processo fornecido não é mais válido.
 *
 * @param P Um ponteiro para a estrutura Processo que será destruída.
 *
 * @example
 * Exemplo de uso:
 *      Processos *proc = cria_processo(1, 0);
 *      // Usa o processo...
 *      destroi_processo(proc);
 */
void destroi_processo(struct Processo *P);

/**
 * @brief Executa uma instrução específica em um processo.
 *
 * Esta função executa a instrução indicada pelo número `numInstru` no processo
 * apontado pelo ponteiro duplo `P`. O processo é modificado conforme necessário
 * pela execução da instrução. O retorno indica o resultado da execução da instrução.
 *
 * @param P Um ponteiro duplo para a estrutura Processo que será modificado pela execução da instrução.
 * @param numInstru O número da instrução a ser executada no processo.
 * @return Um valor inteiro indicando o resultado da execução da instrução.
 *
 * @example
 * Exemplo de uso:
 *      Processos *proc = cria_processo(1, 0);
 *      // Insere instruções no processo...
 *      int resultado = executar_instrucao_processo(&proc, 0);
 */
int executar_instrucao_processo(struct Processo **P, unsigned numInstru);

/**
 * @brief Pesquisa um processo pelo seu ID em um array de ponteiros para Processos.
 *
 * Esta função busca por um processo com o ID especificado no array de ponteiros para
 * Processos `P`. Se encontrar o processo, retorna o índice do processo no array.
 * Caso contrário, retorna o valor definido por MAX_PROCESSOS, indicando que o processo
 * não foi encontrado.
 *
 * @param P Um array de ponteiros para estruturas Processos onde será realizada a busca.
 * @param numProcessos O número total de processos no array.
 * @param id O ID do processo que está sendo pesquisado.
 * @return O índice do processo no array se encontrado, ou MAX_PROCESSOS se não encontrado.
 *
 * @example
 * Exemplo de uso:
 *      Processos *arrayProc[MAX_PROCESSOS];
 *      unsigned numProcessos = 0;
 *      // Adiciona processos ao array...
 *      unsigned indice = pesquisa_processo(arrayProc, numProcessos, 1);
 */
unsigned pesquisa_processo(Processos *P[], unsigned numProcessos, unsigned id);

/**
 * @brief Copia um processo para outra estrutura de processo.
 *
 * Esta função realiza uma cópia profunda do processo original `pOriginal` para uma nova
 * estrutura de processo `pCopia`. Todos os campos e instruções do processo original são
 * copiados para a nova estrutura. A função não retorna nenhum valor diretamente, mas
 * modifica a estrutura apontada por `pCopia`.
 *
 * @param pCopia Um ponteiro para o ponteiro da estrutura Processo que receberá a cópia.
 *               Deve ser um ponteiro nulo inicialmente para que a função aloque memória
 *               para a nova estrutura.
 * @param pOriginal Um ponteiro para a estrutura Processo original que será copiada.
 *
 * @example
 * Exemplo de uso:
 *      Processos *copia = NULL;
 *      Processos original;
 *      // Configura o processo original...
 *      copia_processo(&copia, &original);
 */
void copia_processo(struct Processo **pCopia, struct Processo *pOriginal);

/**
 * @brief Finaliza um processo com o ID especificado em um array de ponteiros para Processos.
 *
 * Esta função busca por um processo com o ID especificado no array de ponteiros para
 * Processos `P` e o finaliza. Isso geralmente envolve liberar recursos alocados
 * dinamicamente pelo processo. O processo finalizado é removido do array e o número
 * total de processos é atualizado. Retorna a nova quantidade de processos no array.
 *
 * @param P Um array de ponteiros para estruturas Processos onde será realizada a busca e finalização.
 * @param numProcessos Um ponteiro para o número total de processos no array.
 * @param id O ID do processo que deve ser finalizado.
 * @return A nova quantidade de processos no vetor
 *
 * @example
 * Exemplo de uso:
 *      Processos *arrayProc[MAX_PROCESSOS];
 *      unsigned numProcessos = 0;
 *      // Adiciona processos ao array...
 *      unsigned novoTamanho = finaliza_processo(arrayProc, &numProcessos, 1);
 */
unsigned finaliza_processo(Processos *P[], unsigned numProcessos, unsigned id);

#endif