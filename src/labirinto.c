#include <stdio.h>
#include "game.h"

#define MAX_TAMANHO_LABIRINTO 100
#define NUM_DIFICULDADES 3

void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador);
void CarregaFase(FASE *fase_atual, int num_fase);


// Carrega enésimo labirinto do arquivo -> a CarregaFase já está fazendo isso!
/*
int CarregaLabirinto(int n)
{
	FILE *arq;

    arq = fopen("labirintos.dat","rb");
    if(arq == NULL)
		num_labirintos = -1; // ERRO
    else{
		while(!feof(arq)){
			num_labirintos++;
			fread(&labirintos[num_labirintos], sizeof(LABIRINTO), 1, arq);
		}
	}
	fclose(arq);

	return num_labirintos;
}
*/

// Preenche a fase atual com os dados do arquivo de labirintos
void CarregaFase(FASE *fase_atual, int num_fase) {
    int ajuste_lab, num_lab;
    int m, n;
    FILE *arqlab;

    arqlab = fopen("labirintos.dat", "rb");

    ajuste_lab = sizeof(LABIRINTO)*num_fase;
    fseek(arqlab, ajuste_lab, 0);

    fread(&fase_atual->labirinto, sizeof(LABIRINTO), 1, arqlab);

    fclose(arqlab);
}

// Renderização da matriz principal do jogo
void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador)
{
	int quadrado_tamanho = 16;
    int exibicaoX = 0;
	int exibicaoY = 0;
    int i, j;

    exibicaoX = 50 * (jogador.pos.x / 50);
	exibicaoY = 25 * (jogador.pos.y / 25);

    for(i = exibicaoX ; i < labirinto.tamX && i < exibicaoX + 50; i++){
		for(j = exibicaoY; j < labirinto.tamY && j < exibicaoY + 25; j++){
            switch(labirinto.m[i][j]){
            case 0:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, DARKGRAY);
                break;
            case 1:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, WHITE);
                break;
			case 2:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, ORANGE);
                break;
			case 3:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, RED);
                break;
            case 4:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, BLUE);
                break;
            }
		}
    }
}
