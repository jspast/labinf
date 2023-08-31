#include "game.h"

// Testa se o jogador tem créditos suficientes para passar de fase
bool CreditoSuficiente(JOGADOR *jogador, FASE *fase) {
    bool passar_fase;

    if (jogador->creditos >= fase->min_creditos)
        passar_fase = true;
    else
        passar_fase = false;

    return passar_fase;
}

// Mecânica da movimentação do jogador
bool MovimentacaoJogador(JOGADOR *jogador, FASE *fase, bool *passar_fase)
{
	bool perguntar = false;
	char ultimo_mov;

	// Verifica se já passou tempo suficiente desde o último movimento
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

		// Se o jogador ocupará a posição de um crédito após o movimento, aumentar em 1 o número de créditos
		if(fase->labirinto.m[jogador->pos.x + jogador->movX][jogador->pos.y + jogador->movY] == 5)
			jogador->creditos++;

		// Se o jogador ocupará a posição de uma estátua após o movimento, chamar a pergunta
		if(fase->labirinto.m[jogador->pos.x + jogador->movX][jogador->pos.y + jogador->movY] == 4)
			perguntar = true;

		// Se o jogador alcança a saída do labirinto após o movimento, testa se ele já tem créditos suficientes para passar a fase
        if(fase->labirinto.m[jogador->pos.x + jogador->movX][jogador->pos.y + jogador->movY] == 7)
            *passar_fase = CreditoSuficiente(jogador, fase);

		// Se o movimento for válido, atualiza a matriz com a nova posição do jogador
		if(fase->labirinto.m[jogador->pos.x + jogador->movX][jogador->pos.y + jogador->movY] != 1 &&
		   fase->labirinto.m[jogador->pos.x + jogador->movX][jogador->pos.y + jogador->movY] != 7 &&
		   jogador->pos.x + jogador->movX != -1 && jogador->pos.x + jogador->movX != fase->labirinto.tamX &&
		   jogador->pos.y + jogador->movY != -1 && jogador->pos.y + jogador->movY != fase->labirinto.tamY)
		{
			fase->labirinto.m[jogador->pos.x][jogador->pos.y] = 0;
			jogador->pos.x += jogador->movX;
			jogador->pos.y += jogador->movY;
		}
		fase->labirinto.m[jogador->pos.x][jogador->pos.y] = 2;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		jogador->cooldown = 0.05;
	}
	return perguntar;
}

void JogarBomba(JOGADOR *jogador, FASE *fase)
{
	int i, j;

	jogador->bombas--;

	for(i = jogador->pos.x - 1; i <= jogador->pos.x + 1; i++){
		for(j = jogador->pos.y - 1; j <= jogador->pos.y + 1; j++){
			if(fase->labirinto.m[i][j] == 1)
				fase->labirinto.m[i][j] = 0;
		}
	}
}
