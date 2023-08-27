#include "game.h"

void DirecaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador);
int AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, FASE *fase);
void MovimentacaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador, FASE *fase);
int ProfessorAoLado(int id, PROFESSOR professores[], JOGADOR jogador);

void DirecaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador)
{
	int distX;
	int distY;

	// Calcula as distancias X e Y entre o jogador e o professor
	distX = jogador.pos.x - professores[id].pos.x;
	distY = jogador.pos.y - professores[id].pos.y;

	// Se a distancia entre o jogador e o professores[id] for grande
	if(sqrt(pow(distX, 2) + pow(distY, 2)) > 10){
        // E se ele ja nao tiver uma direcao, define uma direçao aleatoria para o professor
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
    // Senao, dadas as distancias X e Y, decide a direcao do movimento perseguindo o aluno
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


/* Ideia de função recursiva para verificar se há parede entre o professor e o aluno.
int ProfessorTemVisao(int posX, int posY)
{
	DirecaoProfessor();

	if(distX == 0 && distY == 0)
		return 1;

	if(labirinto->matriz[pos.x + professor.movX][pos.y + professor.movY] == 1)
		return 0;
	else{

		return ProfessorTemVisao();
	}
}
*/

int AtualizaProfessores(PROFESSOR professores[], JOGADOR jogador, FASE *fase)
{
	int perguntar = 0;
	int i;

	for(i = 0; i < MAX_PROFESSORES; i++){
		if(professores[i].ativo == 1){
			MovimentacaoProfessor(i, professores, jogador, fase);
			if(ProfessorAoLado(i, professores, jogador)){
				professores[i].ativo = 0;
				fase->labirinto.m[professores[i].pos.x][professores[i].pos.y] = 0;
				perguntar = 1;
			}
		}
	}
	return perguntar;
}

void MovimentacaoProfessor(int id, PROFESSOR professores[], JOGADOR jogador, FASE *fase)
{
	if(professores[id].cooldown > 0)
		professores[id].cooldown -= GetFrameTime();
	else{
		// Define a direção da movimentação do professor
		DirecaoProfessor(id, professores, jogador);

		// Se o movimento for válido, atualiza a matriz com a nova posição do professor
		if(fase->labirinto.m[professores[id].pos.x + professores[id].movX][professores[id].pos.y] == 0 &&
		   professores[id].pos.x + professores[id].movX != -1 &&
		   professores[id].pos.x + professores[id].movX != fase->labirinto.tamX)
		{
			fase->labirinto.m[professores[id].pos.x][professores[id].pos.y] = 0;
			professores[id].pos.x += professores[id].movX;
		}
		else
	        professores[id].movX = 0;

		if(fase->labirinto.m[professores[id].pos.x][professores[id].pos.y + professores[id].movY] == 0 &&
		   professores[id].pos.y + professores[id].movY != -1 &&
		   professores[id].pos.y + professores[id].movY != fase->labirinto.tamY)
		{
			fase->labirinto.m[professores[id].pos.x][professores[id].pos.y] = 0;
			professores[id].pos.y += professores[id].movY;
		}
		else
	        professores[id].movY = 0;

		fase->labirinto.m[professores[id].pos.x][professores[id].pos.y] = 3;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		professores[id].cooldown = 0.03;
	}
}

// Verifica se o professor está em um bloco adjacente ao jogador
int ProfessorAoLado(int id, PROFESSOR professores[], JOGADOR jogador)
{
	int ao_lado = 0;

	if(professores[id].pos.x+1 == jogador.pos.x ||
	   professores[id].pos.x == jogador.pos.x ||
	   professores[id].pos.x-1 == jogador.pos.x)
	{
		if(professores[id].pos.y+1 == jogador.pos.y ||
		   professores[id].pos.y == jogador.pos.y ||
		   professores[id].pos.y-1 == jogador.pos.y)
		{
		   ao_lado = 1;
		}
	}
	return ao_lado;
}
