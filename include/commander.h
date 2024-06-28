#ifndef _commander_h_
#define _commander_h_

#include "../include/process_manager.h"

/**
 * @brief Envia comandos para um processo `ProcessManager` através de um pipe.
 *
 * Esta função cria um pipe para comunicação entre processos. Ela cria um processo filho para executar o `ProcessManager`
 * e envia comandos, um por segundo, para ele através do pipe. Os comandos são lidos da entrada padrão (`stdin`) pelo
 * processo pai e escritos no pipe. O processo filho lê os comandos do pipe e os executa através da função `process_manager`.
 * Após enviar todos os comandos, o processo pai espera a execução dos comandos pelo processo filho e finaliza.
 *
 * @param Pm Um ponteiro para a estrutura `ProcessManager` que gerencia os processos.
 * @return 0 se todos os comandos foram enviados e executados com sucesso, ou um valor negativo em caso de erro.
 *
 * @example
 * Exemplo de uso:
 *      ProcessManager *pm = cria_process_manager();
 *      int resultado = commander(pm);
 *      if (resultado == 0) {
 *          // Comandos enviados e executados com sucesso
 *      } else {
 *          // Erro ao enviar ou executar comandos
 *      }
 */
int commander(ProcessManager *Pm);

#endif