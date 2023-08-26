#include "game.h"

#define NUM_OPCOES 5
#define TAM_MAX_OPCOES 18

#define MAX_PERGUNTAS 10
#define NUM_MAX_ALTERNATIVAS 5
#define TAM_MAX_ALTERNATIVAS 18
#define TAM_MAX_ENUNCIADO 50
#define FONTE_ENUNCIADO 30
#define COR_ENUNCIADO WHITE

#define FONTE_INDICADORES 20
#define COR_INDICADORES LIGHTGRAY
#define INDICADORES_Y 400 + (45 - FONTE_INDICADORES*2)/2

#define DURACAO_FASE 300
#define MAX_CREDITOS 10

#define COR_FUNDO (Color){ 0, 0, 0, 128}


int Jogo(int *estado, JOGADOR *jogador, LABIRINTO *labirinto, PROFESSOR professores[]);

int NovoJogo(JOGADOR *jogador, LABIRINTO *labirinto, PROFESSOR professores[], int dificulade);
int CarregaJogo();
void IniciaJogo(LABIRINTO *labirinto, PROFESSOR professores[], SAVE jogo_atual);

int Pause(int *opcao_selecionada);
int Pergunta(int *alt_selecionada);
void DesenhaIndicadores(JOGADOR jogador);
void CalculaPontuacao(JOGADOR jogador);

PERGUNTA perguntas[MAX_PERGUNTAS];
SAVE jogo_atual;

double tempo_jogo;
int opcao_selecionada_pause = 0;
int alt_selecionada;

// Lógica principal do jogo
int Jogo(int *estado, JOGADOR *jogador, LABIRINTO *labirinto, PROFESSOR professores[])
{
	int acao_pause = 5;

	//-----------------------------------------------------------------------------------
    // Separa a lógica de cada estado do jogo
    switch(*estado){
    //-----------------------------------------------------------------------------------
    // JOGO
    case 0:
		tempo_jogo += GetFrameTime();
		jogador->tempo_restante = DURACAO_FASE - tempo_jogo;
		MovimentacaoJogador(jogador, labirinto);
		if(AtualizaProfessores(professores, *jogador, labirinto))
		{
			*estado = 2;
			alt_selecionada = 0;
        }

		DesenhaLabirinto(*labirinto, *jogador);
		DesenhaIndicadores(*jogador);

		if(IsKeyPressed(KEY_ESCAPE)){
			opcao_selecionada_pause = 0;
			*estado = 1;
		}
        break;
    //------------------------------------------------------------------------------------
    // PAUSE
    case 1:
		DesenhaLabirinto(*labirinto, *jogador);
		DesenhaIndicadores(*jogador);

		if(IsKeyPressed(KEY_ESCAPE))
			*estado = 0;
		else
			acao_pause = Pause(&opcao_selecionada_pause);

		switch(acao_pause){
		// Se  opção pressionada foi voltar
		case 0:
			acao_pause = 5;
			*estado = 0;
			break;
		// Se  opção pressionada foi salvar
		case 1:
			acao_pause = 5;
			// ...
			break;
		// Se  opção pressionada foi reiniciar
		case 2:
			acao_pause = 5;
			// ...
			break;
		// Se  opção pressionada foi novo jogo
		case 3:
			*estado = 0;
			opcao_selecionada_pause = 0;
			break;
		}
        break;
	//------------------------------------------------------------------------------------
    // PERGUNTA
    case 2:
		DesenhaLabirinto(*labirinto, *jogador);
		DesenhaIndicadores(*jogador);

		switch(Pergunta(&alt_selecionada)){
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
		// Se nao respondeu ainda
		default:
			*estado = 2;
		}

        break;
	}

	return acao_pause;
}

void IniciaJogo(LABIRINTO *labirinto, PROFESSOR professores[], SAVE jogo_atual)
{
	int i;

	// Posiciona professores no labirinto
	for(i = 0; i < MAX_PROFESSORES * (1 + jogo_atual.dificuldade)/3; i++){
		int posX, posY;
		do{
		posX = GetRandomValue(0, labirinto->tamX - 1);
		posY = GetRandomValue(0, labirinto->tamY - 1);
		} while(labirinto->matriz[posX][posY] != 0);
		professores[i].ativo = 1;
		professores[i].pos.x = posX;
		professores[i].pos.y = posY;
	}
}

// Inicia um jogo com a dificuldade fornecida
int NovoJogo(JOGADOR *jogador, LABIRINTO *labirinto, PROFESSOR professores[], int dificulade)
{
	int i;

	jogador->pos.x = 1;
	jogador->pos.y = 1;
	labirinto->matriz[1][1] = 2;

	perguntas[0].enunciado[0] = 'A';
    perguntas[0].num_alternativas = 2;
    perguntas[0].alternativa_correta = 2;
    perguntas[0].alternativas[0][0] = '1';
    perguntas[0].alternativas[1][0] = '2';

	labirinto->tamX = 100;
	labirinto->tamY = 50;

	for(i = 0; i < labirinto->tamX; i += 3)
		labirinto->matriz[i][0] = 1;

	IniciaJogo(labirinto, professores, (SAVE){0, dificulade});

	return 1;
}

// Inicia um jogo com os parâmetros salvos no arquivo de save
/*
int CarregaJogo()
{
	FILE *arq;
	SAVE save;

    arq = fopen("save.dat","rb");
    if(arq == NULL)
		{}
    else{
		fread(&save, sizeof(SAVE), 1, arq);
	}
 	IniciaJogo(...);

	return -1;

}
*/

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

int Pergunta(int *alt_selecionada)
{
    int resposta;
    int aleatorio;
    int acertou = -1;

    // Com o arquivo de perguntas lido, sortear um numero entre 0 e num_perguntas - 1
    // ...
    // aleatorio = GetRandomValue(0, num_perguntas - 1);
    aleatorio = 0;

	resposta = Selecao(alt_selecionada, perguntas[aleatorio].num_alternativas);

	if(resposta == perguntas[aleatorio].alternativa_correta - 1){
        acertou = 1;
	} else if(resposta != -1){
        acertou = 0;
	}
	// Desenha fundo para a pergunta
	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);

    DrawText(perguntas[aleatorio].enunciado, (RES_X - MeasureText(perguntas[aleatorio].enunciado, FONTE_ENUNCIADO))/2, 50, FONTE_ENUNCIADO, COR_ENUNCIADO);

	DesenhaSelecao(*alt_selecionada, perguntas[aleatorio].num_alternativas, perguntas[aleatorio].alternativas);

    return acertou;
}

void DesenhaIndicadores(JOGADOR jogador)
{
	CalculaPontuacao(jogador);

	DrawText(TextFormat("Vida:\n\n%d", jogador.vida), 10, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Tempo\n\n%4.2f", jogador.tempo_restante), 120, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Créditos:\n\n%d/%d", jogador.creditos, MAX_CREDITOS), 260, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Labirinto:\n\n%d", jogador.labirinto), 440, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Pontuação:\n\n%d", jogador.pontuacao), 620, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
}

void CalculaPontuacao(JOGADOR jogador)
{
	jogador.pontuacao = 10 * MAX_CREDITOS * jogador.labirinto * jogador.creditos / tempo_jogo;
}
