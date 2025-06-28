#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <stdlib.h>
#include "tad_configs.h"

typedef struct No {
    Item item;
    struct No *proximo;
} No;

typedef struct Fila {
    FILE *arquivo;
    No *primeiro;
    No *ultimo;
    int tamanho;
} Fila;

FILE *abrir_QUEUE_FILE(int w_r); // 0 para escrever ou 1 para leitura
Fila *fila_criar(); // Cria uma nova fila
void fechar_QUEUE_FILE(FILE *arquivo); // Fecha o arquivo da fila
void limpar_fila(Fila *fila); // Limpa a fila, removendo todos os elementos
void fila_para_QUEUE_FILE(Fila *fila); // Salva a fila no arquivo
void QUEUE_FILE_para_fila(Fila *fila); // Carrega a fila PARA O arquivo
void fila_mostrar(Fila *fila); // Mostra os elementos da fila
No *fila_inserir_fim(Fila* fila, Item item); // Insere um item no final da fila
No *fila_remover_inicio(Fila *fila); // Remove e retorna o primeiro item da fila
const char* obter_nome_prioridade(PrioridadeItem num); // Retorna o nome da prioridade como texto
void destruir_fila(Fila *fila); // Libera a memória da fila e seus elementos

#endif