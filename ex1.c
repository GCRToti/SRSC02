#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("inicio do codigo\n");

    fork();

    printf("fim do codigo\n");

    return 0;
}