#include "jogo.h"
#include "tipos.h"
#include "constantes.h"
#include <raylib.h>
#include <stdio.h>

// Preenche a fase atual com os dados do arquivo de labirintos
bool CarregaFase(FASE *fase_atual, char num_fase)
{
	bool carregou = false;

	int ajuste_lab;
	FILE *arqlab;

	arqlab = fopen(A_LABIRINTOS, "rb");
	if(arqlab != NULL){
		ajuste_lab = sizeof(LABIRINTO)*num_fase;
		fseek(arqlab, ajuste_lab, 0);

		fread(&fase_atual->labirinto, sizeof(LABIRINTO), 1, arqlab);

		fclose(arqlab);
		carregou = true;
	}

	return carregou;
}

// Ajusta os parâmetros da fase conforme número e dificuldade escolhida
void AjustaFase(FASE *f, SAVE s)
{
	f->max_tempo = DURACAO_FASE - (s.fase + s.dificuldade)*20;
	f->tempo_restante = f->max_tempo;
	f->max_professores = MAX_PROFESSORES * (1 + s.dificuldade)/3;
	f->min_creditos = MIN_CREDITOS + (s.fase + s.dificuldade);
}

// Posiciona e reseta o jogador
void IniciaJogador(FASE *f, SAVE s)
{
	f->tempo_restante = f->max_tempo;
	f->jogador.creditos = 0;
	f->jogador.pos.x = f->labirinto.entrada.x;
	f->jogador.pos.y = f->labirinto.entrada.y;
}

// Coloca os blocos de saída do labirinto
void PosicionaSaida(LABIRINTO *l)
{
	for(int i = l->saida.inic_x; i <= l->saida.fim_x; i++){
		for(int j = l->saida.inic_y; j <= l->saida.fim_y; j++){
			l->m[i][j] = M_SAIDA;
		}
	}
}

// Posiciona colegas salvos no labirinto
// Se não for possível, deixa de armazenar o colega
// Retorna o número de colegas posicionados
int PosicionaColegasSalvos(FASE *fase, COLEGA colegas[], char num_fase)
{
	int posicionados = 0;

	for(int i = num_fase * NUM_COLEGAS; i < num_fase * NUM_COLEGAS + NUM_COLEGAS; i++){

		if(colegas[i].ocupado){

			if (fase->labirinto.m[colegas[i].pos.x][colegas[i].pos.y] == M_VAZIO){

				fase->labirinto.m[colegas[i].pos.x][colegas[i].pos.y] = M_COLEGA;
				posicionados++;
			}
			else {
				RemoveColega(colegas[i].pos, num_fase, colegas);
			}
		}
	}

	return posicionados;
}

// Devolve uma posição vazia aleatória no labirinto
POSICAO PosicaoVaziaAleatoria(FASE fase)
{
	POSICAO pos;
		
	do{
		pos.x = GetRandomValue(0, fase.labirinto.tam.x - 1);
		pos.y = GetRandomValue(0, fase.labirinto.tam.y - 1);
		
	} while(fase.labirinto.m[pos.x][pos.y] != M_VAZIO);

	return pos;
}

// Posiciona colegas de modo aleatório na fase
void PosicionaColegasAleatorio(FASE *fase, char num_colegas)
{
	for(int i = 0; i < num_colegas; i++){

		POSICAO pos = PosicaoVaziaAleatoria(*fase);

		fase->labirinto.m[pos.x][pos.y] = M_COLEGA;
	}
}

// Posiciona os colegas na matriz
void PosicionaColegas(FASE *fase, COLEGA colegas[], char num_fase)
{
	char posicionados = PosicionaColegasSalvos(fase, colegas, num_fase);

	PosicionaColegasAleatorio(fase, NUM_COLEGAS - posicionados);
}

// Posiciona professores no labirinto
void PosicionaProfessores(FASE *f)
{
	for(int i = 0; i < f->max_professores; i++){

		POSICAO pos = PosicaoVaziaAleatoria(*f);

		f->professores[i].ativo = true;
		f->professores[i].pos = pos;
	}
}

// Posiciona créditos no labirinto
void PosicionaCreditos(FASE *f)
{
	for(int i = 0; i < f->min_creditos; i++){

		POSICAO pos = PosicaoVaziaAleatoria(*f);

		f->labirinto.m[pos.x][pos.y] = M_CREDITO;
	}
}

// Inicia uma fase com as informações do SAVE
bool IniciaFase(JOGO *j)
{
	PlaySound(j->audio.sons[2]);

	if(!CarregaFase(&j->fase, j->save.fase))
		return false;
	
	else{
		AjustaFase(&j->fase, j->save);

		IniciaJogador(&j->fase, j->save);

		PosicionaSaida(&j->fase.labirinto);
		PosicionaColegas(&j->fase, j->colegas, j->save.fase);
		PosicionaProfessores(&j->fase);
		PosicionaCreditos(&j->fase);

		j->mensagem.tempo_restante = 0;
		j->estado.menu = M_JOGO;

		return true;
	}
}
