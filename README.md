# Simulador de Gerenciamento de Processos

## Descrição

Este projeto é um simulador de gerenciamento de processos que simula cinco funções principais: criação de processos, substituição do processo atual por outro processo, transição de estados de um processo, escalonamento e troca de contexto. A aplicação utiliza chamadas de sistema do Linux como `fork()`, `wait()`, `pipe()`, e `sleep()`.

## Estrutura do Simulador

### Processos do Simulador

- **Commander**: Inicia a simulação, cria um processo `Process Manager` e lê comandos da entrada padrão para passar ao `Process Manager` através de um pipe.
- **Process Manager**: Gerencia a criação, execução e finalização dos processos simulados, além de gerenciar suas transições de estado, escalonamento e troca de contexto.
- **Reporter**: Imprime o estado atual do sistema quando solicitado pelo `Process Manager`.

### Tipos de Comandos

O processo `Commander` pode enviar os seguintes comandos ao `Process Manager`:
1. `Q`: Indica o fim de uma unidade de tempo.
2. `U`: Desbloqueia o primeiro processo simulado na fila de bloqueados.
3. `P`: Imprime o estado atual do sistema.
4. `T`: Imprime o tempo de retorno médio e finaliza o simulador (este comando aparece apenas uma vez, como o último comando).

### O Processo Simulado

Um processo simulado gerencia a execução de um programa que manipula uma única variável inteira. Cada processo possui um conjunto de instruções que pode incluir operações como atualizar, somar ou subtrair o valor da variável, bloquear ou terminar o processo, criar novos processos e substituir o programa atual por um novo.

#### Tipos de Instruções

1. `S n`: Atualiza o valor da variável inteira para `n`.
2. `A n`: Soma `n` à variável inteira.
3. `D n`: Subtrai `n` da variável inteira.
4. `B`: Bloqueia o processo simulado.
5. `E`: Termina o processo simulado.
6. `F n`: Cria um novo processo simulado como uma cópia exata do pai.
7. `R arquivo`: Substitui o programa do processo simulado com o programa do arquivo especificado.

### Estruturas de Dados do Process Manager

- **Tempo**: Inteiro que começa em 0 e é incrementado a cada comando `Q`.
- **CPU**: Estrutura que simula a execução do processo atual, contendo ponteiros para o programa, contador de programa, valor inteiro e outras informações relevantes.
- **TabelaPcb**: Array com uma entrada para cada processo ativo, contendo informações como id do processo, id do processo pai, ponteiro para o contador de programa, valor inteiro, prioridade, estado, tempo de início e CPU utilizada até o momento.
- **EstadoPronto**: Contém índices dos processos na `TabelaPcb` que estão prontos para execução.
- **EstadoBloqueado**: Contém índices dos processos na `TabelaPcb` que estão bloqueados.
- **EstadoExecutando**: Contém o índice do processo atualmente em execução.

### Comportamento do Process Manager

O `Process Manager` processa comandos recebidos do `Commander` e gerencia a execução de processos simulados conforme as instruções recebidas. Inclui funções para:
- Executar a próxima instrução do processo atual (comando `Q`).
- Desbloquear o primeiro processo da fila de bloqueados (comando `U`).
- Criar um processo `Reporter` para imprimir o estado atual do sistema (comando `P`).
- Finalizar a simulação após criar um `Reporter` para imprimir o tempo de retorno médio (comando `T`).

### Escalonamento e Troca de Contexto

O `Process Manager` implementa uma estratégia de escalonamento para decidir qual processo deve ser executado a seguir. A troca de contexto envolve copiar o estado do processo atual da `CPU` para a `TabelaPcb` e carregar o estado do próximo processo da `TabelaPcb` para a `CPU`.

### Processo Reporter

O `Reporter` imprime o estado atual do sistema, incluindo informações sobre:
- Tempo atual
- Processo em execução (pid, ppid, prioridade, valor, tempo de início, CPU usada até agora)
- Processos bloqueados
- Processos prontos para execução

## Compilação e Execução

Para compilar e executar o simulador, siga os passos abaixo:

1. Compile o programa:
   ```sh
   make
2. Execute o programa:
   ```sh
   ./main
