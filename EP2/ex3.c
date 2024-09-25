#define _GNU_SOURCE // acesso especifico do linux

#include<stdio.h> // printf()
#include<stdlib.h> //malloc()
#include<stdint.h> //intptr_t
#include<unistd.h> //getpid(),gettid()
#include<sched.h> //clone(),flags

#define STACK_SIZE 64 * 1024 // 64KB

int function(void* params);

int function(void* params)
{
    printf("Thread iniciando... %d %d\n", getpid(), gettid());

    int sleepTime = (intptr_t) params;
    sleep(sleepTime);

    printf("Thread encerrando... %d %d\n", getpid(), gettid());

    return sleepTime;
}

int main()
{
    int tid;

    void* childStack = malloc(STACK_SIZE);

    if (childStack == NULL) {
        perror("Erro ao alocar memória para a pilha");
        exit(1);
    }

    printf("Antes de clonar - pid: %d\n", getpid());

    tid = clone(&function, childStack + STACK_SIZE, CLONE_SIGHAND | CLONE_FS |  CLONE_VM | CLONE_FILES | CLONE_THREAD, (void*) 10);

    if (tid == -1) {
        perror("Erro ao criar thread");
        free(childStack);
        exit(1);
    }

    printf("Depois de clonar - pid: %d %d\n", getpid(), gettid());

    system("ps -eLF | grep -e LWP -e saida");

    sleep(5);

    printf("Encerrando antes de %d\n", tid);

    free(childStack);

    // quando acabar, executar ps -eLF | grep -e LWP -e saida no terminal

    return 0;
}