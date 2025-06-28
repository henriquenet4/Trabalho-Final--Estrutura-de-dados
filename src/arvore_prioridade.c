#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/fila.h"  // Para usar a Fila
#include "../include/tad_configs.h" // Para usar o tipo Item e PrioridadeItem
#include "../include/arvore_prioridade.h" // Para usar a estrutura ArvorePrioridade e NoArvore
#define ARVORE_FILE "../data/arvore.dat" // Cria um arquivo para armazenar a árvore

ArvorePrioridade* arvore_criar() {
    ArvorePrioridade* nova_arvore = (ArvorePrioridade*) malloc(sizeof(ArvorePrioridade));
    if (nova_arvore != NULL) {
        nova_arvore->raiz = NULL;
    } else {
        printf("ERRO CRÍTICO: Falha ao alocar memória para a árvore!\n");
    }
    return nova_arvore;
}

void arvore_inserir(ArvorePrioridade* arvore, Item item_novo) {
    if(item_novo.prioridade < 1 || item_novo.prioridade > 5) {
        return;
    }
    if (arvore == NULL) {
        printf("ERRO: A árvore não foi criada!\n");
        return;
    }
    NoArvore* no_arvore = (NoArvore*) malloc(sizeof(NoArvore));
    no_arvore->prioridade = item_novo.prioridade;
    no_arvore->fila = fila_criar();
    fila_inserir_fim(no_arvore->fila, item_novo);
    no_arvore->esquerda = NULL;
    no_arvore->direita = NULL;

    if (arvore->raiz == NULL) {
        arvore->raiz = no_arvore; // O novo nó vira a raiz
        return;
    }

    NoArvore* no_arvore_atual = arvore->raiz;
    while (1) {

        // CASO 1: O nó para esta prioridade JÁ EXISTE
        if (item_novo.prioridade == no_arvore_atual->prioridade) {
            // Insere o item no final da fila que já existe
            fila_inserir_fim(no_arvore_atual->fila, item_novo);
            
            // nao usamos o no_arvore criado, precisamos liberar a memória dele
            limpar_fila(no_arvore->fila);
            free(no_arvore);
            
            break;
        } else if (item_novo.prioridade < no_arvore_atual->prioridade) {
            
            // Se o caminho da esquerda está livre
            if (no_arvore_atual->esquerda == NULL) {
                // colocamos o novo nó aqui.
                no_arvore_atual->esquerda = no_arvore;
                break;
            } else {
                // Se não está livre, continuamos a caminhada para a esquerda
                no_arvore_atual = no_arvore_atual->esquerda;
            }
        } else { // (item_novo.prioridade > no_atual->prioridade)
            
            // Se o caminho da direita está livre
            if (no_arvore_atual->direita == NULL) {
                // colocamos o novo nó aqui
                no_arvore_atual->direita = no_arvore;
                break;
            } else {
                // Se não está livre, continuamos a caminhada para a direita
                no_arvore_atual = no_arvore_atual->direita;
            }
        }
    }
}

void arvore_mostrar(NoArvore* no_arvore) {
    if (no_arvore == NULL) return NULL;
    if (no_arvore != NULL) {
        arvore_mostrar(no_arvore->esquerda);
        fila_mostrar(no_arvore->fila); // Mostra a fila associada ao nó atual
        arvore_mostrar(no_arvore->direita);
    }
}

No* arvore_remover_proximo(ArvorePrioridade* arvore) {
    if (arvore == NULL || arvore->raiz == NULL) {
        return NULL;
    }
    //// Encontrar o nó de maior prioridade e o pai dele
    NoArvore* pai_do_alvo = NULL;
    NoArvore* no_alvo = arvore->raiz;

    // Laço para caminhar para a esquerda até o fim. Ao final, no_alvo é o nó com a maior prioridade
    while (no_alvo->esquerda != NULL) {
        pai_do_alvo = no_alvo;
        no_alvo = no_alvo->esquerda;
    }
    //// Remover o primeiro paciente da fila deste nó.
    No* paciente_atendido = fila_remover_inicio(no_alvo->fila);

    //// Se a fila do nó ficou vazia, removemos o nó da árvore.
    if (no_alvo->fila->tamanho == 0) {
        // Se o pai é NULL, então no_alvo é a raiz da árvore.
        if (pai_do_alvo == NULL) {
            // A nova raiz da árvore será o filho da direita do nó removido.
            arvore->raiz = no_alvo->direita;
        } 
        // Se o pai existe, o nó a ser removido era um filho esquerdo.
        else {
            pai_do_alvo->esquerda = no_alvo->direita;
        }

        destruir_fila(no_alvo->fila); // Libera a fila vazia de dentro do nó
        free(no_alvo); // Libera o nó
    }
    return paciente_atendido;
}

