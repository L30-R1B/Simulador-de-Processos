#ifndef _reporter_h_
#define _reporter_h_

#include "../include/process_manager.h"

/**
 * @brief Imprime informações sobre o gerenciador de processos.
 *
 * Esta função exibe informações detalhadas sobre o gerenciador de processos
 * fornecido. Ela imprime o estado atual dos processos gerenciados, incluindo
 * detalhes como ID do processo, estado, tempo de execução, e outros dados
 * relevantes. A função não retorna nenhum valor.
 *
 * @param Pm Um ponteiro para a estrutura ProcessManager que contém as informações
 *           sobre os processos gerenciados.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager pm;
 *      // Inicializa e gerencia os processos...
 *      process_reporter(&pm); // Exibe as informações do gerenciador de processos
 */
void process_reporter(ProcessManager *Pm);

#endif