#include "game.h"

#define MAX_TAMANHO_LABIRINTO 100
#define NUM_DIFICULDADES 3
#define BLOCOS_X 50
#define BLOCOS_Y 25
#define TAM_BLOCO 16

void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador);
int CarregaFase(FASE *fase_atual, int num_fase);

// Preenche a fase atual com os dados do arquivo de labirintos
int CarregaFase(FASE *fase_atual, int num_fase)
{
    int ajuste_lab;
    FILE *arqlab;

    arqlab = fopen("labirintos.dat", "rb");
	if(arqlab == NULL)
		return 0;
	else{
    	ajuste_lab = sizeof(LABIRINTO)*num_fase;
    	fseek(arqlab, ajuste_lab, 0);

    	fread(&fase_atual->labirinto, sizeof(LABIRINTO), 1, arqlab);

    	fclose(arqlab);
		return 1;
	}
}

// Renderização da matriz principal do jogo
void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador)
{
    int exibicaoX = 0;
	int exibicaoY = 0;
    int i, j;

	// Calcula a parte da matriz a ser desenhada
    exibicaoX = BLOCOS_X * (jogador.pos.x / BLOCOS_X);
	exibicaoY = BLOCOS_Y * (jogador.pos.y / BLOCOS_Y);

	// Desenha a parte da matriz
    for(i = exibicaoX ; i < labirinto.tamX && i < exibicaoX + BLOCOS_X; i++){
		for(j = exibicaoY; j < labirinto.tamY && j < exibicaoY + BLOCOS_Y; j++){
            switch(labirinto.m[i][j]){
            case 0:
                DrawRectangle((i - exibicaoX) * TAM_BLOCO, (j - exibicaoY) * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, DARKGRAY);
                break;
            case 1:
                DrawRectangle((i - exibicaoX) * TAM_BLOCO, (j - exibicaoY) * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, WHITE);
                break;
			case 2:
                DrawRectangle((i - exibicaoX) * TAM_BLOCO, (j - exibicaoY) * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, ORANGE);
                break;
			case 3:
                DrawRectangle((i - exibicaoX) * TAM_BLOCO, (j - exibicaoY) * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, RED);
                break;
            case 4:
                DrawRectangle((i - exibicaoX) * TAM_BLOCO, (j - exibicaoY) * TAM_BLOCO, TAM_BLOCO, TAM_BLOCO, BLUE);
                break;
            }
		}
    }
}
