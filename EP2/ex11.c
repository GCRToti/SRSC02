#include <stdio.h>
#include <pthread.h>

typedef struct relogio
{
    int h;
    int m;
}Relogio;

void* mensagem(void* params)
{
    Relogio* r = (Relogio*) params;

    if(r->h < 4)
    {
        printf("Boa Noite!\n");
    }
    else if(r->h < 12)
    {
        printf("Bom dia!\n");
    } else if(r->h > 18)
    {
        printf("Boa Noite!\n");
    } else {
        printf("Boa tarde!\n");
    }

    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t thread;
    Relogio relogio;

    printf("Digite a hora: ");

    scanf("%d", &relogio.h);

    printf("Digite os minutos: ");
    scanf("%d", &relogio.m);

    pthread_create(&thread, NULL, mensagem, (void*)&relogio);

    pthread_exit(NULL);
    return 0;
}