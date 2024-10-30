#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define FILE_AMOUNT 8

typedef struct {
    unsigned short dia;
    unsigned short mes;
    unsigned short ano;
    unsigned short hora;
    unsigned short min;
    unsigned short seg;
} data;

typedef struct {
    char nome[8]; // nome do arquivo (deletados iniciam com 0xEB)
    char extensao[3];
    unsigned short sistema;   // proteção: sistema
    unsigned short hidden;    // proteção: oculto
    unsigned short archieved; // proteção: arquivado
    data criacao;             // data de criação
    data acesso;              // data do último acesso
    unsigned long int tamanho;// tamanho do arquivo
    unsigned long int cluster;// cluster inicial
} arquivo;

typedef struct {
    char dados[512];
    unsigned long int prox;
} cluster;

int main(int argc, char* argv[])
{
    unsigned long int clusterStack[100];
    arquivo arquivos[FILE_AMOUNT];

    FILE* disco = fopen("Disco.dat", "rb");

    if(disco == NULL)
    {
        perror("Erro ao ler disco\n");
        exit(1);
    }

    fread(arquivos, sizeof(arquivo), FILE_AMOUNT, disco);

    /*for(int i = 0; i < FILE_AMOUNT; i++)
    {
        printf("%s.%s\n", arquivos[i].nome, arquivos[i].extensao);
    }*/

    arquivo target;
    char nome[8];
    char extensao[3];

    if(argc > 1)
    {
        sscanf(argv[1], "%7[^.].%3s", nome, extensao);
    }
    
    printf("%s %s\n", nome, extensao);
    for(int i = 0; i < FILE_AMOUNT; i++)
    {
        if(arquivos[i].nome == nome && arquivos[i].extensao == extensao)
        {
            target = arquivos[i];
            break;
        }
    }


    return 0;
}