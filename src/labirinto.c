#include "game.h"

#define MAX_TAMANHO_LABIRINTO 100
#define NUM_DIFICULDADES 3
#define BLOCOS_X 50
#define BLOCOS_Y 25
#define TAM_BLOCO 16

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

// Renderização da matriz principal do jogo
void DesenhaLabirinto(LABIRINTO labirinto, JOGADOR jogador, Texture2D texturas[])
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
			// Se o bloco a ser desenhado for livre, não desenhar nada
            case 0:
                break;
			// Senão, desenhar a textura correspondente (número do bloco - 1)
            default:
				DrawTexture(texturas[(labirinto.m[i][j] - 1)], (i - exibicaoX) * TAM_BLOCO, (j - exibicaoY) * TAM_BLOCO, WHITE);
                break;
            }
		}
    }
}
