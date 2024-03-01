#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include "defines.h"
#include <math.h>

typedef enum{
	MENU = 0,
	GANHADORES = 1,
	INFORMACOES = 2,
	NOVO_JOGO = 3,
	CARREGA_JOGO = 4, 
	JOGO = 5,
	ERRO = -2,
	FIM = -1
} ESTADO;

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
	int bombas;
	int total_creditos;
	double tempo_total;
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
} SAVE;

typedef struct{
	char enunciado[TAM_MAX_ENUNCIADO];
	int num_alternativas;
	int alternativa_correta;
	char alternativas[NUM_MAX_ALTERNATIVAS][TAM_MAX_ALTERNATIVAS];
} PERGUNTA;

bool NovoJogo(JOGADOR *jogador, FASE *fase, PROFESSOR professores[], int dificuldade, SAVE *jogo_atual);
bool CarregaJogo(JOGADOR *jogador, FASE *fase, PROFESSOR professores[], SAVE *jogo_atual);

ESTADO Jogo(int *estado, JOGADOR *jogador, FASE *fase, PROFESSOR professores[], PERGUNTA perguntas[], int num_perguntas, Texture2D texturas[], SAVE *jogo_atual);

bool MovimentacaoJogador(JOGADOR *jogador, FASE *fase, bool *passar_fase);
void JogarBomba(JOGADOR *jogador, FASE *fase);
bool AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, FASE *fase);

bool CarregaFase(FASE *fase_atual, int num_fase);

int CarregaPerguntas(PERGUNTA perguntas[]);
int Pergunta(PERGUNTA perguntas[], int num_perguntas, int *alt_selecionada);

#endif

