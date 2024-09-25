#define _GNU_SOURCE // acesso especifico do linux

#include <stdio.h>  // printf()
#include <stdlib.h> // malloc()
#include <unistd.h> // getpid, gettid, getppid, fork, pid_t
#include <sched.h>  // clone, flags
#include <stdint.h> // intptr_t
#include <sys/wait.h> // wait

#define STACK_SIZE 64 * 1024 // 64KB

int function(void* params);

int function(void* params) {

    printf("Thread iniciando... %d %d\n", getpid(), gettid());

    pid_t pid;

    int sleepTime = (intptr_t) params;

    

    if((pid = fork()) == -1)
    {
        perror("Erro ao fazer fork");
        return 1;
    } else if(pid == 0)
    {
        printf("Eu sou um fork em uma thread.\n");
        printf("Meu pid é: %d.\n", getpid());
        printf("Pid do meu pai é: %d.\n", getppid());
        return 0;
    } else{ 
       
        
        int status;

        wait(&status);
        
        if (WIFEXITED(status))
        {
            printf("Exit status: %d\n", WEXITSTATUS(status));
        }

        system("ps -eLF | grep -e LWP -e saida");

        sleep(sleepTime);

        printf("Thread encerrando... %d %d\n", getpid(), gettid());
    
    }


    return 0;
}

int main()
{

    int tid;

    void* childStack = malloc(STACK_SIZE);

    tid = clone(&function, childStack + STACK_SIZE, CLONE_SIGHAND | CLONE_FS |  CLONE_VM | CLONE_FILES | CLONE_THREAD, (void*) 10);

    printf("Thread com tid %d criada.\n", tid);

    sleep(20);

    free(childStack);

    system("ps -eLF | grep -e LWP -e saida");

    printf("Encerrando processo principal.\n");

    return 0;
}