
## Projeto do Modulo 2 - Produtor e Consumidor
Dois processos (produtores e consumidores) compartilham um buffer de tamanho fixo. O produtor insere a informação no buffer e o consumidor retira a informação do buffer.
 -   Se o buffer estiver cheio, o produtor é bloqueado.
    
 -   Se o buffer estiver vazio, o consumidor é bloqueado.
    
 -   Apenas um processo (produtor ou consumidor) pode acessar o buffer em um dado instante.
    
 - É possível generalizar para *n* produtores e *m* consumidores.


####  Exclusão Mútua
A solução consiste na implementação em que aconteça exclusão mútua entre os processos produtores e os consumidores, isto é, garantir que produtores e consumidores não acessem a região crítica (buffer) ao mesmo tempo.

#### Utilização de Semáforos:
 - **full**: para contar o número de espaços ocupados no buffer
 - **empty**: para contar o número de espaços vazios no buffer
 - **mutex**: para garantir que consumidor e produtor não acessem o buffer ao mesmo tempo, ou seja, garantir a exclusão mútua.

## Como Compilar ?
Para compilar o arquivo, execute o comando: `gcc produtor-consumidor.c -pthread`
## Como Executar ?
Para executar o arquivo, execute o comando: `./a.out`
## Visualizar as Threads
 1. Utilizar o **htop**. Se necessário, instalar pelo comando ` sudo apt-get install htop `
 2. Enquanto o programa estiver em execução, abra outro terminal e execute `htop`
 3. Utilize o filtro e digite `./a.out`
 4. Visualizar a alternância dos estados dos processos (produtor e consumidor) com a coluna **s (state)** 


