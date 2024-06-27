#include "jogo.h"
#include "tipos.h"
#include "constantes.h"
#include <raylib.h>
#include <stdio.h>

void CarregaColegas(COLEGA colegas[])
{
	if(FileExists(A_COLEGAS)){
		FILE *arq;
		arq = fopen(A_COLEGAS, "rb");
		fread(colegas, sizeof(COLEGA), MAX_COLEGAS, arq);
		fclose(arq);
	}
}

void SalvaColegas(COLEGA colegas[])
{
	FILE *arq;
	arq = fopen(A_COLEGAS, "wb");
	if(arq != NULL)
		fwrite(colegas, sizeof(COLEGA), MAX_COLEGAS, arq);
}

// Recompensa o jogador por salvar o colega
void RecompensaColega(JOGADOR j, FASE *fase){

	POSICAO pos;
	pos.x = j.pos.x + j.mov.x;
	pos.y = j.pos.y + j.mov.y;

	switch(GetRandomValue(0, 2)){
        // Vida
		case 0:
			fase->labirinto.m[pos.x][pos.y] = M_CORACAO;
			break;
        // Tempo
		case 1:
			fase->labirinto.m[pos.x][pos.y] = M_RELOGIO;
			break;
        // Bomba
		case 2:
			fase->labirinto.m[pos.x][pos.y] = M_BOMBA_INATIVA;
			break;
	}
}

// Armazana colega
void AdicionaColega(POSICAO pos, char num_labirinto, COLEGA colegas[])
{
	bool adicionado = false;
	unsigned char i = num_labirinto * NUM_COLEGAS;

	while(!adicionado){

		if(!colegas[i].ocupado){
			colegas[i].pos = pos;
			colegas[i].ocupado = true;
			adicionado = true;
		}
		else if((i + 1) < NUM_COLEGAS){
			i++;
		}
		else {
			colegas[i].pos = pos;
			adicionado = true;
		}
	}
}

// Deixa de armazenar colega
void RemoveColega(POSICAO pos, char num_labirinto, COLEGA colegas[])
{
	bool removido = false;
	unsigned char i = num_labirinto * NUM_COLEGAS;

	while(!removido){

		if(colegas[i].ocupado &&
		   colegas[i].pos.x == pos.x &&
		   colegas[i].pos.y == pos.y){

			colegas[i].ocupado = false;
			removido = true;
		}
		else if((i + 1) < NUM_COLEGAS){
			i++;
		}
		else {
			removido = true;
		}
	}
}