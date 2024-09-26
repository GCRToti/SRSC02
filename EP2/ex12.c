#include <stdio.h>
#include <pthread.h>

typedef struct intervalo
{
    int a, b;
    int soma, produto;
} Intervalo;

void* soma(void* params)
{
    Intervalo* intervalo = (Intervalo*) params;
    int i, soma = 0;
    for(i = intervalo->a; i <= intervalo->b; i++)
    {
        if((i & 0x01) == 0)
        {
            soma += i;
        }
    }

    intervalo->soma = soma;

    pthread_exit(NULL);
}


void* produto(void* params)
{
    Intervalo* intervalo = (Intervalo*) params;
    int i, produto = 1;
    for(i = intervalo->a; i <= intervalo->b; i++)
    {

        if((i & 0x01) != 0)
        {
            produto *= i;
        }
        
    }

    intervalo->produto = produto;

    pthread_exit(NULL);
    return NULL;
}

int main()
{
    Intervalo intervalo;
    pthread_t t1, t2;

    scanf("%d", &intervalo.a);
    scanf("%d", &intervalo.b);

    pthread_create(&t1, NULL, soma, (void*)&intervalo);
    pthread_create(&t2, NULL, produto, (void*)&intervalo);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Soma pares: %d\n", intervalo.soma);
    printf("Produto impares: %d\n", intervalo.produto);

    pthread_exit(NULL);
    return 0;
}