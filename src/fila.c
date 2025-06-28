#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // LEITURA E ESCRITA
#include "../include/tad_configs.h" // IMPORTAR HEADER
#include "../include/fila.h" // IMPORTAR HEADER DA FILA
#define QUEUE_FILE "../data/fila.dat" // CRIA UM ARQUIVO PARA ARMAZENAR A FILA

Fila *fila_criar() {
    Fila *fila = malloc(sizeof(Fila));
    if (!fila) {
        printf("Falha ao criar a fila!\n");
        return NULL;
    }
	fila->tamanho = 0;
	fila->primeiro = NULL;
	fila->ultimo = NULL;
    return fila;
}

No *fila_inserir_fim(Fila* fila, Item item){
	if (!fila){
		printf("Erro ao inserir na fila!\n");
		return NULL;
	}
	No *novo = (No *)malloc(sizeof(No));
	if (!novo){
		printf("Falha ao criar o Nó!\n");
		return NULL;
	}
	novo->item = item;
	novo->proximo = NULL;
	fila->tamanho++;
	if (fila->primeiro == NULL){
		fila->primeiro = novo;
	}
	else {
		fila->ultimo->proximo = novo;
	}
	fila->ultimo = novo;
	return novo;
}

No *fila_remover_inicio(Fila *fila){
	if (fila->primeiro == NULL){
		return NULL;
	}
	No* removido = fila->primeiro;
	fila->tamanho--;
	if (removido->proximo == NULL){
		fila->primeiro = NULL;
		fila->ultimo = NULL;
		return removido;
	}
	fila->primeiro = removido->proximo;
	return removido;
}

FILE* abrir_QUEUE_FILE(int w_r) { // 0 para escrever ou 1 para leitura
    FILE *arquivo;
    if( access(QUEUE_FILE, F_OK ) != -1 && w_r) {
        arquivo = fopen(QUEUE_FILE, "rb+"); // ABRE OU CRIA ARQUIVO PARA ESCREVER (FILA->ARQUIVO)
    } else {
        arquivo = fopen(QUEUE_FILE, "wb+"); // ABRE OU CRIA ARQUIVO PARA LER (ARQUIVO->FILA)
    }
    return arquivo;
}

void fechar_QUEUE_FILE(FILE *arquivo) {
    fclose(arquivo);
}

void limpar_fila(Fila *fila) { // EXCLUI TUDO DA FILA
  if (fila) {
    while(fila->tamanho > 0){
      free(fila_remover_inicio(fila));
    }
  }
}

void destruir_fila(Fila *fila) { // EXCLUI TUDO DA FILA E DÁ FREE NELA
    if (fila) {
        limpar_fila(fila); // LIMPA A FILA
        free(fila); // LIBERA MEMÓRIA DA FILA
    }
}

void fila_para_QUEUE_FILE(Fila *fila) {
    FILE *arquivo = abrir_QUEUE_FILE(0); // ABRE PARA ESCREVER
    if (fila && arquivo) {
        No *atual = fila->primeiro;
        while (atual != NULL) {
            fwrite(&atual->item, sizeof(Item), 1, arquivo);
            atual = atual->proximo;
        }
    }
    else {
        printf("Erro ao salvar fila no arquivo.\n");
    }
    fechar_QUEUE_FILE(arquivo);
}

void QUEUE_FILE_para_fila(Fila *fila) {
  limpar_fila(fila); // LIMPA FILA
  FILE *arquivo = abrir_QUEUE_FILE(1); // ABRE PARA LEITURA O QUEUE FILE
  if (fila && arquivo) {
	  Item item;
    // DO INICIO AO FIM - DAT LÊ NESSA ORDEM
	  while (fread(&item, sizeof(Item), 1, arquivo) == 1) {
		  fila_inserir_fim(fila, item); //Insere na lista os itens
	  }
  }
  fechar_QUEUE_FILE(arquivo); // FECHA O QUEUE FILE
}

// Função auxiliar para obter o nome da prioridade como texto
const char* obter_nome_prioridade(PrioridadeItem num) {
    switch (num) {
        case GESTANTE: // Supondo que você tenha esses nomes no seu enum
            return "Gestante";
        case IDOSO:
            return "Idoso";
        case NECESSIDADES_ESPECIAIS:
            return "P. com necessidades especiais";
        case CRIANCA_COLO:
            return "Criança de colo";
        case DOENCA_CRONICA:
            return "Doença crônica";
        case DEMAIS_PACIENTES:
            return "Demais pacientes";
        default:
            return "Prioridade desconhecida";
    }
}
void fila_mostrar(Fila *fila) {
    if (fila) {
        if (fila->tamanho > 0) {
            No *atual = fila->primeiro;
            
            // Itera pela fila para mostrar cada item
            for (int i = 0; i < fila->tamanho; i++) {
                
                // Pega o nome da prioridade usando a função auxiliar
                const char* nome_prioridade = obter_nome_prioridade(atual->item.prioridade);

                // Imprime no formato "ID (Nome da Prioridade)"
                printf("  - ID: %d (%s)\n", atual->item.id, nome_prioridade);

                // Avança para o próximo nó da fila
                atual = atual->proximo;
            }
            
            printf("---------------------------\n");
            printf("Tamanho total da Fila de %s = %d\n", obter_nome_prioridade(fila->primeiro->item.prioridade), fila->tamanho); // PRINTA TAMANHO DA FILA
            printf("\n");
        }
    }
}