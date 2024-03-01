#include <raylib.h>
#include "defines.h"
#include "game.h"
#include "desenha.h"

// Renderização do jogo
void DesenhaJogo(FASE fase, JOGADOR jogador, PROFESSOR professores[], Texture2D texturas[])
{
	POSICAO exibicao = CalculaExibicao(jogador);

	DesenhaLabirinto(fase.labirinto, exibicao, texturas);

	DesenhaJogador(jogador, exibicao, texturas[1]);
	DesenhaProfessores(professores, fase, exibicao, texturas[2]);

	DesenhaIndicadores(jogador, fase, texturas[5]);
}

POSICAO CalculaExibicao(JOGADOR jogador)
{
	POSICAO exibicao;

	// Calcula a parte da matriz a ser desenhada
	exibicao.x = BLOCOS_X * (jogador.pos.x / BLOCOS_X);
	exibicao.y = BLOCOS_Y * (jogador.pos.y / BLOCOS_Y);

	return exibicao;
}

void DesenhaLabirinto(LABIRINTO labirinto, POSICAO exibicao, Texture2D texturas[])
{
	int i, j;

	for(i = exibicao.x ; i < labirinto.tamX && i < exibicao.x + BLOCOS_X; i++){
		for(j = exibicao.y; j < labirinto.tamY && j < exibicao.y + BLOCOS_Y; j++){
			switch(labirinto.m[i][j]){
			// Se o bloco a ser desenhado for livre ou jogador ou professor, não desenhar nada
			case 0:
			case 2 ... 3:
				break;
			// Senão, desenhar a textura correspondente (número do bloco - 1)
			default:
				DrawTexture(texturas[(labirinto.m[i][j] - 1)], (i - exibicao.x) * TAM_BLOCO, (j - exibicao.y) * TAM_BLOCO, WHITE);
				break;
			}
		}
	}

}

void DesenhaJogador(JOGADOR jogador, POSICAO exibicao, Texture2D textura)
{
	DrawTexture(textura, (jogador.pos.x - exibicao.x - (jogador.movX * jogador.cooldown/COOLDOWN_JOGADOR)) * TAM_BLOCO, (jogador.pos.y - exibicao.y - (jogador.movY * jogador.cooldown/COOLDOWN_JOGADOR)) * TAM_BLOCO, WHITE);
}

void DesenhaProfessores(PROFESSOR professores[], FASE fase, POSICAO exibicao, Texture2D textura)
{
	int i;

	for(i = 0; i < fase.max_professores; i++){
		if(professores[i].ativo == 1 &&
		   professores[i].pos.x - exibicao.x >= 0 && professores[i].pos.x - exibicao.x < BLOCOS_X &&
		   professores[i].pos.y - exibicao.y >= 0 && professores[i].pos.y - exibicao.y < BLOCOS_Y)
			DrawTexture(textura, 
					   (professores[i].pos.x - exibicao.x - (professores[i].movX * professores[i].cooldown/COOLDOWN_PROFESSOR)) * TAM_BLOCO,
					   (professores[i].pos.y - exibicao.y - (professores[i].movY * professores[i].cooldown/COOLDOWN_PROFESSOR)) * TAM_BLOCO,
					   WHITE);
	}
}

// Desenha a HUD com as informações do jogador e da fase
void DesenhaIndicadores(JOGADOR jogador, FASE fase, Texture2D textura)
{
	int i;

	DrawText("Vida:", 10, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d", jogador.vida), 10, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Tempo:", 100, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%4.2f", jogador.tempo_restante), 100, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Créditos:", 210, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d/%d", jogador.creditos, fase.min_creditos), 210, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Labirinto:", 340, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d", jogador.labirinto), 340, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Pontuação:", 480, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d", jogador.pontuacao), 480, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Inventário:", 640, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	for(i = 0; i < jogador.bombas; i++)
		DrawTexture(textura, 640 + (i * 16), INDICADORES_Y2, WHITE);
}