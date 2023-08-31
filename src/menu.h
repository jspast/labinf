#ifndef MENU_H
#define MENU_H

#include "main.h"

#define COR_TITULO WHITE
#define COR_OPCOES GRAY
#define COR_OPCAO_SELECIONADA LIGHTGRAY

#define FONTE_TITULO 30
#define FONTE_OPCOES 20
#define ESPACAMENTO (FONTE_OPCOES/2)
#define ESPACAMENTO_INFORMACOES 32

#define TAM_MAX_OPCOES 50
#define NUM_OPCOES 5
#define NUM_INFORMACOES 7
#define NUM_GANHADORES 10
#define NUM_DIFICULDADES 3

typedef struct{
	char nome[50];
	int pontuacao;
} GANHADOR;

int MenuInicial(int *opcao_selecionada);
bool MenuGanhadores();
bool MenuInformacoes(Texture2D texturas[]);
int MenuNovoJogo(int *opcao_selecionada);

int Selecao(int *opcao_selecionada, int num_opcoes);
void DesenhaSelecao(int opcao_selecionada, int num_opcoes, char opcoes[][TAM_MAX_OPCOES]);

#endif
