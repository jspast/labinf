#include "tipos.h"
#include "constantes.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

void CarregaGanhadores(GANHADORES *g)
{
	g->total = 0;

	FILE *arq;
	arq = fopen(A_GANHADORES, "rb");
	if(arq != NULL) {
		while(!feof(arq)){
			fread(&g->ganhadores[g->total], sizeof(GANHADOR), 1, arq);
			g->total++;
		}
		fclose(arq);
		g->total--;
	}
}

void SalvaGanhadores(GANHADORES g)
{
	FILE *arq;
	arq = fopen(A_GANHADORES, "wb");
	if(arq != NULL) {
		fwrite(&g, sizeof(GANHADOR), g.total, arq);
		fclose(arq);
	}
}

char PosicaoGanhador(GANHADORES g, int pontuacao)
{
	char i = g.total;

	while(i > 0 && pontuacao > g.ganhadores[i - 1].pontuacao)
		i--;

	return i;
}

void InsereGanhador(GANHADORES *g, int posicao, JOGADOR j)
{
	unsigned char i = g->total;

	if(i > MAX_GANHADORES)
		i = MAX_GANHADORES;

	while(i > posicao){
		strcpy(g->ganhadores[i].nome, g->ganhadores[i-1].nome);
		g->ganhadores[i].pontuacao = g->ganhadores[i-1].pontuacao;
		i--;
	}

	strcpy(g->ganhadores[posicao].nome, j.nome);
	g->ganhadores[posicao].pontuacao = j.pontuacao;
}