#ifndef ARVORE_PRIORIDADE_H
#define ARVORE_PRIORIDADE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tad_configs.h" // Para usar o tipo Item e PrioridadeItem
#include "fila.h" // Para usar a Fila e Item

// Estrutura para um NÓ da árvore de prioridades
typedef struct no_arvore {
    int prioridade;
    Fila *fila;
    struct no_arvore *esquerda;
    struct no_arvore *direita;
} NoArvore;

// Estrutura principal que gerencia a árvore
typedef struct {
    NoArvore *raiz;
} ArvorePrioridade;

ArvorePrioridade *arvore_criar(); // Cria uma nova árvore de prioridades
void arvore_inserir(ArvorePrioridade *arvore, Item item_novo); //Insere um novo paciente na árvore de prioridades, de acordo com a prioridade do paciente
void arvore_mostrar(NoArvore *no_arvore); //Mostra a árvore de prioridades, recursivamente.
No* arvore_remover_proximo(ArvorePrioridade *arvore); //Remove e retorna o paciente com a maior prioridade (menor número)
void arvore_limpar(NoArvore* no_arvore); // Limpa as filas de todos os nós da árvore, mas não libera a memória dos nós
void arvore_destruir(NoArvore* no_arvore); //Libera toda a memória alocada pela árvore e seus componentes
void arvore_salvar_em_arquivo(ArvorePrioridade* arvore); // Salva a árvore de prioridades em um arquivo
void arquivo_para_arvore(ArvorePrioridade* arvore); // Carrega a árvore de prioridades a partir de um arquivo
NoArvore* arvore_pegar_no_maior_prioridade(ArvorePrioridade* arvore); // Retorna o nó com a maior prioridade (menor número) da árvore

#endif