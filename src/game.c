#include <raylib.h>
#include <stdio.h>
#include "defines.h"
#include "game.h"
#include "menu.h"
#include "desenha.h"
#include "ganhadores.h"

bool IniciaFase(FASE *fase, JOGADOR *jogador, PROFESSOR professores[], SAVE jogo_atual);
bool SalvarJogo(SAVE jogo_atual);

int Pause(int *opcao_selecionada);
void CalculaPontuacao(JOGADOR *jogador);
void RecompensaColega(JOGADOR *jogador);

int Derrota(int *opcao_selecionada);
int Vitoria(int pontuacao, int *opcao_selecionada);

int CarregaColegas(POSICAO colegas[]);
void PosicionaColegas(FASE *fase);
bool SalvaEstatua(JOGADOR jogador);

int opcao_selecionada = 0;
int pergunta_aleatoria;

// Lógica principal do jogo
ESTADO Jogo(int *estado, JOGADOR *jogador, FASE *fase, PROFESSOR professores[], PERGUNTA perguntas[], int num_perguntas, Texture2D texturas[], SAVE *jogo_atual, int *num_letras, char nome[], GANHADOR ganhadores[])
{
	int acao_pause = 5;
	bool passar_fase = false;

	//-----------------------------------------------------------------------------------
	// Separa a lógica de cada estado do jogo
	switch(*estado){
	//-----------------------------------------------------------------------------------
	// JOGO
	case 0:
		// Calcula o tempo de jogo
		jogador->tempo_total += GetFrameTime();
		jogador->tempo_restante -= GetFrameTime();

		if(jogador->vida <= 0 || jogador->tempo_restante <= 0){
			*estado = 4;
			opcao_selecionada = 0;
			//SalvaEstatua(*jogador);
		}

		if(IsKeyPressed(KEY_SPACE) && jogador->bombas > 0)
			JogarBomba(jogador, fase);

		// Verifica se, na movimentação, o jogador encostou num colega
		if(MovimentacaoJogador(jogador, fase, &passar_fase))
		{
			pergunta_aleatoria = GetRandomValue(0, num_perguntas - 1);
			opcao_selecionada = 0;
			*estado = 3;
		}

		// De acordo com a movimentação analisada, verifica se o jogador passou de fase
		if(passar_fase)
		{
			jogo_atual->fase++;
			if(jogo_atual->fase == NUM_MAX_FASES)
				*estado = 5;
			else
				IniciaFase(fase, jogador, professores, *jogo_atual);
		}

		// Verifica se, na atualização, um professor encostou no jogador
		if(AtualizaProfessores(professores, *jogador, fase))
		{
			pergunta_aleatoria = GetRandomValue(0, num_perguntas - 1);
			opcao_selecionada = 0;
			*estado = 2;
		}

		CalculaPontuacao(jogador);
		DesenhaJogo(*fase, *jogador, professores, texturas);

		if(IsKeyPressed(KEY_ESCAPE)){
			opcao_selecionada = 0;
			*estado = 1;
		}
		break;
	//------------------------------------------------------------------------------------
	// PAUSE
	case 1:
		DesenhaJogo(*fase, *jogador, professores, texturas);

		if(IsKeyPressed(KEY_ESCAPE))
			*estado = 0;
		else
			acao_pause = Pause(&opcao_selecionada);

		switch(acao_pause){
		// Se a opção pressionada foi voltar
		case 0:
			acao_pause = 5;
			*estado = 0;
			break;
		// Se a opção pressionada foi salvar
		case 1:
			acao_pause = 5;
			SalvarJogo(*jogo_atual); // Por enquanto não verifica se conseguiu salvar
			*estado = 1;
			break;
		// Se a opção pressionada foi reiniciar
		case 2:
			*estado = 0;
			acao_pause = 5;
			IniciaFase(fase, jogador, professores, *jogo_atual);
			break;
		// Se a opção pressionada foi novo jogo
		case 3:
			*estado = 0;
			opcao_selecionada = 0;
			break;
		}
		break;
	//------------------------------------------------------------------------------------
	// PERGUNTA PROFESSOR
	case 2:
		DesenhaJogo(*fase, *jogador, professores, texturas);

		switch(Pergunta(perguntas, pergunta_aleatoria, &opcao_selecionada)){
		// Se errou
		case 0:
			jogador->vida--;
			*estado = 0;
			break;
		// Se acertou
		case 1:
			jogador->creditos++;
			*estado = 0;
			break;
		// Se não respondeu ainda
		default:
			*estado = 2;
			break;
		}
		break;
	//------------------------------------------------------------------------------------
	// PERGUNTA COLEGA
	case 3:
		DesenhaJogo(*fase, *jogador, professores, texturas);

		switch(Pergunta(perguntas, pergunta_aleatoria, &opcao_selecionada)){
		// Se errou
		case 0:
			*estado = 0;
			break;
		// Se acertou
		case 1:
			// Recompensa por ajudar o colega
			// bomba, relógio ou vida
			RecompensaColega(jogador);
			*estado = 0;
			break;
		// Se não respondeu ainda
		default:
			*estado = 3;
			break;
		}
		break;
	//------------------------------------------------------------------------------------
	// DERROTA
	case 4:
		DesenhaJogo(*fase, *jogador, professores, texturas);

		switch(Derrota(&opcao_selecionada)){
		// Se a opção pressionada foi reiniciar
		case 0:
			*estado = 0;
			acao_pause = 5;
			IniciaFase(fase, jogador, professores, *jogo_atual);
			break;
		// Se a opção pressionada foi novo jogo
		case 1:
			*estado = 0;
			opcao_selecionada = 0;
			acao_pause = 3;
			break;
		// Se a opção pressionada foi sair
		case 2:
			acao_pause = -1;
			break;
		// Se não respondeu ainda
		default:
			*estado = 4;
			break;
		}
		break;
	//------------------------------------------------------------------------------------
	// VITORIA
	case 5:
		DesenhaJogo(*fase, *jogador, professores, texturas);

		if(VerificaGanhador(ganhadores, jogador->pontuacao, num_letras, nome))
			*estado = 6;
		else
			*estado = 5;
		break;

	case 6:
		DesenhaJogo(*fase, *jogador, professores, texturas);

		switch(Vitoria(jogador->pontuacao, &opcao_selecionada)){
		// Se a opção pressionada foi novo jogo
		case 0:
			*estado = 0;
			opcao_selecionada = 0;
			acao_pause = 3;
			break;
		// Se a opção pressionada foi sair
		case 1:
			acao_pause = -1;
			break;
		// Se não respondeu ainda
		default:
			*estado = 6;
			break;
		}
		break;
	}

	return acao_pause;
}

