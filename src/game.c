#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "menu.h"

#define gameScreenWidth 800
#define gameScreenHeight 450

#define MAX_TAMANHO_LABIRINTO 200

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

#define MAX_PROFESSORES 6

#define COR_FUNDO (Color){ 0, 0, 0, 128}

typedef struct{
	int x;
	int y;
} POSICAO;

typedef struct{
	int tamX;
	int tamY;
	POSICAO entrada;
	POSICAO saida;
	char matriz[MAX_TAMANHO_LABIRINTO][MAX_TAMANHO_LABIRINTO];
} LABIRINTO;

typedef struct{
	int fase;
	char dificuldade;
	// ...
} SAVE;

typedef struct{
	POSICAO pos;
	int movX;
	int movY;
	double cooldown;
	int vida;
	double tempo_restante;
	int creditos;
	int labirinto;
	int pontuacao;
} JOGADOR;

typedef struct{
	POSICAO pos;
	int movX;
	int movY;
	double cooldown;
	int ativo;
} PROFESSOR;

typedef struct{
    char enunciado[TAM_MAX_ENUNCIADO];
    int num_alternativas;
    int alternativa_correta;
    char alternativas[5][TAM_MAX_ALTERNATIVAS];
} PERGUNTA;


int Jogo(int *estado);

int NovoJogo(int dificulade);
int CarregaJogo();

int Carregalabirinto();

void MovimentacaoJogador();
int AtualizaProfessores();
void MovimentacaoProfessor(int id);
int ProfessorAoLado(int id);

void DesenhaLabirinto();

int Pause(int *opcao_selecionada);
int Pergunta(int *alt_selecionada);
void DesenhaIndicadores();
void CalculaPontuacao();


LABIRINTO labirinto;
JOGADOR jogador;
PROFESSOR professores[MAX_PROFESSORES];
PERGUNTA perguntas[MAX_PERGUNTAS];
SAVE jogo_atual;

double tempo_jogo;
int opcao_selecionada_pause = 0;
int alt_selecionada;

// Lógica principal do jogo
int Jogo(int *estado)
{
	int acao_pause = 5;

	//-----------------------------------------------------------------------------------
    // Separa a lógica de cada estado do jogo
    switch(*estado){
    //-----------------------------------------------------------------------------------
    // JOGO
    case 0:
		tempo_jogo += GetFrameTime();
		jogador.tempo_restante = DURACAO_FASE - tempo_jogo;
		MovimentacaoJogador();
		if(AtualizaProfessores())
		{
			*estado = 2;
			alt_selecionada = 0;
        }

		DesenhaLabirinto();
		DesenhaIndicadores();

		if(IsKeyPressed(KEY_ESCAPE)){
			opcao_selecionada_pause = 0;
			*estado = 1;
		}
        break;
    //------------------------------------------------------------------------------------
    // PAUSE
    case 1:
		DesenhaLabirinto();
		DesenhaIndicadores();

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
		DesenhaLabirinto();
		DesenhaIndicadores();

		switch(Pergunta(&alt_selecionada)){
		// Se errou
		case 0:
			jogador.vida--;
            *estado = 0;
            break;
        // Se acertou
		case 1:
			jogador.creditos++;
            *estado = 0;
            break;
		// Se nao respondeu ainda
		default:
			*estado = 2;
		}

        // remover professor
        // ...

        break;
	}

	return acao_pause;
}

void IniciaJogo(SAVE jogo_atual)
{
	int i;

	// Posiciona professores no labirinto
	for(i = 0; i < MAX_PROFESSORES * (1 + jogo_atual.dificuldade)/3; i++){
		int posX, posY;
		do{
		posX = GetRandomValue(0, labirinto.tamX - 1);
		posY = GetRandomValue(0, labirinto.tamY - 1);
		} while(labirinto.matriz[posX][posY] != 0);
		professores[i].ativo = 1;
		professores[i].pos.x = posX;
		professores[i].pos.y = posY;
	}
}

