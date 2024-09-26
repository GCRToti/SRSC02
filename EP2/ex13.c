#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TAMANHO 20

typedef struct dados
{
    int* lista;
    int media;
    float mediana;
} Dados;


void* media(void* params)
{
    Dados* dados = (Dados*) params;

    int soma = 0, i;
    float media = 0;

    for(i = 0; i < 20; i++)
    {
        soma += dados->lista[i];
    }

    media = (float) soma / TAMANHO;

    dados->media = media;

    pthread_exit(NULL);
    return NULL;
}

int comparar(const void* a, const void* b)
{
    return (*(int*) a - *(int*) b); // achar a diferença entre os valores
}

void* mediana(void* params)
{
    Dados* dados = (Dados*) params;

    float mediana = 0;

    qsort(dados->lista, TAMANHO, sizeof(int), &comparar); // organiza os valores com base na função comparar

    mediana = (dados->lista[(TAMANHO / 2) - 1] + dados->lista[TAMANHO / 2]) / 2.0; // como a quantidade é par, tem que fazer a media dos dois centrais

    dados->mediana = mediana;

    pthread_exit(NULL);
    return NULL;
}

int main()
{
    srand(time(NULL));
    Dados dados;
    int i;
    pthread_t t1, t2;

    dados.lista = malloc(sizeof(int) * TAMANHO);
    
    for(i = 0; i < TAMANHO; i++)
    {
        dados.lista[i] = rand() % 101;
    }

    pthread_create(&t1, NULL, media, (void*) &dados);
    pthread_create(&t2, NULL, mediana, (void*) &dados);

    printf("Numeros: ");

    for(i = 0; i < TAMANHO; i++)
    {
        printf("%d ", dados.lista[i]);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nMedia: %d\n", dados.media);
    printf("Mediana: %f\n", dados.mediana);

    pthread_exit(NULL);
    return 0;
}