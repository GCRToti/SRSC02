#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

// pra funcionar exatamente na ordem que se espera de um cinema, é necessário ter um semaforo que considere a ocupação das cadeiras, pra so entrar gente quando o resto sair

#define N 10 // limite de fans por sessão
#define FAN_AMOUNT 20 // quantidade de fans na fila
#define MOVIE_TIME 5 // duração do filme

int curFans = 0;

sem_t queue; // fila de fans para entrada
sem_t show; // fans em atendimento
sem_t chairs;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void watchMovie(int id)
{
    printf("Filme sendo assistido pelo fan %d...\n", id);
    sleep(MOVIE_TIME);
    printf("Saindo do filme (%d)\n", id);
    sem_post(&chairs);
    return;
}

void callMom(int id)
{
    printf("Ligação do fan %d\n", id);
    printf("Chamando...\n");
    sleep(1);
    printf("O filme é show, mamãe (%d)\n", id);
    return;
}

void showMovie()
{
    printf("Iniciando exibição\n");
    sleep(MOVIE_TIME);
    printf("Encerrando sessão\n");
    return;
}

void* fan(void* args)
{
    int fanId = *((int*) args);
    free(args);
    while (1 == 1)
    {
        pthread_mutex_lock(&mutex);
        curFans += 1;
        pthread_mutex_unlock(&mutex);
        sem_post(&show); // solicitar o ingresso ao showman

        sem_wait(&queue); // preencher posição na fila --> se não tiver mais cadeiras disponiveis, ele espera
        
        
        watchMovie(fanId);
        callMom(fanId);
    }
    return NULL;
}

void* showmanJob()
{
    while (1 == 1)
    {
        int i = 0;
        while(curFans < N)
        {
            sem_wait(&show); // entregar ingresso
        }

        // descontar todos os ingressos
        pthread_mutex_lock(&mutex);
        curFans -= N;
        pthread_mutex_unlock(&mutex);

        for(i = 0; i < N; i++)
        {
            sem_post(&queue); // permitir a entrada
        }

        showMovie(); // exibe o filme

        for(i = 0; i < N; i++) // libera conforme as pessoas saem e espera até ter alguem que saiu pra dar como liberado
        {
            sem_wait(&chairs); // libera as caideiras
        } // só volta a liberar entrada e distribuir os ingressos quando todo mundo sair

    }
    return NULL;
}

int main()
{
    sem_init(&queue, 0, 0);
    sem_init(&show, 0, 0);
    sem_init(&chairs, 0, 0);
    pthread_t fans[FAN_AMOUNT]; // Fans a espera do filme
    pthread_t showman;

    pthread_create(&showman, NULL, showmanJob, NULL);

    for(int i = 0; i < FAN_AMOUNT; i++)
    {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&fans[i], NULL, fan, (void*) (id));
    }

    pthread_join(showman, NULL);
    for(int i = 0; i < FAN_AMOUNT; i++)
    {
        pthread_join(fans[i], NULL);
    }

    sem_destroy(&queue);
    sem_destroy(&show);

    return 0;
}