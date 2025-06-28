#include <stdio.h>
#include <windows.h>
#include <unistd.h>
#include <stdlib.h>
#include "../include/tad_configs.h" // Para usar o tipo Item e PrioridadeItem
#include "../include/fila.h" // Para usar a Fila
#include "../include/arvore_prioridade.h" // Para usar a estrutura ArvorePrioridade e NoArvore

void realizar_atendimento(No* paciente_atendido) {
    if (paciente_atendido == NULL) return;

    const char* nome_prioridade = obter_nome_prioridade(paciente_atendido->item.prioridade);
    
    printf("-> Chamando ID: %d (%s), Duração: %d segundos...\n", paciente_atendido->item.id, nome_prioridade, paciente_atendido->item.tempo_processamento);
    
    Sleep(paciente_atendido->item.tempo_processamento * 1000);

    // Libera a memória do nó do paciente
    free(paciente_atendido);
}

void simular(Fila *fila, ArvorePrioridade *arvore) {
    // Garante que a fila em memória está sincronizada com o arquivo
    arvore_limpar(arvore->raiz);
    arquivo_para_arvore(arvore);
    limpar_fila(fila);
    QUEUE_FILE_para_fila(fila);
    // Tenta remover o primeiro item da fila
    No* paciente_atendido = arvore_remover_proximo(arvore);
    if (paciente_atendido != NULL) { // ATENDIMENTO COM PRIORIDADE
        arvore_salvar_em_arquivo(arvore);
    } else { // ATENDIMENTO SEM PRIORIDADE
        paciente_atendido = fila_remover_inicio(fila);
        if (paciente_atendido != NULL) {
            fila_para_QUEUE_FILE(fila);
        }
    }
    NoArvore* no_seguinte = arvore_pegar_no_maior_prioridade(arvore);
    if (no_seguinte != NULL && no_seguinte->fila->primeiro != NULL) { // Ve se proximo no existe e se a fila dentro dele existe também 
        if(no_seguinte->fila->primeiro != NULL){
            printf("Próximo com prioridade: %d\n", no_seguinte->fila->primeiro->item.id);
        }
    } else {
        printf("Próximo com prioridade: Vazio\n");
    }
    if (fila != NULL && fila->primeiro != NULL) { // Verifica se a fila existe e se o primeiro item não é nulo
        printf("Próximo sem prioridade: %d\n", fila->primeiro->item.id);
    } else {
        printf("Próximo sem prioridade: Vazio\n"); 
    }
    realizar_atendimento(paciente_atendido);
}

int main() {
    system("chcp 65001 > NUL"); // ARRUMA FONTE
    
    // CRIA TAD CONFIGS, FILA E ÁRVORE DE PRIORIDADE
    TadConfigs* tad_configs = configs_inicializar();
    Fila* fila = fila_criar();
    ArvorePrioridade* arvore_prioridade = arvore_criar();

    // TRANSFERE OS DADOS DO ARQUIVO PARA A FILA E ÁRVORE
    QUEUE_FILE_para_fila(fila);
    arquivo_para_arvore(arvore_prioridade);

    // Carregar configurações
    configs_ler(tad_configs);
    configs_mostrar(tad_configs);

    while(tad_configs->configs.status != TERMINAR) {
        Sleep(tad_configs->configs.intervalo * 2000);
        if(tad_configs->configs.status == SIMULAR) {
            simular(fila, arvore_prioridade);
        } else{
            printf("Aguardando...\n");
        }
        configs_ler(tad_configs);
    }

    configs_destruir(tad_configs);
    destruir_fila(fila);
    free(fila);
    arvore_destruir(arvore_prioridade->raiz);
    free(arvore_prioridade);

    return 0;
}