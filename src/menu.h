#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include "defines.h"

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
