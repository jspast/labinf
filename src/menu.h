#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include "defines.h"

int MenuInicial(int *opcao_selecionada, Sound sons[]);
bool MenuInformacoes(Texture2D texturas[]);
int MenuNovoJogo(int *opcao_selecionada, Sound sons[]);

int Selecao(int *opcao_selecionada, int num_opcoes, Sound sons[]);
void DesenhaSelecao(int opcao_selecionada, int num_opcoes, char opcoes[][TAM_MAX_OPCOES]);

#endif
