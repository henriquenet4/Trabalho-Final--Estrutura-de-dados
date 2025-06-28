#ifndef TAD_CONFIGS_H
#define TAD_CONFIGS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    AGUARDAR,
    SIMULAR,
    TERMINAR,
    SITUACAO
} statusProcessamento;

typedef struct especialidades { // STRUCT NOVA PARA AS ESPECIALIDADES
    int cardiologista;
    int oftalmologista;
    int alergia_imunologia;
    int dermatologia;
} Especialidades;

typedef struct conf {
    statusProcessamento status;
    int intervalo;
} Configs;

typedef struct tad_configs {
  FILE *arquivo;
  Configs configs;
} TadConfigs;

typedef enum {
    GESTANTE = 1,
    IDOSO = 2,
    NECESSIDADES_ESPECIAIS = 3,
    CRIANCA_COLO = 4,
    DOENCA_CRONICA = 5,
    DEMAIS_PACIENTES = 6
} PrioridadeItem;

typedef struct item {
    int id;
    int tempo_processamento;
    PrioridadeItem prioridade;
} Item;

FILE *configs_abrir(); // Abre o arquivo de configurações, criando-o se não existir
TadConfigs *configs_inicializar(); // Inicializa o TAD e abre o arquivo de configurações
void configs_fechar(FILE *arquivo); // Fecha o arquivo de configurações
void configs_destruir(TadConfigs *tad); // Destroi o TAD e fecha o arquivo
void configs_salvar(TadConfigs *tad); // Salva as configurações no arquivo
void configs_ler(TadConfigs *tad); // Lê as configurações do arquivo
void configs_mostrar(TadConfigs *tad); // Mostra as configurações atuais
void configs_atualizar(TadConfigs *tad, statusProcessamento status, int intervalo); // Atualiza o status e intervalo das configurações
void mostrar_especialidades(Especialidades* especialidades); // Mostra as especialidades e seus valores
Especialidades* especialidades_inicializar(); // Inicializa as especialidades com valores padrão (0)
#endif