void arvore_limpar(NoArvore* no_arvore) {
    if (no_arvore == NULL) {
        return;
    }
    // destrói a sub-arvore da esquerda
    arvore_limpar(no_arvore->esquerda);
    // destrói a sub-arvore da direita
    arvore_limpar(no_arvore->direita);
    // libera a fila de pacientes que está dentro do nó
    limpar_fila(no_arvore->fila);
}

void arvore_destruir(NoArvore* no_arvore) {
    if (no_arvore == NULL) {
        return;
    }
    // MESMA COISA DO LIMPAR, SÓ QUE AQUI LIBERAMOS A MEMÓRIA DO NÓ
    arvore_destruir(no_arvore->esquerda);
    arvore_destruir(no_arvore->direita);
    destruir_fila(no_arvore->fila);
    free(no_arvore);
}

FILE* abrir_ARVORE_FILE(int w_r) { // 0 para escrever ou 1 para leitura
    FILE *arquivo;
    if( access(ARVORE_FILE, F_OK ) != -1 && w_r) {
        arquivo = fopen(ARVORE_FILE, "rb+"); // READ (1)
    } else {
        arquivo = fopen(ARVORE_FILE, "wb+"); // WRITE (0)
    }
    return arquivo;
}

void fechar_ARVORE_FILE(FILE *arquivo) {
    if (arquivo == NULL) {
        return;
    }
    fclose(arquivo);
}

// É NECESSÁRIO TER DUAS FUNÇÕES DE SALVAR, PORQUE SE FOSSE SÓ UMA, A FUNÇÃO SEMPRE ABRIRIA O ARQUIVO PARA ESCRITA.
// POR CAUSA DA RECURSÃO, APAGANDO SEMPRE O QUE FOI ESCRITO ANTES.
void salvar(NoArvore* no, FILE* arquivo) {
    if (no == NULL) {
        return;
    }    
    // 1. Vai para a esquerda
    salvar(no->esquerda, arquivo);

    // Escreve todos os itens da fila do nó no arquivo
    No* paciente_atual = no->fila->primeiro;
    while (paciente_atual != NULL) {
        fwrite(&paciente_atual->item, sizeof(Item), 1, arquivo);
        paciente_atual = paciente_atual->proximo;
    }

    // 3. Vai para a direita
    salvar(no->direita, arquivo);
}

void arvore_salvar_em_arquivo(ArvorePrioridade* arvore) {
    if (arvore == NULL) { 
        return;
    }
    FILE *arquivo = abrir_ARVORE_FILE(0); // Abre para write

    if (arquivo == NULL) {
        return;
    }
    salvar(arvore->raiz, arquivo);
    fechar_ARVORE_FILE(arquivo);
}

void arquivo_para_arvore(ArvorePrioridade* arvore) {
    
  arvore_limpar(arvore->raiz); // LIMPA ARVORE ANTES DE CARREGAR
  FILE *arquivo = abrir_ARVORE_FILE(1); // ABRE PARA LEITURA

  if (arvore && arquivo) {
	  Item item;
    // DO INICIO AO FIM - DAT LÊ NESSA ORDEM
	  while (fread(&item, sizeof(Item), 1, arquivo) == 1) {
		  arvore_inserir(arvore, item); //Insere na lista os itens
	  }
  }
  fechar_ARVORE_FILE(arquivo); // FECHA O QUEUE FILE
}

NoArvore* arvore_pegar_no_maior_prioridade(ArvorePrioridade* arvore) {
    if (arvore == NULL || arvore->raiz == NULL) {
        return NULL;
    }
    NoArvore* atual = arvore->raiz;

    // Laço para caminhar o mais à esquerda possível.
    while (atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}