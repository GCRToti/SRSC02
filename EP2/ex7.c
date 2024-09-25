#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 20

void *function(void* params);

void *function(void* params)
{
    int param = *((int*) params);

    printf("Thread %d criada\n", param);

    sleep(1);

    printf("Encerrando thread %d\n", param);
    free(params);
    pthread_exit(NULL);
}

int main()
{
    
    pthread_t threads[MAX];

    int ret, i, *pi;
    for(i = 0; i < MAX; i++)
    {
        pi = malloc(sizeof(int*));
        *pi = i + 1;
        ret = pthread_create(&threads[i], NULL, function, (void*)pi);
        
        if(ret)
        {
            fprintf(stderr,"falha na criacao da thread...\n");
            exit(1);    
        }
    }
    printf("Main finalizando...\n");
    pthread_exit(NULL);
    // return 0;
}