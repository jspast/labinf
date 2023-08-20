#ifndef MENU_H
#define MENU_H

typedef struct{
	char nome[50];
	int pontuacao;
} GANHADOR;

int Menu();
int MenuGanhadores();
int MenuInformacoes();
int MenuNovoJogo();

#endif
