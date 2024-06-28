#ifndef _vetor_h_
#define _vetor_h_

/**
 * @brief Busca um número em um vetor e retorna o índice correspondente.
 *
 * Esta função procura por um número em um vetor. Se o número for encontrado,
 * ela retorna o índice da posição correspondente no vetor. Caso contrário,
 * retorna um valor de erro padrão definido pela constante MAX_PROCESSOS,
 * contida no módulo "processo_simulado.h".
 *
 * @param vetor O vetor onde a busca será realizada.
 * @param tamanho O tamanho do vetor.
 * @param numero O número a ser procurado no vetor.
 * @return O índice do número no vetor, ou MAX_PROCESSOS se não for encontrado.
 *
 * @example
 * Tem-se um vetor:
 *      int X[] = {1, 2, 3, 4, 5};
 *
 * Como resultado tem-se:
 *      pesquisa_num_vet(X, 5, 3); // Retorna 2
 *      pesquisa_num_vet(X, 5, 4); // Retorna 3
 *      pesquisa_num_vet(X, 5, 1); // Retorna 0
 *      pesquisa_num_vet(X, 5, 8); // Retorna MAX_PROCESSOS
 */
unsigned pesquisa_num_vet(unsigned *vetor, unsigned tamanho, unsigned num);

/**
 * @brief Insere um número inédito em um vetor e retorna o novo tamanho do vetor.
 *
 * Esta função insere um número desde que não esteja no vetor.
 * Se o número já existir no vetor, ele não será inserido novamente. 
 * A função retorna o novo tamanho do vetor após a inserção.
 *
 * @param vetor O vetor onde o número será inserido.
 * @param tamanho O tamanho atual do vetor.
 * @param num O número a ser inserido no vetor.
 * @return O novo tamanho do vetor após a inserção.
 *
 * @example
 * Tem-se um vetor:
 *      unsigned X[6] = {1, 2, 3, 5, 6};
 *      unsigned tamanho = 5;
 *
 * Após a inserção tem-se:
 *      insere_num_vet(X, tamanho, 4); // Retorna 6, vetor torna-se {1, 2, 3, 4, 5, 6}
 *      insere_num_vet(X, tamanho, 7); // Retorna 7, vetor torna-se {1, 2, 3, 4, 5, 6, 7}
 *      insere_num_vet(X, tamanho, 3); // Retorna 5, vetor permanece {1, 2, 3, 5, 6}
 */
unsigned insere_num_vet(unsigned *vetor, unsigned tamanho, unsigned num);

/**
 * @brief Remove um número de um vetor e retorna o novo tamanho do vetor.
 *
 * Esta função remove a primeira ocorrência de um número em um vetor. Se o número
 * for encontrado, ele é removido e os elementos subsequentes são deslocados para
 * preencher a lacuna. A função retorna o novo tamanho do vetor após a remoção.
 * Se o número não for encontrado, o vetor permanece inalterado e o tamanho original
 * é retornado.
 *
 * @param vetor O vetor do qual o número será removido.
 * @param tamanho O tamanho atual do vetor.
 * @param num O número a ser removido do vetor.
 * @return O novo tamanho do vetor após a remoção, ou o tamanho original se o número não for encontrado.
 *
 * @example
 * Tem-se um vetor:
 *      unsigned X[5] = {1, 2, 3, 4, 5};
 *      unsigned tamanho = 5;
 *
 * Após a remoção tem-se:
 *      remove_num_vet(X, tamanho, 3); // Retorna 4, vetor torna-se {1, 2, 4, 5}
 *      remove_num_vet(X, tamanho, 1); // Retorna 4, vetor torna-se {2, 3, 4, 5}
 *      remove_num_vet(X, tamanho, 6); // Retorna 5, vetor permanece {1, 2, 3, 4, 5}
 */
unsigned remove_num_vet(unsigned *vetor, unsigned tamanho, unsigned num);


#endif