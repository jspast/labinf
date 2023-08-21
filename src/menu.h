#ifndef MENU_H
#define MENU_H

#define TAM_MAX_OPCOES 18

typedef struct{
	char nome[50];
	int pontuacao;
} GANHADOR;

int Menu();
int MenuGanhadores();
int MenuInformacoes();
int MenuNovoJogo();

int Selecao(int *opcao_selecionada, int num_opcoes);
void DesenhaOpcoes(int opcao_selecionada, int num_opcoes, char opcoes[][TAM_MAX_OPCOES]);

#endif
