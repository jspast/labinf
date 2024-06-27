#include "jogo.h"
#include "constantes.h"
#include "tipos.h"
#include <raylib.h>

// Testa se o jogador tem créditos suficientes para passar de fase
bool CreditoSuficiente(JOGADOR *j, FASE *fase)
{
    return j->creditos >= fase->min_creditos;
}

// Zera variáveis de movimento do jogador
void CancelaMovimentoJogador(JOGADOR *j)
{
	j->mov.x = 0;
	j->mov.y = 0;
}

// Define movimento do jogador
bool MovimentacaoJogador(JOGADOR *j, FASE *fase)
{
	bool movimento = false;

	CancelaMovimentoJogador(j);

	// Recebe o movimento do jogador
	if(IsKeyDown(KEY_UP))
		j->mov.y--;
	if(IsKeyDown(KEY_DOWN))
		j->mov.y++;
	if(IsKeyDown(KEY_LEFT))
		j->mov.x--;
	if(IsKeyDown(KEY_RIGHT))
		j->mov.x++;

	// Descarta movimento para fora da área do labirinto
	if(j->pos.x + j->mov.x == -1 ||
	   j->pos.x + j->mov.x == fase->labirinto.tam.x)
	   j->mov.x = 0;

	if(j->pos.y + j->mov.y == -1 ||
	   j->pos.y + j->mov.y == fase->labirinto.tam.y)
	   j->mov.y = 0;

	// Descarta movimento para blocos não ocupáveis
	if(j->mov.x != 0 &&
	   fase->labirinto.m[j->pos.x + j->mov.x][j->pos.y] == M_PAREDE)
		j->mov.x = 0;

	if(j->mov.y != 0 &&
	   fase->labirinto.m[j->pos.x][j->pos.y + j->mov.y] == M_PAREDE)
		j->mov.y = 0;

	if(j->mov.x != 0 && j->mov.y != 0 && 
	   fase->labirinto.m[j->pos.x + j->mov.x][j->pos.y + j->mov.y] == M_PAREDE)
		j->mov.x = 0;

	if(j->mov.x != 0 || j->mov.y != 0)
		movimento = true;

	return movimento;
}

// Atualiza posição do jogador
void AtualizaPosicaoJogador(JOGADOR *j)
{
	j->pos.x += j->mov.x;
	j->pos.y += j->mov.y;
}

// Atualiza posição do jogador na matriz
void AtualizaMatrizJogador(JOGADOR *j, FASE *fase)
{
	fase->labirinto.m[j->pos.x - j->mov.x][j->pos.y - j->mov.y] = M_VAZIO;
	fase->labirinto.m[j->pos.x][j->pos.y] = M_ALUNO;
}

void IncrementaCreditos(JOGADOR *j, SAVE *save)
{
	j->creditos++;
	save->total_creditos++;
}

// Aumenta em 1 o número de créditos e toca som
void ColisaoJogadorCredito(JOGADOR *j, SAVE *save, Sound som)
{
	IncrementaCreditos(j, save);
	PlaySound(som);
}

// Aumenta em 1 o número de vidas e toca som
void ColisaoJogadorCoracao(SAVE *s, Sound som)
{
	s->vida++;
	PlaySound(som);
}

// Aumenta em 1 o número de bombas e toca som
void ColisaoJogadorBombaInativa(SAVE *s, Sound som)
{
	s->bombas++;
	PlaySound(som);
}

// Aumenta em 30s o tempo e toca som
void ColisaoJogadorRelogio(FASE *f, Sound som)
{
	f->tempo_restante += 30;
	PlaySound(som);
}

// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
void CooldownJogador(JOGADOR *j)
{
	if(j->mov.x != 0 && j->mov.y != 0)
		j->cooldown = COOLDOWN_JOGADOR * RAIZ_2;
	else
		j->cooldown = COOLDOWN_JOGADOR;
}

// Toca sons de passo
void PassoJogador(JOGADOR *j, Sound sons[])
{
	if((j->mov.x != 0 || j->mov.y != 0) &&
	   (!IsSoundPlaying(sons[S_PASSO_0]) && !IsSoundPlaying(sons[S_PASSO_1]) &&
		!IsSoundPlaying(sons[S_PASSO_2]) && !IsSoundPlaying(sons[S_PASSO_3]) &&
		!IsSoundPlaying(sons[S_PASSO_4]) && !j->mov_continuo))
	{
		PlaySound(sons[GetRandomValue(S_PASSO_0, S_PASSO_4)]);
	}
}

void AtualizaMovimentoJogador(JOGADOR *j, FASE *fase, Sound sons[])
{
	PassoJogador(j, sons);
	AtualizaPosicaoJogador(j);
	AtualizaMatrizJogador(j, fase);
}

void GuardaMovimento(JOGADOR *j)
{
	j->ult_mov = j->mov;
}

