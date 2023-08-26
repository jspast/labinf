#include "game.h"

void MovimentacaoJogador(JOGADOR *jogador, LABIRINTO *labirinto)
{
	char ultimo_mov;

	if(jogador->cooldown > 0)
		jogador->cooldown -= GetFrameTime();
	else{
		// Verifica qual foi o último movimento do jogador
		if(jogador->movX != 0)
			ultimo_mov = 'X';

		if(jogador->movY != 0)
			ultimo_mov = 'Y';

		jogador->movX = 0;
		jogador->movY = 0;

		// Calcula o novo movimento do jogador
		if(IsKeyDown(KEY_UP))
			jogador->movY--;
		if(IsKeyDown(KEY_DOWN))
			jogador->movY++;
		if(IsKeyDown(KEY_LEFT))
			jogador->movX--;
		if(IsKeyDown(KEY_RIGHT))
			jogador->movX++;

		// Se pressionou duas teclas, move-se para direção diferente da anterior
		if(jogador->movX != 0 && jogador->movY != 0){
			if(ultimo_mov == 'X')
				jogador->movX = 0;
			if(ultimo_mov == 'Y')
				jogador->movY = 0;
		}

		// Se o movimento for válido, atualiza a matriz com a nova posição do jogador
		if(labirinto->matriz[jogador->pos.x + jogador->movX][jogador->pos.y] != 1 &&
		   jogador->pos.x + jogador->movX != -1 && jogador->pos.x + jogador->movX != labirinto->tamX)
		{
			labirinto->matriz[jogador->pos.x][jogador->pos.y] = 0;
			jogador->pos.x += jogador->movX;
		}
		if(labirinto->matriz[jogador->pos.x][jogador->pos.y + jogador->movY] != 1 &&
		   jogador->pos.y + jogador->movY != -1 && jogador->pos.y + jogador->movY != labirinto->tamY)
		{
			labirinto->matriz[jogador->pos.x][jogador->pos.y] = 0;
			jogador->pos.y += jogador->movY;
		}
		labirinto->matriz[jogador->pos.x][jogador->pos.y] = 2;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		jogador->cooldown = 0.05;
	}
}
