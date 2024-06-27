#include "tipos.h"
#include "constantes.h"
#include <raylib.h>
#include <stdlib.h>
#include <math.h>

POSICAO DistanciaVetor(POSICAO a, POSICAO b)
{
	POSICAO dist;

	dist.x = a.x - b.x;
	dist.y = a.y - b.y;

	return dist;
}

short Distancia(POSICAO a, POSICAO b)
{
	POSICAO dist = DistanciaVetor(a, b);

	return sqrt(pow(dist.x, 2) + pow(dist.y, 2));
}

// Calcula a direção de movimento
MOVIMENTO MovimentoProfessor(POSICAO dist)
{
	MOVIMENTO mov = {0};

	if(abs(dist.x) > abs(dist.y)){
		if(dist.x > 0)
			mov.x = 1;
		else
			mov.x = -1;
	}
	else if(abs(dist.x) < abs(dist.y)){
		if(dist.y > 0)
			mov.y = 1;
		else
			mov.y = -1;
	}
	else {
		if(dist.x > 0)
			mov.x = 1;
		else
			mov.x = -1;
		if(dist.y > 0)
			mov.y = 1;
		else
			mov.y = -1;
	}

	return mov;
}

// Verifica se há parede entre o professor e o aluno
bool ProfessorTemVisao(POSICAO p, POSICAO j, FASE f)
{
	bool visao = true;

	MOVIMENTO mov = {0};
	POSICAO dist = DistanciaVetor(j, p);

	do{
		mov = MovimentoProfessor(dist);

		p.x += mov.x;
		p.y += mov.y;

		if(f.labirinto.m[p.x][p.y] != M_VAZIO &&
		   f.labirinto.m[p.x][p.y] != M_ALUNO)
		   visao = false;

		dist = DistanciaVetor(j, p);

	} while(visao && (dist.x != 0 || dist.y != 0));

	return visao;
}

// Verifica se o professor está em um bloco adjacente ao jogador
bool ProfessorAoLado(PROFESSOR p, JOGADOR j)
{
	bool ao_lado = false;

	if(p.pos.x+1 == j.pos.x ||
	   p.pos.x == j.pos.x ||
	   p.pos.x-1 == j.pos.x)
	{
		if(p.pos.y+1 == j.pos.y ||
		   p.pos.y == j.pos.y ||
		   p.pos.y-1 == j.pos.y)
		{
		   ao_lado = true;
		}
	}
	return ao_lado;
}

// Define a direção de movimento do professor
void DirecaoProfessor(PROFESSOR *p, JOGADOR j, FASE f)
{
	short dist = Distancia(j.pos, p->pos);

	// Se a distancia entre o j e o professor for grande ou o professor não tem visão do aluno
	if(dist > 10 || !ProfessorTemVisao(p->pos, j.pos, f)){
		// E se ele já não tiver uma direção, define uma direção aleatória para o professor
		if(p->mov.x == 0 && p->mov.y == 0)
			switch(GetRandomValue(0, 7)){
			case 0:
				p->mov.x = 1;
				break;
			case 1:
				p->mov.x = -1;
				break;
			case 2:
				p->mov.y = 1;
				break;
			case 3:
				p->mov.y = -1;
				break;
			case 4:
				p->mov.x = 1;
				p->mov.y = 1;
				break;
			case 5:
				p->mov.x = -1;
				p->mov.y = 1;
				break;
			case 6:
				p->mov.x = 1;
				p->mov.y = -1;
				break;
			case 7:
				p->mov.x = -1;
				p->mov.y = -1;
				break;
			}
	}
	// Senão, dadas as distancias X e Y, decide a direção do movimento perseguindo o aluno
	else{
		p->mov = MovimentoProfessor(DistanciaVetor(j.pos, p->pos));
	}
}

// Mecânica da movimentação do professor
void MovimentacaoProfessor(PROFESSOR *p, JOGADOR j, FASE *fase)
{
	// Verifica se já passou tempo suficiente desde o último movimento
	if(p->cooldown > 0)
		p->cooldown -= GetFrameTime();
	else{
		// Define a direção da movimentação do professor
		DirecaoProfessor(p, j, *fase);

		// Se o movimento for válido, atualiza a matriz com a nova posição do professor
		if(fase->labirinto.m[p->pos.x + p->mov.x][p->pos.y + p->mov.y] == M_VAZIO &&
		   p->pos.x + p->mov.x != -1 &&
		   p->pos.x + p->mov.x != fase->labirinto.tam.x &&
		   p->pos.y + p->mov.y != -1 &&
		   p->pos.y + p->mov.y != fase->labirinto.tam.y)
		{
			fase->labirinto.m[p->pos.x][p->pos.y] = M_VAZIO;
			p->pos.x += p->mov.x;
			p->pos.y += p->mov.y;
		}
		else{
			p->mov.x = 0;
			p->mov.y = 0;
		}
		fase->labirinto.m[p->pos.x][p->pos.y] = M_PROFESSOR;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		if(p->mov.x != 0 && p->mov.y != 0)
			p->cooldown = COOLDOWN_PROFESSOR * RAIZ_2;
		else
			p->cooldown = COOLDOWN_PROFESSOR;
	}
}

// Lógica completa dos p, atualiza os ativos
bool AtualizaProfessores(JOGO *jogo)
{
	bool perguntar = false;
	int i;

	for(i = 0; i < jogo->fase.max_professores; i++){
		if(jogo->fase.professores[i].ativo == 1){
			MovimentacaoProfessor(&jogo->fase.professores[i], jogo->fase.jogador, &jogo->fase);
			if(ProfessorAoLado(jogo->fase.professores[i], jogo->fase.jogador)){
				jogo->fase.professores[i].ativo = 0;
				jogo->fase.labirinto.m[jogo->fase.professores[i].pos.x][jogo->fase.professores[i].pos.y] = M_VAZIO;
				perguntar = true;
			}
		}
	}
	return perguntar;
}