// Mecânica da movimentação do jogador
bool AtualizaJogador(JOGO *jogo)
{
	bool perguntar = false;

	CalculaPontuacao(&jogo->fase.jogador, jogo->save);

	// Verifica se já passou tempo suficiente desde o último movimento
	if(jogo->fase.jogador.cooldown > 0){
		jogo->fase.jogador.cooldown -= GetFrameTime();
		jogo->fase.jogador.mov_continuo = true;
	}	
	else{

		if(MovimentacaoJogador(&jogo->fase.jogador, &jogo->fase)){

			GuardaMovimento(&jogo->fase.jogador);

			switch(jogo->fase.labirinto.m[jogo->fase.jogador.pos.x + jogo->fase.jogador.mov.x]
										 [jogo->fase.jogador.pos.y + jogo->fase.jogador.mov.y]){

			// Se o jogador se movimentou para a posição de um colega, chamar a pergunta
			case M_COLEGA:
				perguntar = true;
				break;
				
			// Se o jogador alcança a saída do labirinto após o movimento, testa se ele já tem créditos suficientes para passar a fase
			case M_SAIDA:
				jogo->fase.jogador.passou_fase = CreditoSuficiente(&jogo->fase.jogador, &jogo->fase);
				CancelaMovimentoJogador(&jogo->fase.jogador);
				break;

			// Se o jogador passou a ocupar a posição de um crédito após o movimento, aumentar em 1 o número de créditos
			case M_CREDITO:
				ColisaoJogadorCredito(&jogo->fase.jogador, &jogo->save, jogo->audio.sons[7]);
				AtualizaMovimentoJogador(&jogo->fase.jogador, &jogo->fase, jogo->audio.sons);
				break;

			case M_CORACAO:
				ColisaoJogadorCoracao(&jogo->save, jogo->audio.sons[7]);
				AtualizaMovimentoJogador(&jogo->fase.jogador, &jogo->fase, jogo->audio.sons);
				break;

			case M_BOMBA_INATIVA:
				ColisaoJogadorBombaInativa(&jogo->save, jogo->audio.sons[7]);
				AtualizaMovimentoJogador(&jogo->fase.jogador, &jogo->fase, jogo->audio.sons);
				break;

			case M_BOMBA_ATIVA:
				ExplosaoBomba(&jogo->fase.jogador, &jogo->save, &jogo->fase, jogo->audio.sons[S_EXPLOSAO]);
				AtualizaMovimentoJogador(&jogo->fase.jogador, &jogo->fase, jogo->audio.sons);
				break;
			
			case M_RELOGIO:
				ColisaoJogadorRelogio(&jogo->fase, jogo->audio.sons[7]);
				AtualizaMovimentoJogador(&jogo->fase.jogador, &jogo->fase, jogo->audio.sons);
				break;

			default:
				AtualizaMovimentoJogador(&jogo->fase.jogador, &jogo->fase, jogo->audio.sons);
			}

			CooldownJogador(&jogo->fase.jogador);
		}
		else{
			jogo->fase.jogador.mov_continuo = false;
		}
	}
	return perguntar;
}

void JogarBomba(JOGADOR *j, SAVE *s, FASE *fase)
{
	j->bomba.pos = j->pos;
	j->bomba.pos.x += j->ult_mov.x;
	j->bomba.pos.y += j->ult_mov.y;

	if(fase->labirinto.m[j->bomba.pos.x][j->bomba.pos.y] == M_VAZIO){
		fase->labirinto.m[j->bomba.pos.x][j->bomba.pos.y] = M_BOMBA_ATIVA;
		s->bombas--;
		j->bomba.tempo = 2;
		j->bomba.ativa = true;
	}
}

void ExplosaoBomba(JOGADOR *j, SAVE *s, FASE *fase, Sound som)
{
	int i, k;

	j->bomba.ativa = false;
	fase->labirinto.m[j->bomba.pos.x][j->bomba.pos.y] = M_VAZIO;

	PlaySound(som);

	for(i = j->bomba.pos.x - 2; i <= j->bomba.pos.x + 2; i++){
		for(k = j->bomba.pos.y - 2; k <= j->bomba.pos.y + 2; k++){

			switch (fase->labirinto.m[i][k]){

			case M_PAREDE:
				fase->labirinto.m[i][k] = M_VAZIO;
				break;

			case M_ALUNO:
				s->vida--;
			}
				
		}
	}
}

void AtualizaBomba(JOGADOR *j, SAVE *save, FASE *fase, Sound som)
{
	if (j->bomba.tempo < 0)
		ExplosaoBomba(j, save, fase, som);
	else
		j->bomba.tempo -= GetFrameTime();
}

// Calcula a pontuação total acumulada pelo aluno
void CalculaPontuacao(JOGADOR *j, SAVE s)
{
	int termo2 = 0;

	if(s.tempo_total / 60 != 0)
		termo2 = s.total_creditos / (s.tempo_total / 60);
	
	j->pontuacao = 10 * MAX_CREDITOS * s.fase + termo2;
}
