#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
 
#define MAX 1
#define STACK_SIZE 64 * 1024

int function2(void* params);

int function2(void* params)
{
    int p = (intptr_t) params;
    printf("Sou uma thread dentro de outra thread.\n");
    printf("Recebi o parametro %d.\n", p);


    // char* arg[] = {"execv_ex", NULL};

    // execv("./execv_ex", arg);

    

    sleep(p);

    printf("Thread encerrando... %d %d\n", getpid(), gettid());
    return 0;
}

void *function(void* params);

void *function(void* params)
{
    int param = *((int*) params);

    printf("Thread %d criada, tid = %d\n", param, gettid());

    int tid;

    void* childStack = malloc(STACK_SIZE);

    tid = clone(&function2, childStack + STACK_SIZE, CLONE_SIGHAND | CLONE_FS |  CLONE_VM | CLONE_FILES | CLONE_THREAD, (void*) 1);

    printf("Thread com tid %d criada.\n", tid);

    system("ps -eLF | grep -e LWP -e saida");

    sleep(5);

    free(childStack);

    printf("Eu sou a primeira thread.\n");
    printf("Encerrando thread %d\n", param);
    pthread_exit(NULL);
    
    
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