// Inicia um jogo com a dificuldade fornecida
int NovoJogo(int dificulade)
{
	int i;

	jogador.pos.x = 1;
	jogador.pos.y = 1;
	labirinto.matriz[1][1] = 2;

	perguntas[0].enunciado[0] = 'A';
    perguntas[0].num_alternativas = 2;
    perguntas[0].alternativa_correta = 2;
    perguntas[0].alternativas[0][0] = '1';
    perguntas[0].alternativas[1][0] = '2';

	labirinto.tamX = 100;
	labirinto.tamY = 50;

	for(i = 0; i < labirinto.tamX; i += 3)
		labirinto.matriz[i][0] = 1;

	IniciaJogo((SAVE){0, dificulade});

	return 1;
}

// Inicia um jogo com os parâmetros salvos no arquivo de save
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

	return -1;
}

void ConfiguraLabirinto(int dificuldade);

//void CompletaLabirinto();


// Carrega enésimo labirinto do arquivo
/*
int CarregaLabirinto(int n)
{
	FILE *arq;

    arq = fopen("labirintos.dat","rb");
    if(arq == NULL)
		num_labirintos = -1; // ERRO
    else{
		while(!feof(arq)){
			num_labirintos++;
			fread(&labirintos[num_labirintos], sizeof(LABIRINTO), 1, arq);
		}
	}
	fclose(arq);

	return num_labirintos;
}
*/
void MovimentacaoJogador()
{
	char ultimo_mov;

	if(jogador.cooldown > 0)
		jogador.cooldown -= GetFrameTime();
	else{
		// Verifica qual foi o último movimento do jogador
		if(jogador.movX != 0)
			ultimo_mov = 'X';

		if(jogador.movY != 0)
			ultimo_mov = 'Y';

		jogador.movX = 0;
		jogador.movY = 0;

		// Calcula o novo movimento do jogador
		if(IsKeyDown(KEY_UP))
			jogador.movY--;
		if(IsKeyDown(KEY_DOWN))
			jogador.movY++;
		if(IsKeyDown(KEY_LEFT))
			jogador.movX--;
		if(IsKeyDown(KEY_RIGHT))
			jogador.movX++;

		// Se pressionou duas teclas, move-se para direção diferente da anterior
		if(jogador.movX != 0 && jogador.movY != 0){
			if(ultimo_mov == 'X')
				jogador.movX = 0;
			if(ultimo_mov == 'Y')
				jogador.movY = 0;
		}

		// Se o movimento for válido, atualiza a matriz com a nova posição do jogador
		if(labirinto.matriz[jogador.pos.x + jogador.movX][jogador.pos.y] != 1 &&
		   jogador.pos.x + jogador.movX != -1 && jogador.pos.x + jogador.movX != labirinto.tamX)
		{
			labirinto.matriz[jogador.pos.x][jogador.pos.y] = 0;
			jogador.pos.x += jogador.movX;
		}
		if(labirinto.matriz[jogador.pos.x][jogador.pos.y + jogador.movY] != 1 &&
		   jogador.pos.y + jogador.movY != -1 && jogador.pos.y + jogador.movY != labirinto.tamY)
		{
			labirinto.matriz[jogador.pos.x][jogador.pos.y] = 0;
			jogador.pos.y += jogador.movY;
		}
		labirinto.matriz[jogador.pos.x][jogador.pos.y] = 2;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		jogador.cooldown = 0.05;
	}
}

void DirecaoProfessor(int id)
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

	if(labirinto.matriz[pos.x + professor.movX][pos.y + professor.movY] == 1)
		return 0;
	else{

		return ProfessorTemVisao();
	}
}
*/

int AtualizaProfessores()
{
	int perguntar = 0;
	int i;

	for(i = 0; i < MAX_PROFESSORES; i++){
		if(professores[i].ativo == 1){
			MovimentacaoProfessor(i);
			if(ProfessorAoLado(i)){
				professores[i].ativo = 0;
				labirinto.matriz[professores[i].pos.x][professores[i].pos.y] = 0;
				perguntar = 1;
			}
		}
	}
	return perguntar;
}

