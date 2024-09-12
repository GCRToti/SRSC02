#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    pid_t pid_P1;
    const pid_t parentPid = getpid();



    if((pid_P1 = fork()) < 0)
    {
        perror("Erro na criação do processo P1.\n");
    } else if(pid_P1 == 0)
    {
        printf("Eu sou o processo P1! Meu pid é %d. Meu pai é o processo com o pid %d.\n", getpid(), getppid());
        printf("Agora meu processo pai é %d\n", getppid());

        if(getppid() != parentPid)
        {
            printf("O processo pai já se encerrou.\n");
        } else {
            printf("O processo pai ainda está em execução\n.");
        }
    }

    return 0;
}