#define _GNU_SOURCE // acesso especifico do linux

#include <stdio.h>  // printf()
#include <stdlib.h> // malloc()
#include <unistd.h> // getpid, gettid, getppid
#include <sched.h>  // clone, flags
#include <stdint.h> // intptr_t

#define STACK_SIZE 64 * 1024 // 64KB

int function(void* params);

int function2(void* params);

int function2(void* params)
{
    int p = (intptr_t) params;
    printf("Sou uma thread dentro de outra thread.\n");
    printf("Recebi o parametro %d.\n", p);

    system("ps -eLF | grep -e LWP -e saida");
    
    sleep(p);
    
    printf("Thread encerrando... %d\n", gettid());
    return 0;
}

int function(void* params) {

    printf("Thread iniciando... %d\n", gettid());

    int sleepTime = (intptr_t) params;

    int tid;

    void* childStack = malloc(STACK_SIZE);

    tid = clone(&function2, childStack + STACK_SIZE, CLONE_SIGHAND | CLONE_FS |  CLONE_VM | CLONE_FILES | CLONE_THREAD, (void*) 30);

    printf("Thread com tid %d criada.\n", tid);

    sleep(sleepTime);

    free(childStack);

    system("ps -eLF | grep -e LWP -e saida");

    printf("Thread encerrando... %d\n", gettid());
    return 0;
}

int main()
{

    int tid;

    void* childStack = malloc(STACK_SIZE);

    tid = clone(&function, childStack + STACK_SIZE, CLONE_SIGHAND | CLONE_FS |  CLONE_VM | CLONE_FILES | CLONE_THREAD, (void*) 10);

    printf("Thread com tid %d criada.\n", tid);

    sleep(40);

    free(childStack);

    system("ps -eLF | grep -e LWP -e saida");

    printf("Encerrando processo principal.\n");

    return 0;
}