void MovimentacaoProfessor(int id)
{
	if(professores[id].cooldown > 0)
		professores[id].cooldown -= GetFrameTime();
	else{
		// Define a direção da movimentação do professor
		DirecaoProfessor(id);

		// Se o movimento for válido, atualiza a matriz com a nova posição do professor
		if(labirinto.matriz[professores[id].pos.x + professores[id].movX][professores[id].pos.y] == 0 &&
		   professores[id].pos.x + professores[id].movX != -1 &&
		   professores[id].pos.x + professores[id].movX != labirinto.tamX)
		{
			labirinto.matriz[professores[id].pos.x][professores[id].pos.y] = 0;
			professores[id].pos.x += professores[id].movX;
		}
		else
	        professores[id].movX = 0;

		if(labirinto.matriz[professores[id].pos.x][professores[id].pos.y + professores[id].movY] == 0 &&
		   professores[id].pos.y + professores[id].movY != -1 &&
		   professores[id].pos.y + professores[id].movY != labirinto.tamY)
		{
			labirinto.matriz[professores[id].pos.x][professores[id].pos.y] = 0;
			professores[id].pos.y += professores[id].movY;
		}
		else
	        professores[id].movY = 0;

		labirinto.matriz[professores[id].pos.x][professores[id].pos.y] = 3;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		professores[id].cooldown = 0.03;
	}
}

// Verifica se o professor está em um bloco adjacente ao jogador
int ProfessorAoLado(int id)
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

// Renderização da matriz principal do jogo
void DesenhaLabirinto()
{
	int quadrado_tamanho = 16;
    int exibicaoX = 0;
	int exibicaoY = 0;
    int i, j;

    exibicaoX = 50 * (jogador.pos.x / 50);
	exibicaoY = 25 * (jogador.pos.y / 25);

    for(i = exibicaoX ; i < labirinto.tamX && i < exibicaoX + 50; i++){
		for(j = exibicaoY; j < labirinto.tamY && j < exibicaoY + 25; j++){
            switch(labirinto.matriz[i][j]){
            case 0:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, DARKGRAY);
                break;
            case 1:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, WHITE);
                break;
			case 2:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, ORANGE);
                break;
			case 3:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, RED);
                break;
            case 4:
                DrawRectangle((i - exibicaoX) * quadrado_tamanho, (j - exibicaoY) * quadrado_tamanho, quadrado_tamanho, quadrado_tamanho, BLUE);
                break;
            }
		}
    }
}

// Lógica do menu de pause
int Pause(int *opcao_selecionada)
{
	int acao;
	char opcoes_pause[NUM_OPCOES][TAM_MAX_OPCOES] = {"Voltar", "Salvar", "Reiniciar", "Novo Jogo", "Sair"};

	acao = Selecao(opcao_selecionada, NUM_OPCOES);

	// Desenha fundo para as opções de pause
	DrawRectangle(gameScreenWidth/2 - 100, gameScreenHeight/2 - 100, 200, 200, COR_FUNDO);

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

	if(resposta == perguntas[aleatorio].alternativa_correta - 1)
        acertou = 1;
    else if(resposta != -1)
        acertou = 0;

	// Desenha fundo para a pergunta
	DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, COR_FUNDO);

    DrawText(perguntas[aleatorio].enunciado, (gameScreenWidth - MeasureText(perguntas[aleatorio].enunciado, FONTE_ENUNCIADO))/2, 50, FONTE_ENUNCIADO, COR_ENUNCIADO);

	DesenhaSelecao(*alt_selecionada, perguntas[aleatorio].num_alternativas, perguntas[aleatorio].alternativas);

    return acertou;
}

void DesenhaIndicadores()
{
	void CalculaPontuacao();

	DrawText(TextFormat("Vida:\n\n%d", jogador.vida), 10, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Tempo\n\n%4.2f", jogador.tempo_restante), 120, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Créditos:\n\n%d/%d", jogador.creditos, MAX_CREDITOS), 260, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Labirinto:\n\n%d", jogador.labirinto), 440, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Pontuação:\n\n%d", jogador.pontuacao), 620, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
}

void CalculaPontuacao()
{
	jogador.pontuacao = 10 * MAX_CREDITOS * jogador.labirinto * jogador.creditos / tempo_jogo;
}
