#include <stdio.h> // PRINTF
#include <unistd.h> //WIRTE READ ARQUIVOS
#include <stdlib.h>
#include <time.h> // BIBLIOTECA PRO TIME RANDOMIZER
#include "../include/tad_configs.h" // INCLUI O HEADER
#include "../include/fila.h" // INCLUI O HEADER DA FILA
#include "../include/arvore_prioridade.h" // INCLUI O HEADER DA ÁRVORE DE PRIORIDADE

int menu() {
    printf("1. Aguardar\n2. Simular\n3. Terminar\n4. Ler\n5. Criar Ficha\n0. Sair\n");
    printf("Informe a opção desejada: ");

    int aux;
    // VERIFICA SE FOI BEM SUCEDIDA (não tem lixo e.g "ABC")
    if (scanf("%d", &aux) != 1) { 
        aux = -1; // cair no default do switch
    }
    
    // Limpa buffer,
    while (getchar() != '\n'); 
    printf("\n");
    return aux;
}

int main() {
    system("chcp 65001 > NUL"); // ARRUMAR FONTE
    srand(time(NULL)); // INICIA O RANDOMIZER

    // CRIA TAD CONFIGS, FILA E ÁRVORE DE PRIORIDADE
    TadConfigs* tad_configs = configs_inicializar(); 
    Fila* fila = fila_criar();
    ArvorePrioridade* arvore_prioridade = arvore_criar();
    Especialidades* especialidades = especialidades_inicializar(); // INICIALIZA AS ESPECIALIDADES

    // TRANSFERE OS DADOS DO ARQUIVO PARA A FILA E ÁRVORE
    QUEUE_FILE_para_fila(fila);
    arquivo_para_arvore(arvore_prioridade);

    int op; // Variável para armazenar a opção do menu
    if (!tad_configs) {
        printf("Erro ao criar TAD\n");
        return 1;
    }
    do {
        op = menu();
        switch(op) {
            case 1: {
                configs_atualizar(tad_configs, AGUARDAR, 1);
                break;
            }
            case 2: {
                configs_atualizar(tad_configs, SIMULAR, 1);
                break;
            }
            case 3: {
                configs_atualizar(tad_configs, TERMINAR, 1);
                break;
            }
            case 4: {
                // TAD CONFIGS
                configs_ler(tad_configs);
                //TAD CONFIGS
                
                // FILA
                limpar_fila(fila);
                QUEUE_FILE_para_fila(fila);
                // FILA

                // ARVORE
                arvore_limpar(arvore_prioridade->raiz);
                arquivo_para_arvore(arvore_prioridade);
                // ARVORE

                //PRINTS
                configs_mostrar(tad_configs);
                mostrar_especialidades(especialidades);

                printf("\n--- PACIENTES PRIORITÁRIOS ---");
                printf("\n");
                arvore_mostrar(arvore_prioridade->raiz); // Usa recursão com argumento tipo NoArvore

                printf("--- PACIENTES NA FILA COMUM ---\n");

                fila_mostrar(fila);

                printf("\n");
                // PRINTS

                break;
            }
            case 5: {

                // PREPARA LISTA
                limpar_fila(fila);
                QUEUE_FILE_para_fila(fila);
                // PREPARA LISTA

                // PREPARA ÁRVORE
                arvore_limpar(arvore_prioridade->raiz);
                arquivo_para_arvore(arvore_prioridade);
                // PREPARA ÁRVORE
                
                //Cria um item
                Item item;
                item.id = rand() % 10000;
                item.tempo_processamento = (rand() % 5) + 1;

                //Pergunta prioridade e salva no item.prioridade o tipo enum de prioridade
                int prioridade_lida;
                // Pergunta especialidade
                int especialidade_lida;

                while (1) { // Loop infinito para garantir que o usuário digite uma opção válida
                    printf("\nDigite a especialidade requerida (1-4):\n");
                    printf("1. Cardiologista\n2. Oftalmologista\n3. Alergia e Imunologia\n4. Dermatologia\n"); // Corrigido o "Dermatologian"
                    printf("Opção: ");

                    // PASSO 1: LER A ENTRADA DO USUÁRIO
                    if (scanf("%d", &especialidade_lida) == 1 && especialidade_lida >= 1 && especialidade_lida <= 4) {
                        switch (especialidade_lida) {
                            case 1:
                                especialidades->cardiologista++;
                                printf("\n-> Especialidade Cardiologista adicionada.\n");
                                break; // Sai do switch
                            case 2:
                                especialidades->oftalmologista++;
                                printf("\n-> Especialidade Oftalmologista adicionada.\n");
                                break; // Sai do switch
                            case 3:
                                especialidades->alergia_imunologia++;
                                printf("\n-> Especialidade Alergia e Imunologia adicionada.\n");
                                break; // Sai do switch
                            case 4:
                                especialidades->dermatologia++;
                                printf("\n-> Especialidade Dermatologia adicionada.\n");
                                break; // Sai do switch
                            default:
                                printf("\n--- ERRO: Opção inválida. A especialidade deve ser entre 1 e 4. Tente novamente. ---\n");
                        }
                        break; // Sai do loop se a entrada for válida
                    } else {
                        printf("\n--- ERRO: Entrada inválida. Por favor, digite um NÚMERO CONDIZENTE. Tente novamente. ---\n"); // A entrada não era um número (ex: "abc").
                    }
                    while (getchar() != '\n'); // Limpa o buffer de entrada para evitar loops infinitos.
                }

                while (1) { // Cria um loop infinito que só será interrompido com 'break'

                    printf("Digite a prioridade do paciente (1-6):\n");
                    printf("1. Gestante\n2. Idoso\n3. Pessoas com necessidades especiais\n4. Criança de colo\n5. Doença crônica\n6. Demais pacientes\n");
                    printf("Opção: ");

                    if (scanf("%d", &prioridade_lida) == 1) { // SE O SCANF RETORNAR 1, FOI BEM SUCEDIDO

                        if (prioridade_lida >= 1 && prioridade_lida <= 6) { // Se conseguiu ler um número, verifica se está no intervalo válido.
                            
                            item.prioridade = (PrioridadeItem)prioridade_lida; // Converte o número lido para o tipo enum PrioridadeItem 

                            if (item.prioridade <= 5) { // PACIENTES COM PRIORIDADE

                                arvore_inserir(arvore_prioridade, item); // INSERE NA ÁRVORE DE PRIORIDADE
                                arvore_salvar_em_arquivo(arvore_prioridade); // SALVA A ÁRVORE NO ARQUIVO
                                printf("\n");
                                printf("-> Paciente com ID %d adicionado à fila prioritária.\n", item.id);
                                printf("\n");

                                
                            } else { // DEMAIS_PACIENTES (prioridade 6)

                                fila_inserir_fim(fila, item);// INSERE NA FILA
                                fila_para_QUEUE_FILE(fila); // fila -> QUEUE FILE

                                printf("\n");
                                printf("-> Paciente com ID %d adicionado à fila comum.\n", item.id);
                                printf("\n");

                            }
                            break;

                        } else {
                            printf("\n--- ERRO: Opção inválida. A prioridade deve ser entre 1 e 6. Tente novamente. ---\n"); // O usuário digitou um número, mas fora do intervalo (ex: 8 ou -1).
                        }
                    } else {
                        printf("\n--- ERRO: Entrada inválida. Por favor, digite um NÚMERO. Tente novamente. ---\n"); // A entrada não era um número (ex: "abc").
                    }
                    while (getchar() != '\n'); // Limpa o buffer de entrada para evitar loops infinitos.
                } // O loop só termina quando a entrada for válida.
                break;
            }
            case 0: {
                configs_destruir(tad_configs);
                destruir_fila(fila);
                free(fila);
                arvore_destruir(arvore_prioridade->raiz);
                free(arvore_prioridade);
                printf("Até a próxima!\n");
                break;
            }
            default: {
                printf("Opção inválida!\n");
            }
        }
    } while (op != 0);
    return 0;
}