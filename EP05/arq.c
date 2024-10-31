/*
Universidade Federal de Itajubá
SRSC02 – Sistemas Operacionais
Gabriel Carneiro Roque Toti Silva - 2023005594
Antony Souza Siqueira - 2022015120
EP05 - Implementação do Sistemas de Arquivos (Exercício 1)
*/

#include <stdio.h>
#include <string.h>


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

void listarSistema(arquivo arquivos[], int count);
void listaCompleto(arquivo arquivos[], int count);
void listarSimples(arquivo arquivos[], int count, int por_linha);

// Função para verificar se o arquivo é deletado
int isDeleted(arquivo *arq) {
    return (unsigned char) arq->nome[0] == 0xEB;
}

// Função para listar arquivos conforme o formato "nome.extensao"
void listarSimples(arquivo arquivos[], int count, int por_linha) {
    int printed = 0;
    for (int i = 0; i < count + 1; i++) {
        if (isDeleted(&arquivos[i]) || arquivos[i].sistema || arquivos[i].hidden) {
            continue;
        }
        printf("%s.%s ", arquivos[i].nome, arquivos[i].extensao);
        printed++;
        if (printed % por_linha == 0) {
            printf("\n");
        }
    }
    if (printed % por_linha != 0) {
        printf("\n");
    }
}

// Função para listar todos os dados dos arquivos
void listaCompleto(arquivo arquivos[], int count) {
    for (int i = 0; i < count; i++) {
        if (isDeleted(&arquivos[i]) || arquivos[i].sistema || arquivos[i].hidden) {
            continue;
        }
        printf("Nome: %s.%s\n", arquivos[i].nome, arquivos[i].extensao);
        printf("Tamanho: %lu bytes\n", arquivos[i].tamanho);
        printf("Criação: %02d/%02d/%04d %02d:%02d:%02d\n", arquivos[i].criacao.dia, arquivos[i].criacao.mes, arquivos[i].criacao.ano, arquivos[i].criacao.hora, arquivos[i].criacao.min, arquivos[i].criacao.seg);
        printf("Acesso: %02d/%02d/%04d %02d:%02d:%02d\n", arquivos[i].acesso.dia, arquivos[i].acesso.mes, arquivos[i].acesso.ano, arquivos[i].acesso.hora, arquivos[i].acesso.min, arquivos[i].acesso.seg);

        printf("\n");
    }
}

// Função para listar arquivos com proteção de sistema

void listarSistema(arquivo arquivos[], int count) {
    int printed = 0;
    for (int i = 0; i < count; i++) {
        if (arquivos[i].sistema && !isDeleted(&arquivos[i])) {
            printf("%s.%s ", arquivos[i].nome, arquivos[i].extensao);
            printed++;
            if (printed % 2 == 0) {
                printf("\n");
            }
        }
    }
    if (printed % 2 != 0) {
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    arquivo arquivos[FILE_AMOUNT];
    int i;
    int qtd = FILE_AMOUNT;
    
    // cria os arquivos a serem lidos
    strcpy(arquivos[0].nome, "arq1\0");
    strcpy(arquivos[0].extensao, "txt\0");
    arquivos[0].sistema = 0;
    arquivos[0].hidden = 0;
    arquivos[0].tamanho = 1024;
    arquivos[0].cluster = 1;

    strcpy(arquivos[1].nome, "arq2\0");
    strcpy(arquivos[1].extensao, "dat\0");
    arquivos[1].sistema = 1;
    arquivos[1].hidden = 0;
    arquivos[1].tamanho = 512;
    arquivos[1].cluster = 3;

    strcpy(arquivos[2].nome, "oculto\0");
    strcpy(arquivos[2].extensao, "bin\0");
    arquivos[2].sistema = 0;
    arquivos[2].hidden = 1;
    arquivos[2].tamanho = 2048;
    arquivos[2].cluster = 4;

    char deletado_nome[8] = {0xEB, 'd', 'e', 'l', '\0', ' ', ' ', ' '};
    strcpy(arquivos[3].nome, deletado_nome);
    strcpy(arquivos[3].extensao, "old\0");
    arquivos[3].sistema = 0;
    arquivos[3].hidden = 0;
    arquivos[3].tamanho = 256;
    arquivos[3].cluster = 8;

    strcpy(arquivos[4].nome, "arq3\0");
    strcpy(arquivos[4].extensao, "txt\0");
    arquivos[4].sistema = 0;
    arquivos[4].hidden = 0;
    arquivos[4].tamanho = 512;
    arquivos[4].cluster = 9;

    strcpy(arquivos[5].nome, "arq4\0");
    strcpy(arquivos[5].extensao, "txt\0");
    arquivos[5].sistema = 0;
    arquivos[5].hidden = 0;
    arquivos[5].tamanho = 512;
    arquivos[5].cluster = 10;

    strcpy(arquivos[6].nome, "arq5\0");
    strcpy(arquivos[6].extensao, "bat\0");
    arquivos[6].sistema = 0;
    arquivos[6].hidden = 0;
    arquivos[6].tamanho = 1024;
    arquivos[6].cluster = 11;

    strcpy(arquivos[7].nome, "arq6\0");
    strcpy(arquivos[7].extensao, "dat\0");
    arquivos[7].sistema = 1;
    arquivos[7].hidden = 0;
    arquivos[7].tamanho = 256;
    arquivos[7].cluster = 13;

    for (i = 0; i < FILE_AMOUNT; i++)
    {
        data dataCriacao = {i+1, 10, 2024, i*2 + 1, 0, 0};
        data dataAcesso = {i*2 + 1, 11, 2024, i+2, 0, 0};
        arquivos[i].criacao = dataCriacao;
        arquivos[i].acesso = dataAcesso;
    }

    if (argc == 1) {
        listarSimples(arquivos, qtd, 2);
    } else if (strcmp(argv[1], "-a") == 0) {
        listaCompleto(arquivos, qtd);
    } else if (strcmp(argv[1], "-s") == 0) {
        listarSistema(arquivos, qtd);
    } else if (strcmp(argv[1], "-w") == 0) {
        listarSimples(arquivos, qtd, 4);
    } else {
        printf("Opção inválida para arq\n");
    }

    return 0;
}