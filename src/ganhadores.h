#ifndef GANHADORES_H
#define GANHADORES_H

#include <raylib.h>
#include "defines.h"

typedef struct{
	char nome[TAM_MAX_NOME];
	int pontuacao;
} GANHADOR;

bool MenuGanhadores();
int CarregaGanhadores(GANHADOR ganhadores[]);

int PosicaoGanhador(GANHADOR ganhadores[], int pontuacao);
void InsereGanhador(GANHADOR ganhadores[], int posicao, int pontuacao, char nome[]);
bool VerificaGanhador(GANHADOR ganhadores[MAX_GANHADORES], int pontuacao, int *num_letras, char nome[]);

#endif