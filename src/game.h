#ifndef GAME_H
#define GAME_H

#include "main.h"

#define NUM_MAX_ALTERNATIVAS 5
#define TAM_MAX_ALTERNATIVAS 18
#define TAM_MAX_ENUNCIADO 50

#define MAX_PROFESSORES 6

#define MAX_TAMANHO_LABIRINTO 200


typedef struct{
	int x;
	int y;
} POSICAO;

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

typedef struct{
	int tamX;
	int tamY;
	POSICAO entrada;
	POSICAO saida;
	char matriz[MAX_TAMANHO_LABIRINTO][MAX_TAMANHO_LABIRINTO];
} LABIRINTO;

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

int NovoJogo(JOGADOR *jogador, LABIRINTO *labirinto, PROFESSOR professores[], int dificulade);
int CarregaJogo();

int Jogo(int *estado, JOGADOR *jogador, LABIRINTO *labirinto, PROFESSOR professores[]);

void MovimentacaoJogador(JOGADOR *jogador, LABIRINTO *labirinto);
int AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, LABIRINTO *labirinto);
void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador);



#endif
