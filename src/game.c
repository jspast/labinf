#include <stdio.h>
#include "raylib.h"

#define gameScreenWidth 640
#define gameScreenHeight 360
#define MAX_TAMANHO_LABIRINTO 200
#define MAX_LABIRINTOS 20

typedef struct{
	int tamanho;
	char matriz[MAX_TAMANHO_LABIRINTO][MAX_TAMANHO_LABIRINTO];
} LABIRINTO;

typedef struct{
	int fase;
	// ...
} SAVE;

typedef struct{
	int posX;
	int posY;
	// ...
} JOGADOR;

LABIRINTO labirintos[MAX_LABIRINTOS];
JOGADOR jogador;

int NovoJogo(){

	int i;

	jogador.posX = 1;
	jogador.posY = 1;
	labirintos[0].matriz[1][1] = 2;

	labirintos[0].tamanho = 100;

	for(i = 0; i < labirintos[0].tamanho; i += 3)
		labirintos[0].matriz[i][0] = 1;

	return 1;
}

int CarregaJogo(){

	FILE *arq;
	SAVE save;

    arq = fopen("save.dat","rb");
    if(arq == NULL)
		{}
    else{
		fread(&save, sizeof(SAVE), 1, arq);
	}

	return -1;
}

int CarregaLabirintos(){

	int num_labirintos = -1;
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

void MovimentacaoJogador(){

	if (IsKeyDown(KEY_UP) && jogador.posY > 0 && labirintos[0].matriz[jogador.posX][jogador.posY-1] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posY--;
	}  if (IsKeyDown(KEY_DOWN) && jogador.posY < labirintos[0].tamanho-1 && labirintos[0].matriz[jogador.posX][jogador.posY+1] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posY++;
	}  if (IsKeyDown(KEY_LEFT) && jogador.posX > 0 && labirintos[0].matriz[jogador.posX-1][jogador.posY] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posX--;
	}  if (IsKeyDown(KEY_RIGHT) && jogador.posX < labirintos[0].tamanho-1 && labirintos[0].matriz[jogador.posX+1][jogador.posY] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posX++;
	}

	labirintos[0].matriz[jogador.posX][jogador.posY] = 2;
	WaitTime(0.05);
}

void Jogo(){

	int quadrado_tamanho = 16;
	int i, j;
	int exibicaoX = 0;
	int exibicaoY = 0;

	MovimentacaoJogador();

	exibicaoX = 40 * (jogador.posX / 40);
	exibicaoY = 20 * (jogador.posY / 20);

	for(i = exibicaoX ; i < labirintos[0].tamanho && i < exibicaoX + 40; i++){
		for(j = exibicaoY; j < labirintos[0].tamanho && j < exibicaoY + 20; j++){
            switch(labirintos[0].matriz[i][j]){
            case 0:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, BLACK);
                break;
            case 1:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, WHITE);
                break;
			case 2:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, ORANGE);
                break;
            }
		}
    }
}


