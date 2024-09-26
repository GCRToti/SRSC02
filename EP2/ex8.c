#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 
#define MAX 1

void *function(void* params);

void *function(void* params)
{
    int param = *((int*) params);

    printf("Thread %d criada\n", param);

    pid_t pid;

    if((pid = fork()) == -1)
    {
        perror("Erro ao fazer fork.\n");
        exit(1);
    }
    else if(pid == 0)
    {
        printf("Eu sou um fork dentro da thread.\n");
        sleep(2);
        printf("Encerrando fork...\n");
    } else {
        printf("Eu sou a thread.\n");
        printf("Encerrando thread %d\n", param);
        pthread_exit(NULL);
    }
    
    return 0;
}

int main() 
{
    
    pthread_t threads[MAX];

    int ret, i, *pi;
    for(i = 0; i < MAX; i++)
    {
        pi = malloc(sizeof(int*));
        *pi = i + 1;
        ret = pthread_create(&threads[i], NULL, function, (void*) pi);
        
        if(ret)
        {
            fprintf(stderr,"falha na criacao da thread...\n");
            exit(1);    
        }
    }
    printf("Main finalizando...\n");
    pthread_exit(NULL);
    return 0;
}