#include "tipos.h"
#include "constantes.h"
#include <raylib.h>

void CarregaTexturas(Texture2D texturas[])
{
	texturas[T_PAREDE] = LoadTexture("res/texturas/parede.png");
	texturas[T_ALUNO] = LoadTexture("res/texturas/aluno.png");
	texturas[T_PROFESSOR] = LoadTexture("res/texturas/professor.png");
	texturas[T_COLEGA] = LoadTexture("res/texturas/colega.png");
	texturas[T_CREDITO] = LoadTexture("res/texturas/credito.png");
	texturas[T_SAIDA] = LoadTexture("res/texturas/saida.png");
	texturas[T_BOMBA_INATIVA] = LoadTexture("res/texturas/bomba_inativa.png");
	texturas[T_BOMBA_ATIVA] = LoadTexture("res/texturas/bomba_ativa.png");
	texturas[T_CORACAO] = LoadTexture("res/texturas/coracao.png");
	texturas[T_RELOGIO] = LoadTexture("res/texturas/relogio.png");
}

void FechaTexturas(Texture2D texturas[])
{
	int i;

	for(i = 0; i < 6; i++)
		UnloadTexture(texturas[i]);
}

// Calcula a parte da matriz a ser desenhada
POSICAO CalculaExibicao(JOGADOR jogador)
{
	POSICAO exibicao;

	exibicao.x = BLOCOS_X * (jogador.pos.x / BLOCOS_X);
	exibicao.y = BLOCOS_Y * (jogador.pos.y / BLOCOS_Y);

	return exibicao;
}

// Desenha elementos estáticos do labirinto
void DesenhaLabirinto(LABIRINTO labirinto, POSICAO exibicao, Texture2D texturas[])
{
	int i, j;

	for (i = exibicao.x ; i < labirinto.tam.x && i < exibicao.x + BLOCOS_X; i++){

		for (j = exibicao.y; j < labirinto.tam.y && j < exibicao.y + BLOCOS_Y; j++){

			if (labirinto.m[i][j] != M_VAZIO &&
				labirinto.m[i][j] != M_ALUNO &&
				labirinto.m[i][j] != M_PROFESSOR){

				DrawTexture(texturas[(labirinto.m[i][j] - 1)],
							(i - exibicao.x) * TAM_BLOCO, (j - exibicao.y) * TAM_BLOCO,
							WHITE);
			}
		}
	}

}

void DesenhaJogador(JOGADOR jogador, POSICAO exibicao, Texture2D textura)
{
	float cooldown;

	if (jogador.mov.x != 0 && jogador.mov.y != 0)
		cooldown = COOLDOWN_JOGADOR * RAIZ_2;
	else
		cooldown = COOLDOWN_JOGADOR;

	DrawTexture(textura, 
				(jogador.pos.x - exibicao.x - (jogador.mov.x * jogador.cooldown/cooldown)) * TAM_BLOCO,
				(jogador.pos.y - exibicao.y - (jogador.mov.y * jogador.cooldown/cooldown)) * TAM_BLOCO,
				WHITE);
}

void DesenhaProfessores(PROFESSOR professores[], FASE fase, POSICAO exibicao, Texture2D textura)
{
	unsigned char i;

	for(i = 0; i < fase.max_professores; i++){
		if(professores[i].ativo == 1 &&
		   professores[i].pos.x - exibicao.x >= 0 && professores[i].pos.x - exibicao.x < BLOCOS_X &&
		   professores[i].pos.y - exibicao.y >= 0 && professores[i].pos.y - exibicao.y < BLOCOS_Y){

			float cooldown;

			if (professores[i].mov.x != 0 && professores[i].mov.y != 0)
				cooldown = COOLDOWN_PROFESSOR * RAIZ_2;
			else
				cooldown = COOLDOWN_PROFESSOR;

			DrawTexture(textura, 
					   (professores[i].pos.x - exibicao.x - (professores[i].mov.x * professores[i].cooldown/cooldown)) * TAM_BLOCO,
					   (professores[i].pos.y - exibicao.y - (professores[i].mov.y * professores[i].cooldown/cooldown)) * TAM_BLOCO,
					   WHITE);
		}
	}
}

// Desenha a HUD com as informações do jogador e da fase
void DesenhaIndicadores(FASE f, SAVE s, Texture2D texturas[])
{
	unsigned char i;

	DrawRectangle(0, INDICADORES_FUNDO_Y, RES_X, INDICADORES_ALTURA, COR_INTERFACE);

	DrawText("Vida:", 10, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	if(s.vida < 6){
		for(i = 0; i < s.vida; i++)
			DrawTexture(texturas[M_CORACAO - 1], 10 + (i * 16), INDICADORES_Y2, WHITE);
	}
	else {
		DrawText(TextFormat("%d", s.vida), 10, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);
	}

	DrawText("Tempo:", 120, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%4.2f", f.tempo_restante), 120, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Créditos:", 240, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d/%d", f.jogador.creditos, f.min_creditos), 240, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Labirinto:", 380, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d", s.fase), 380, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Pontuação:", 520, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("%d", f.jogador.pontuacao), 520, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);

	DrawText("Bombas:", 680, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	if(s.bombas < 8){
		for(i = 0; i < s.bombas; i++)
			DrawTexture(texturas[M_BOMBA_INATIVA - 1], 680 + (i * 16), INDICADORES_Y2, WHITE);
	}
	else {
		DrawText(TextFormat("%d", s.bombas), 680, INDICADORES_Y2, FONTE_INDICADORES, COR_INDICADORES);
	}
}

void DesenhaFase(JOGO jogo)
{
	POSICAO exibicao = CalculaExibicao(jogo.fase.jogador);

	DesenhaLabirinto(jogo.fase.labirinto, exibicao, jogo.texturas);

	DesenhaJogador(jogo.fase.jogador, exibicao, jogo.texturas[T_ALUNO]);
	DesenhaProfessores(jogo.fase.professores, jogo.fase, exibicao, jogo.texturas[T_PROFESSOR]);

	DesenhaIndicadores(jogo.fase, jogo.save, jogo.texturas);
}

// Desenha texto centralizado respeitando a largura máxima quebrando a linha
void DesenhaTextoCentralizado(const char *text, POSICAO pos, short largura_max, float tam_fonte, Color cor)
{
	int length = TextLength(text);

	int i, j = 0;

	while (j < length){

		int line_size = 0;
		int line_length = 0;
		int last_line_length = 0;

		int word_size = 0;
		int word_start;

		char carac;

		i = j;

		while (line_size + word_size < largura_max && i < length){

			line_size += word_size;
			last_line_length = line_length;

			word_start = i;

			do {

				carac = text[i];

				i++;

			} while (carac != ' ' && i < length);

			word_size = MeasureText(TextSubtext(text, word_start, i - word_start), tam_fonte);

			line_length = i - j;
		}

		if (line_size + word_size >= largura_max)
			line_length = last_line_length;

		last_line_length = line_length;

		if (text[j + line_length - 1] == ' ')
			line_length--;

		DrawText(TextSubtext(text, j, line_length), pos.x - (MeasureText(TextSubtext(text, j, line_length), tam_fonte))/2, pos.y, tam_fonte, cor);

		j += last_line_length;

		pos.y += tam_fonte + tam_fonte/2;
	}
}