// Inicia uma fase com as informações do SAVE
bool IniciaFase(FASE *fase, JOGADOR *jogador, PROFESSOR professores[], SAVE jogo_atual)
{
	int i, j;

	if(!CarregaFase(fase, jogo_atual.fase))
		return false;
	else{
		// Ajusta os parâmetros da fase conforme número e dificuldade escolhida
		fase->max_tempo = DURACAO_FASE - (jogo_atual.fase + jogo_atual.dificuldade)*20;
		fase->max_professores = MAX_PROFESSORES * (1 + jogo_atual.dificuldade)/3;
		fase->min_creditos = MIN_CREDITOS + (jogo_atual.fase + jogo_atual.dificuldade);

		// Coloca os blocos de saída do labirinto
		for(i = fase->labirinto.saida.inic_x; i <= fase->labirinto.saida.fim_x; i++){
			for(j = fase->labirinto.saida.inic_y; j <= fase->labirinto.saida.fim_y; j++){
				fase->labirinto.m[i][j] = 7;
			}
		}

		// Posiciona e reseta o jogador
		jogador->tempo_restante = fase->max_tempo;
		jogador->creditos = 0;
		jogador->vida = MAX_VIDA;
		jogador->pos.x = fase->labirinto.entrada.x;
		jogador->pos.y = fase->labirinto.entrada.y;
		jogador->labirinto = jogo_atual.fase;
		fase->labirinto.m[jogador->pos.x][jogador->pos.y] = 2;

		// Posiciona professores no labirinto
		for(i = 0; i < fase->max_professores; i++){
			int posX, posY;
			do{
				posX = GetRandomValue(0, fase->labirinto.tamX - 1);
				posY = GetRandomValue(0, fase->labirinto.tamY - 1);
			} while(fase->labirinto.m[posX][posY] != 0);
			professores[i].ativo = 1;
			professores[i].pos.x = posX;
			professores[i].pos.y = posY;
		}

		// Posiciona colegas no labirinto
		PosicionaColegas(fase);

		// Posiciona créditos no labirinto
		for(i = 0; i < fase->min_creditos; i++){
			int posX, posY;
			do{
				posX = GetRandomValue(0, fase->labirinto.tamX - 1);
				posY = GetRandomValue(0, fase->labirinto.tamY - 1);
			} while(fase->labirinto.m[posX][posY] != 0);
			fase->labirinto.m[posX][posY] = 5;
		}

		return true;
	}
}

// Posiciona os colegas na matriz com base no arquivo de estátuas
void PosicionaColegas(FASE *fase)
{
	int i;
	int num_colegas = 0;
	POSICAO colegas[NUM_COLEGAS];

	// Carrega do arquivo (DISABILITADO)
	//num_colegas = CarregaColegas(colegas);

	for(i = 0; i < num_colegas; i++)
		fase->labirinto.m[colegas[i].x][colegas[i].y] = 4;

	for(i = 0; i < NUM_COLEGAS - num_colegas; i++){
		int posX, posY;
		do{
			posX = GetRandomValue(0, fase->labirinto.tamX - 1);
			posY = GetRandomValue(0, fase->labirinto.tamY - 1);
		} while(fase->labirinto.m[posX][posY] != 0);
		fase->labirinto.m[posX][posY] = 4;
		}
}

