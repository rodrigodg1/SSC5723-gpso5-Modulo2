#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MaxItems 5   // Maximo de itens que de producao/consumo
#define BufferSize 5 // Tamanho do Buffer

int in = 0;
int out = 0;
int buffer[BufferSize];

pthread_mutex_t mutex;

/* semaforo (empty) para contar o numero de vagas que estao vazias
 e um semaforo (full) para contar o numero de vagas que estao cheias
*/
sem_t empty;
sem_t full;

int espacos_livres = BufferSize - 1;



char* verifica_espacos_livres (int espacos_livres)
{
    char *status;
    status = malloc (sizeof (char) * 15);
    
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


void *producer(void *pno)
{
    int item;
    char *bufferStatus;

    for (int i = 0; i < MaxItems; i++)
    {

        item = 1 + rand() % (50 - 1); // produz um numero inteiro aleatorio

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;

        //veriffica se existe posicoes livres no buffer
        bufferStatus = verifica_espacos_livres(espacos_livres);
        
        printf("Produtor %d: \t Adicionou o Item (%d) \t na posicao [%d]  \t%s \n", *((int *)pno), buffer[in], in, bufferStatus);
        free(bufferStatus);
        espacos_livres--;
        
        in = (in + 1) % BufferSize;

        //delay
        //for (int j = 0; j < (0xFFFFFFFF); j++);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}
void *consumer(void *cno)
{
    for (int i = 0; i < MaxItems; i++)
    {

        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];

        printf("Consumidor %d: \t Removeu o  Item (%d) \t na posicao [%d] \t \n", *((int *)cno), item, out);
        espacos_livres++;
        out = (out + 1) % BufferSize;

        //delay
        //for (int j = 0; j < (0xFFFFFFFF); j++);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{

    int qtd_produtor = 2;
    int qtd_consumidor = 2;

    pthread_t pro[qtd_produtor], con[qtd_consumidor];

    //cria um mutex
    pthread_mutex_init(&mutex, NULL);

    // inicia o semaforo empty
    sem_init(&empty, 0, BufferSize);
    sem_init(&full, 0, 0);

    int a[2] = {1, 2}; // nomeia os produtores e consumidores

    printf("\nID do Produtor \tItem Adicionado \tPosicao do Item \tBuffer\n\n");

    for (int i = 0; i < qtd_produtor; i++)
    {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for (int i = 0; i < qtd_consumidor; i++)
    {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for (int i = 0; i < qtd_produtor; i++)
    {
        pthread_join(pro[i], NULL);
    }
    for (int i = 0; i < qtd_consumidor; i++)
    {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
