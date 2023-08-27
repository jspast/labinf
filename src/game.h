#ifndef GAME_H
#define GAME_H

#include "main.h"

#define NUM_MAX_ALTERNATIVAS 5
#define TAM_MAX_ALTERNATIVAS 18
#define TAM_MAX_ENUNCIADO 50

#define MAX_PROFESSORES 6

#define MAX_TAMANHO_LABIRINTO 100


typedef struct{
	int x;
	int y;
} POSICAO;

typedef struct{
	int inic_x, fim_x;
	int inic_y, fim_y;
} SAIDA;

typedef struct{
	POSICAO pos;
	int movX;
	int movY;
	double cooldown;
	int vida;
	double tempo_restante;
	int creditos;
	int labirinto;
	int pontuacao;
} JOGADOR;

typedef struct{
	POSICAO pos;
	int movX;
	int movY;
	double cooldown;
	int ativo;
} PROFESSOR;

typedef struct {
	int tamX;
	int tamY;
	POSICAO entrada;
	SAIDA saida;
	int m[MAX_TAMANHO_LABIRINTO][MAX_TAMANHO_LABIRINTO];
} LABIRINTO;

typedef struct{
	double max_tempo;
	int max_professores;
	int min_creditos;
	LABIRINTO labirinto;
} FASE;

typedef struct{
	int fase;
	char dificuldade;
	// ...
} SAVE;

typedef struct{
    char enunciado[TAM_MAX_ENUNCIADO];
    int num_alternativas;
    int alternativa_correta;
    char alternativas[NUM_MAX_ALTERNATIVAS][TAM_MAX_ALTERNATIVAS];
} PERGUNTA;

int NovoJogo(JOGADOR *jogador, FASE *fase, PROFESSOR professores[], int dificuldade);
int CarregaJogo();

int Jogo(int *estado, JOGADOR *jogador, FASE *fase, PROFESSOR professores[]);

void MovimentacaoJogador(JOGADOR *jogador, FASE *fase);
int AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, FASE *fase);
void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador);
void CarregaFase(FASE *fase_atual, int num_fase);


#endif
