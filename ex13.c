#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    pid_t pid_P1, pid_P2;

    if((pid_P1 = fork()) < 0)
    {
        perror("Erro na criação do processo P1.\n");
    } else if(pid_P1 == 0)
    {
        printf("Eu sou o processo P1! Meu pid é %d. Meu pai é o processo com o pid %d.\n", getpid(), getppid());
        sleep(4);
        printf("Sou P1 e terminei minha execução.\n");
    } else if((pid_P2 = fork()) < 0)
    {
        perror("Erro na criação do processo P2.\n");
    } else if(pid_P2 == 0)
    {
        printf("Eu sou o processo P2! Meu pid é %d. Meu pai é o processo com o pid %d.\n", getpid(), getppid());
        sleep(3);
        printf("Sou P2 e terminei minha execução.\n");
    } else {
        printf("Sou P e estou esperando meus filhos.\n");

        // alternativamente, a função waitpid tambem pode ser utilizada para esperar um pid especifico.

        if(wait(NULL) < 0)
        {
            perror("Erro na espera dos filhos.");
        } else {
            if(wait(NULL) < 0)
            {
                perror("Erro na espera dos filhos.");
            } else {
                printf("Sou P e terminei minha execução.\n");
            }
        }
    }

    return 0;
}