#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "ganhadores.h"
#include "defines.h"

// Tela dos ganhadores
bool MenuGanhadores()
{
	bool sair = false;

	if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
		sair = true;

	GANHADOR ganhadores[MAX_GANHADORES];

	int num_ganhadores = CarregaGanhadores(ganhadores);

	int i;
	for(i = 0; i < num_ganhadores; i++){
		DrawText(TextFormat("%d", ganhadores[i].pontuacao),
			50,
			(RES_Y - (MAX_GANHADORES * FONTE_OPCOES + (MAX_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
			FONTE_OPCOES, LIGHTGRAY);
		DrawText(ganhadores[i].nome,
			200,
			(RES_Y - (MAX_GANHADORES * FONTE_OPCOES + (MAX_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
			FONTE_OPCOES, LIGHTGRAY);
	}
	return sair;
}

int CarregaGanhadores(GANHADOR ganhadores[])
{
	int i = 0;

	FILE *arq;
	arq = fopen("ganhadores.dat", "rb");
	if(arq != NULL) {
		while(!feof(arq)){
			fread(&ganhadores[i], sizeof(GANHADOR), 1, arq);
			i++;
		}
		fclose(arq);
	}

	return i - 1;
}

void SalvaGanhadores(GANHADOR ganhadores[], int num_ganhadores)
{
	FILE *arq;
	arq = fopen("ganhadores.dat", "wb");
	if(arq != NULL) {
		fwrite(ganhadores, sizeof(GANHADOR), num_ganhadores, arq);
		fclose(arq);
	}
}

int PosicaoGanhador(GANHADOR ganhadores[], int pontuacao)
{
	int i = CarregaGanhadores(ganhadores);

	while(i > 0 && pontuacao > ganhadores[i - 1].pontuacao)
		i--;

	return i;
}

void InsereGanhador(GANHADOR ganhadores[], int posicao, int pontuacao, char nome[])
{
	int i = CarregaGanhadores(ganhadores);

	if(i > MAX_GANHADORES)
		i = MAX_GANHADORES;

	while(i > posicao){
		strcpy(ganhadores[i].nome, ganhadores[i-1].nome);
		ganhadores[i].pontuacao = ganhadores[i-1].pontuacao;
		i--;
	}

	strcpy(ganhadores[posicao].nome, nome);
	ganhadores[posicao].pontuacao = pontuacao;
}

bool VerificaGanhador(GANHADOR ganhadores[MAX_GANHADORES], int pontuacao, int *num_letras, char nome[])
{
	int num_ganhadores = CarregaGanhadores(ganhadores);
	int posicao = PosicaoGanhador(ganhadores, pontuacao);

	if (posicao < MAX_GANHADORES){
		
		int tecla = 0;

		DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);
		DrawText("Digite seu nome:", RES_X/2 - 100, RES_Y/2 - 50, FONTE_OPCOES, LIGHTGRAY);
		DrawText(nome, RES_X/2 - 100, RES_Y/2, FONTE_OPCOES, LIGHTGRAY);

		tecla = GetCharPressed();

		while (tecla > 0) {

			// Tecla de caractere pressionada
			if ((tecla >= 32) && (tecla <= 125) && (*num_letras < TAM_MAX_NOME))
			{
				nome[*num_letras] = (char)tecla;
				(*num_letras)++;
				nome[*num_letras] = '\0';
			}

			tecla = GetCharPressed();
		}

		// Tecla backspace pressionada
		if ((IsKeyPressed(KEY_BACKSPACE)) && (num_letras > 0)) 
		{
			(*num_letras)--;
			nome[*num_letras] = '\0';
		}

		if (IsKeyPressed(KEY_ENTER) && (*num_letras < TAM_MAX_NOME)) {
			InsereGanhador(ganhadores, posicao, pontuacao, nome);
			SalvaGanhadores(ganhadores, num_ganhadores+1);
			return true;
		}
	}

	return false;
}