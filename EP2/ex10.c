#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
 
#define MAX 1
#define STACK_SIZE 64 * 1024



void *function(void* params);

void *function(void* params)
{
    int param = *((int*) params);

    printf("Thread %d criada, tid = %d\n", param, gettid());

    char* arg[] = {"execv_ex", NULL};

    execv("./execv_ex", arg);

    printf("Eu sou a primeira thread.\n");
    printf("Encerrando thread %d\n", param);
    pthread_exit(NULL);
    
    
    return 0;
}

int main() 
{
    printf("EX 10\n");
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
    sleep(10);
    printf("Main finalizando...\n");
    pthread_exit(NULL);
    return 0;
}