#include <raylib.h>
#include <stdio.h>
#include "game.h"

// Preenche a fase atual com os dados do arquivo de labirintos
bool CarregaFase(FASE *fase_atual, int num_fase)
{
	int ajuste_lab;
	FILE *arqlab;

	arqlab = fopen("labirintos.dat", "rb");
	if(arqlab == NULL)
		return false;
	else{
		ajuste_lab = sizeof(LABIRINTO)*num_fase;
		fseek(arqlab, ajuste_lab, 0);

		fread(&fase_atual->labirinto, sizeof(LABIRINTO), 1, arqlab);

		fclose(arqlab);
		return true;
	}
}
