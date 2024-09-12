#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    pid_t pid_P1, pid_P2;

    if((pid_P1 = fork()) < 0)
    {
        perror("Erro na criação do processo P1.\n");
    } else if(pid_P1 == 0)
    {
        printf("Eu sou o processo P1! Meu pid é %d. Meu pai é o processo com o pid %d.\n", getpid(), getppid());
    } else if((pid_P2 = fork()) < 0)
    {
        perror("Erro na criação do processo P2.\n");
    } else if(pid_P2 == 0)
    {
        printf("Eu sou o processo P2! Meu pid é %d. Meu pai é o processo com o pid %d.\n", getpid(), getppid());
    }

    return 0;
}