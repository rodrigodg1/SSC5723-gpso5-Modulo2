#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MaxItems 5   // Maximo de itens de producao/consumo
#define BufferSize 5 // Tamanho do Buffer

/*Armazenam as posições do buffer que será inserido (in) pelo produtor e removido (out) pelo consumidor*/
int in = 0;
int out = 0;

/*Criação do buffer (recurso compartilhado)*/
int buffer[BufferSize];

/*Definição do mutex, para o acesso ao recurso compartilhado*/
pthread_mutex_t mutex;

/* semaforo (empty) para contar o número de vagas que estão vazias
 e um semaforo (full) para contar o número de vagas que estão cheias
*/
sem_t empty;
sem_t full;

int espacos_livres = BufferSize - 1;

/*Função apenas para retornar se o buffer está cheio ou não*/
char *verifica_espacos_livres(int espacos_livres)
{
    char *status;
    status = malloc(sizeof(char) * 15);

    if (espacos_livres == 0)
    {
        strcpy(status, "CHEIO");
    }
    else
    {
        strcpy(status, "");
    }

    return status;
}

/*Função para produção dos itens pelo produtor*/
void *produtor(void *pno)
{
    int item;
    char *bufferStatus;

    for (int i = 0; i < MaxItems; i++)
    {

        // produz um número inteiro aleatório
        item = 1 + rand() % (50 - 1);

        /*verifica o valor de empty, se > 0, o decremento irá acontecer 
        e será obtido uma "trava" para acessar a região compartilhada e adicionar o item em uma posição do buffer.
        Se empty = 0, então não existe espaço no buffer e o processo produtor é bloqueado.
        */
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;

        //verifica se se deve mostrar uma label de "CHEIO" representando o buffer
        bufferStatus = verifica_espacos_livres(espacos_livres);

        printf("Produtor %d: \t Adicionou o Item (%d) \t na posicao [%d]  \t%s \n", *((int *)pno), buffer[in], in, bufferStatus);
        free(bufferStatus);
        espacos_livres--;

        // atualiza a prox. posição do buffer
        in = (in + 1) % BufferSize;

        //delay
        //for (int j = 0; j < (0xFFFFFFFF); j++);

        /*Libera uma trava (bloqueio)*/
        pthread_mutex_unlock(&mutex);

        /* 
       incrementa um valor ao semaforo full, consequentemente se algum outro processo já havia solicitado a entrada
       e está bloqueado pela chamada sem_wait(), será acordado.
       */
        sem_post(&full);
    }
}
void *consumidor(void *cno)
{
    for (int i = 0; i < MaxItems; i++)
    {

        /*o semáforo full será decrementado se full > 0 e o processo entrará na região crítica, bloqueando-a.
        Caso contrário, o processo consumidor será bloqueado.
       */
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        /* o item consumido, segue a ordem da produção*/
        int item = buffer[out];

        printf("Consumidor %d: \t Removeu o  Item (%d) \t na posicao [%d] \t \n", *((int *)cno), item, out);
        espacos_livres++;
        // atualiza a prox. posição do buffer
        out = (out + 1) % BufferSize;

        //delay
        //for (int j = 0; j < (0xFFFFFFFF); j++);

        /*libera a trava e incrementa o semáforo da contagem das posições vazias do buffer*/
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{

    /* definição da qtd de produtor/consumidor */
    int qtd_produtor = 2;
    int qtd_consumidor = 2;

    /* criação da thread para o produtor e o consumidor */
    pthread_t pro[qtd_produtor], con[qtd_consumidor];

    /* cria/inicializa o mutex */
    pthread_mutex_init(&mutex, NULL);

    /* inicia o semáforo empty com o tamanho do buffer.
     e o semaforo full com zero items produzidos.
    */
    sem_init(&empty, 0, BufferSize);
    sem_init(&full, 0, 0);

    // labels (nomes) para os produtores e consumidores
    int p[2] = {1, 2};
    int c[2] = {1, 2};

    printf("\nID do Produtor \tItem Adicionado \tPosicao do Item \tBuffer\n\n");

  /* Criação das Threads
     Concorrência ao acesso ao buffer */ 
    for (int i = 0; i < qtd_produtor; i++)
    {
        pthread_create(&pro[i], NULL, (void *)produtor, (void *)&p[i]);
    }
    for (int i = 0; i < qtd_consumidor; i++)
    {
        pthread_create(&con[i], NULL, (void *)consumidor, (void *)&c[i]);
    }

    
 
 
   /* garante a continuação da execução das threads assim que uma for finalizada */ 
    for (int i = 0; i < qtd_produtor; i++)
    {
        pthread_join(pro[i], NULL);
    }
    for (int i = 0; i < qtd_consumidor; i++)
    {
        pthread_join(con[i], NULL);
    }

    // Elimina os semaforos e o mutex
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