// Carrega colegas do arquivo (DISABILITADO)
int CarregaColegas(POSICAO colegas[])
{
	int num_colegas = 0;

	FILE *arq;

	arq = fopen("estatuas.dat", "rb");
	if(arq == NULL)
		num_colegas = 0;
	else{
		while(!feof(arq)){
			fread(&colegas[num_colegas], sizeof(POSICAO), 1, arq);
			num_colegas++;
		}
		fclose(arq);
	}

	return num_colegas - 1;
}

// Inicia um jogo com a dificuldade fornecida
bool NovoJogo(JOGADOR *jogador, FASE *fase, PROFESSOR professores[], int dificuldade, SAVE *jogo_atual)
{
	jogo_atual->fase = 0;
	jogo_atual->dificuldade = dificuldade;

	return IniciaFase(fase, jogador, professores, *jogo_atual);
}

// Inicia um jogo com os parâmetros salvos no arquivo de save
bool CarregaJogo(JOGADOR *jogador, FASE *fase, PROFESSOR professores[], SAVE *jogo_atual)
{
	FILE *arq;

	arq = fopen("save.dat","rb");
	if(arq == NULL)
		return false;
	else
		fread(jogo_atual, sizeof(SAVE), 1, arq);

	return IniciaFase(fase, jogador, professores, *jogo_atual);

}

bool SalvarJogo(SAVE jogo_atual)
{
	FILE *arq;

	arq = fopen("save.dat", "wb");
	if(arq == NULL)
		return false;
	else
		fwrite(&jogo_atual, sizeof(SAVE), 1, arq);

	return true;
}

// Lógica do menu de pause
int Pause(int *opcao_selecionada)
{
	int acao;
	char opcoes_pause[NUM_OPCOES][TAM_MAX_OPCOES] = {"Voltar", "Salvar", "Reiniciar", "Novo Jogo", "Sair"};

	acao = Selecao(opcao_selecionada, NUM_OPCOES);

	// Desenha fundo para as opções de pause
	DrawRectangle(RES_X/2 - 100, RES_Y/2 - 100, 200, 200, COR_FUNDO);

	DesenhaSelecao(*opcao_selecionada, NUM_OPCOES, opcoes_pause);

	// Define o retorno para função Jogo
	switch(acao){
	// VOLTAR
	case 0:
		break;
	// SALVAR
	case 1:
		break;
	// REINICIAR
	case 2:
		break;
	// NOVO JOGO
	case 3:
		acao = 3;
		break;
	// SAIR
	case 4:
		acao = -1;
		break;
	// CONTINUA NO PAUSE SE NÃO PRESSIONOU NENHUMA OPÇÃO
	default:
		acao = 5;
	}

	return acao;
}


// Calcula a pontuação total acumulada pelo aluno
void CalculaPontuacao(JOGADOR *jogador)
{
	int termo2 = 0;

	if(jogador->tempo_total / 60 != 0)
		termo2 = jogador->total_creditos / (jogador->tempo_total / 60);
	
	jogador->pontuacao = 10 * MAX_CREDITOS * jogador->labirinto + termo2;
}

// Define uma recompensa aleatória ao jogador por salvar o colega
void RecompensaColega(JOGADOR *jogador){

	switch(GetRandomValue(0, 2)){
		case 0:
			jogador->vida++;
			break;
		case 1:
			jogador->tempo_restante += 20;
			break;
		case 2:
			jogador->bombas++;
			break;
	}
}

// Tela de derrota
int Derrota(int *opcao_selecionada)
{
	int acao = -1;
	char opcoes[3][TAM_MAX_OPCOES] = {"Reiniciar", "Novo Jogo", "Sair"};
	char derrota[8] = "DERROTA";

	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);
	DrawText(derrota, (RES_X - MeasureText(derrota, FONTE_TITULO))/2, 50, FONTE_TITULO, RED);

	acao = Selecao(opcao_selecionada, 3);
	DesenhaSelecao(*opcao_selecionada, 3, opcoes);

	return acao;
}

// Tela de vitória
int Vitoria(int pontuacao, int *opcao_selecionada)
{
	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);

	int acao = -1;
	char opcoes[2][TAM_MAX_OPCOES] = {"Novo Jogo", "Sair"};
	char vitoria[10] = "VITÓRIA";

	DrawText(vitoria, (RES_X - MeasureText(vitoria, FONTE_TITULO))/2, 50, FONTE_TITULO, GREEN);

	acao = Selecao(opcao_selecionada, 2);
	DesenhaSelecao(*opcao_selecionada, 2, opcoes);

	return acao;
}

// Salva posição da morte para arquivo de estátuas (DISABILITADO)
bool SalvaEstatua(JOGADOR jogador)
{
	FILE *arq;

	arq = fopen("estatuas.dat", "ab");
	if(arq == NULL)
		return false;
	else
		fwrite(&jogador.pos, sizeof(POSICAO), 1, arq);

	return true;
}

