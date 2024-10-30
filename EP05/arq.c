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

// Função para verificar se o arquivo é deletado
int is_deleted(arquivo *arq) {
    return arq->nome[0] == 0xEB;
}

// Função para listar arquivos conforme o formato "nome.extensao"
void listar_simples(arquivo arquivos[], int count, int por_linha) {
    int printed = 0;
    for (int i = 0; i < count + 1; i++) {
        if (is_deleted(&arquivos[i]) || arquivos[i].sistema || arquivos[i].hidden) {
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
void listar_completo(arquivo arquivos[], int count) {
    for (int i = 0; i < count; i++) {
        if (is_deleted(&arquivos[i]) || arquivos[i].sistema || arquivos[i].hidden) {
            continue;
        }
        printf("Nome: %s.%s\n", arquivos[i].nome, arquivos[i].extensao);
        printf("Tamanho: %lu bytes\n", arquivos[i].tamanho);
        printf("Criação: %d/%d/%d %d:%d:%d\n", arquivos[i].criacao.dia, arquivos[i].criacao.mes, arquivos[i].criacao.ano, arquivos[i].criacao.hora, arquivos[i].criacao.min, arquivos[i].criacao.seg);
        printf("Acesso: %d/%d/%d %d:%d:%d\n", arquivos[i].acesso.dia, arquivos[i].acesso.mes, arquivos[i].acesso.ano, arquivos[i].acesso.hora, arquivos[i].acesso.min, arquivos[i].acesso.seg);
        // Imprimir outras informações se necessário
        printf("\n");
    }
}

// Função para listar arquivos com proteção de sistema
void listar_sistema(arquivo arquivos[], int count) {
    int printed = 0;
    for (int i = 0; i < count; i++) {
        if (arquivos[i].sistema && !is_deleted(&arquivos[i])) {
            printf("%-12s.%s ", arquivos[i].nome, arquivos[i].extensao);
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
    int qtd = 3;
    

    strcpy(arquivos[0].nome, "arquivo1");
    strcpy(arquivos[0].extensao, "txt");
    arquivos[0].sistema = 0;
    arquivos[0].hidden = 0;
    arquivos[0].tamanho = 1024;
    arquivos[0].cluster = 1;

    strcpy(arquivos[1].nome, "arquivo2");
    strcpy(arquivos[1].extensao, "dat");
    arquivos[1].sistema = 1;
    arquivos[1].hidden = 0;
    arquivos[1].tamanho = 512;
    arquivos[1].cluster = 2;

    strcpy(arquivos[2].nome, "oculto");
    strcpy(arquivos[2].extensao, "bin");
    arquivos[2].sistema = 0;
    arquivos[2].hidden = 1;
    arquivos[2].tamanho = 2048;
    arquivos[2].cluster = 3;

    char deletado_nome[8] = {0xEB, 'd', 'e', 'l', '\0', ' ', ' ', ' '};
    strcpy(arquivos[3].nome, deletado_nome);
    strcpy(arquivos[3].extensao, "old");
    arquivos[3].sistema = 0;
    arquivos[3].hidden = 0;
    arquivos[3].tamanho = 256;
    arquivos[3].cluster = 4;

    for (i = 0; i < FILE_AMOUNT; i++)
    {
        data dataCriacao = {i+1, 10, 2024, i*2 + 1, 0, 0};
        data dataAcesso = {i*2 + 1, 11, 2024, i+2, 0, 0};
        arquivos[i].criacao = dataCriacao;
        arquivos[i].acesso = dataAcesso;
    }

    if (argc == 1) {
        listar_simples(arquivos, qtd, 2);
    } else if (strcmp(argv[1], "-a") == 0) {
        listar_completo(arquivos, qtd);
    } else if (strcmp(argv[1], "-s") == 0) {
        listar_sistema(arquivos, qtd);
    } else if (strcmp(argv[1], "-w") == 0) {
        listar_simples(arquivos, qtd, 4);
    } else {
        printf("Opção inválida para arq\n");
    }

    return 0;
}