#include <raylib.h>
#include <stdlib.h>
#include "game.h"
#include "defines.h"

bool AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, FASE *fase);
void MovimentacaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador, FASE *fase);
void DirecaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador, FASE fase);
bool ProfessorTemVisao(POSICAO professor, POSICAO jogador, FASE fase);
bool ProfessorAoLado(int id, PROFESSOR professores[], JOGADOR jogador);

// Lógica completa dos professores, atualiza os ativos
bool AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, FASE *fase)
{
	bool perguntar = false;
	int i;

	for(i = 0; i < fase->max_professores; i++){
		if(professores[i].ativo == 1){
			MovimentacaoProfessor(i, professores, jogador, fase);
			if(ProfessorAoLado(i, professores, jogador)){
				professores[i].ativo = 0;
				fase->labirinto.m[professores[i].pos.x][professores[i].pos.y] = 0;
				perguntar = true;
			}
		}
	}
	return perguntar;
}

// Mecânica da movimentação do professor
void MovimentacaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador, FASE *fase)
{
	// Verifica se já passou tempo suficiente desde o último movimento
	if(professores[id].cooldown > 0)
		professores[id].cooldown -= GetFrameTime();
	else{
		// Define a direção da movimentação do professor
		DirecaoProfessor(id, professores, jogador, *fase);

		// Se o movimento for válido, atualiza a matriz com a nova posição do professor
		if(fase->labirinto.m[professores[id].pos.x + professores[id].movX][professores[id].pos.y + professores[id].movY] == 0 &&
		   professores[id].pos.x + professores[id].movX != -1 &&
		   professores[id].pos.x + professores[id].movX != fase->labirinto.tamX &&
		   professores[id].pos.y + professores[id].movY != -1 &&
		   professores[id].pos.y + professores[id].movY != fase->labirinto.tamY)
		{
			fase->labirinto.m[professores[id].pos.x][professores[id].pos.y] = 0;
			professores[id].pos.x += professores[id].movX;
			professores[id].pos.y += professores[id].movY;
		}
		else{
			professores[id].movX = 0;
			professores[id].movY = 0;
		}
		fase->labirinto.m[professores[id].pos.x][professores[id].pos.y] = 3;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		professores[id].cooldown = 0.03;
	}
}

// Define a direção de movimento do professor
void DirecaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador, FASE fase)
{
	int distX;
	int distY;

	// Calcula as distancias X e Y entre o jogador e o professor
	distX = jogador.pos.x - professores[id].pos.x;
	distY = jogador.pos.y - professores[id].pos.y;

	// Se a distancia entre o jogador e o professor for grande ou o professor não tem visão do aluno
	if(sqrt(pow(distX, 2) + pow(distY, 2)) > 10 ||
	   !ProfessorTemVisao(professores[id].pos, jogador.pos, fase)){
		// E se ele já não tiver uma direção, define uma direção aleatória para o professor
		if(professores[id].movX == 0 && professores[id].movY == 0)
			switch(GetRandomValue(0, 3)){
			case 0:
				professores[id].movX = 1;
				break;
			case 1:
				professores[id].movX = -1;
				break;
			case 2:
				professores[id].movY = 1;
				break;
			case 3:
				professores[id].movY = -1;
				break;
			}
	}
	// Senão, dadas as distancias X e Y, decide a direção do movimento perseguindo o aluno
	else{
		professores[id].movX = 0;
		professores[id].movY = 0;

		if(abs(distX) >= abs(distY)){
			if(distX > 0)
				professores[id].movX = 1;
			else
				professores[id].movX = -1;
		}
		else{
			if(distY > 0)
				professores[id].movY = 1;
			else
				professores[id].movY = -1;
		}
	}
}

// Função recursiva que verifica se há parede entre o professor e o aluno
bool ProfessorTemVisao(POSICAO professor, POSICAO jogador, FASE fase)
{
	int distX;
	int distY;
	int movX = 0;
	int movY = 0;

	// Calcula as distancias X e Y entre o jogador e o professor
	distX = jogador.x - professor.x;
	distY = jogador.y - professor.y;

	// Verifica a direção do próximo movimento
	if(abs(distX) >= abs(distY)){
		if(distX > 0)
			movX = 1;
		else
			movX = -1;
	}
	else{
		if(distY > 0)
			movY = 1;
		else
			movY = -1;
	}

	// Caso trivial professor consegue chegar no aluno
	if(distX == 0 && distY == 0)
		return 1;
	// Caso trivial há obstáculo entre professor e aluno
	else if(fase.labirinto.m[professor.x][professor.y] != 0 &&
			fase.labirinto.m[professor.x][professor.y] != 3)
		return 0;
	// Caso recursivo testa o próximo movimento
	else
		return ProfessorTemVisao((POSICAO){professor.x + movX, professor.y + movY}, jogador, fase);
}

// Verifica se o professor está em um bloco adjacente ao jogador
bool ProfessorAoLado(int id, PROFESSOR professores[], JOGADOR jogador)
{
	bool ao_lado = false;

	if(professores[id].pos.x+1 == jogador.pos.x ||
	   professores[id].pos.x == jogador.pos.x ||
	   professores[id].pos.x-1 == jogador.pos.x)
	{
		if(professores[id].pos.y+1 == jogador.pos.y ||
		   professores[id].pos.y == jogador.pos.y ||
		   professores[id].pos.y-1 == jogador.pos.y)
		{
		   ao_lado = true;
		}
	}
	return ao_lado;
}
