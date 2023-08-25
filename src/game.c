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

#define COR_FUNDO (Color){ 0, 0, 0, 128}

typedef struct{
    double Lifetime;    // Lifetime (seconds)
} Timer;

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
	int creditos_falta;
	int labirinto;
	int pontuacao;
} JOGADOR;

typedef struct{
	POSICAO pos;
	int movX;
	int movY;
	double cooldown;
	// ...
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
void MovimentacaoProfessor();
int ProfessorAoLado();

void DesenhaLabirinto();

int Pause(int *opcao_selecionada);
int Pergunta(int *alt_selecionada);
void DesenhaIndicadores();

void StartTimer(Timer *timer, double lifetime);
bool TimerDone(Timer* timer);
void UpdateTimer(Timer* timer);


Timer timer;
LABIRINTO labirinto;
JOGADOR jogador;
PROFESSOR professor;
PERGUNTA perguntas[MAX_PERGUNTAS];

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
		UpdateTimer(&timer);
		jogador.tempo_restante = timer.Lifetime;
		MovimentacaoJogador();
		MovimentacaoProfessor();
		if(ProfessorAoLado()){
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
            *estado = 0;
            break;
        // Se acertou
		case 1:
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

// Inicia um jogo com a dificuldade fornecida
int NovoJogo(int dificulade)
{
	StartTimer(&timer, DURACAO_FASE);

	// Configura dificuldade
	switch(dificulade){
	// FÁCIL
	case 0:
		break;
	// MÉDIO
	case 1:
		break;
	// DIFÍCIL
	case 2:
		break;
	}

	//ConfiguraLabirinto();


	int i;

	jogador.pos.x = 1;
	jogador.pos.y = 1;
	labirinto.matriz[1][1] = 2;

	professor.pos.x = 10;
	professor.pos.y = 10;
	labirinto.matriz[10][10] = 3;

	perguntas[0].enunciado[0] = 'A';
    perguntas[0].num_alternativas = 2;
    perguntas[0].alternativa_correta = 2;
    perguntas[0].alternativas[0][0] = '1';
    perguntas[0].alternativas[1][0] = '2';

	labirinto.tamX = 100;
	labirinto.tamY = 50;

	for(i = 0; i < labirinto.tamX; i += 3)
		labirinto.matriz[i][0] = 1;

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

void DirecaoProfessor()
{
	int distX;
	int distY;

	// Calcula as distancias X e Y entre o jogador e o professor
	distX = jogador.pos.x - professor.pos.x;
	distY = jogador.pos.y - professor.pos.y;

	// Se a distancia entre o jogador e o professor for grande
	if(sqrt(pow(distX, 2) + pow(distY, 2)) > 10){
        // E se ele ja nao tiver uma direcao, define uma direçao aleatoria para o professor
        if(professor.movX == 0 && professor.movY == 0)
            switch(GetRandomValue(0, 3)){
            case 0:
                professor.movX = 1;
                break;
            case 1:
                professor.movX = -1;
                break;
            case 2:
                professor.movY = 1;
                break;
            case 3:
                professor.movY = -1;
                break;
            }
    }
    // Senao, dadas as distancias X e Y, decide a direcao do movimento perseguindo o aluno
    else{
        professor.movX = 0;
        professor.movY = 0;

        if(abs(distX) >= abs(distY)){
            if(distX > 0)
                professor.movX = 1;
            else
                professor.movX = -1;
        }
        else{
            if(distY > 0)
                professor.movY = 1;
            else
                professor.movY = -1;
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

void MovimentacaoProfessor()
{
	if(professor.cooldown > 0)
		professor.cooldown -= GetFrameTime();
	else{
		// Define a direção da movimentação do professor
		DirecaoProfessor();

		// Se o movimento for válido, atualiza a matriz com a nova posição do professor
		if(labirinto.matriz[professor.pos.x + professor.movX][professor.pos.y] != 1 &&
		   labirinto.matriz[professor.pos.x + professor.movX][professor.pos.y] != 2 &&
		   professor.pos.x + professor.movX != -1 &&
		   professor.pos.x + professor.movX != labirinto.tamX)
		{
			labirinto.matriz[professor.pos.x][professor.pos.y] = 0;
			professor.pos.x += professor.movX;
		}
		else
	        professor.movX = 0;

		if(labirinto.matriz[professor.pos.x][professor.pos.y + professor.movY] != 1 &&
		   labirinto.matriz[professor.pos.x][professor.pos.y + professor.movY] != 2 &&
		   professor.pos.y + professor.movY != -1 &&
		   professor.pos.y + professor.movY != labirinto.tamY)
		{
			labirinto.matriz[professor.pos.x][professor.pos.y] = 0;
			professor.pos.y += professor.movY;
		}
		else
	        professor.movY = 0;

		labirinto.matriz[professor.pos.x][professor.pos.y] = 3;

		// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
		professor.cooldown = 0.03;
	}
}

// Verifica se o professor está em um bloco adjacente ao jogador
int ProfessorAoLado()
{
	if (labirinto.matriz[jogador.pos.x][jogador.pos.y-1] == 3 ||
		labirinto.matriz[jogador.pos.x][jogador.pos.y+1] == 3 ||
		labirinto.matriz[jogador.pos.x-1][jogador.pos.y] == 3 ||
		labirinto.matriz[jogador.pos.x+1][jogador.pos.y] == 3)
		return 1;
	else
		return 0;
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

	if(resposta == perguntas[aleatorio].alternativa_correta)
        acertou = 1;
    else if(resposta != -1)
        acertou = 0;

	// Desenha fundo para a pergunta
	DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, COR_FUNDO);

    DrawText(perguntas[aleatorio].enunciado, (gameScreenWidth - MeasureText(perguntas[aleatorio].enunciado, FONTE_ENUNCIADO))/2, 50, FONTE_ENUNCIADO, COR_ENUNCIADO);

	DesenhaSelecao(*alt_selecionada, perguntas[aleatorio].num_alternativas, perguntas[aleatorio].alternativas);

    return acertou;
}

void DesenhaIndicadores(){

	DrawText(TextFormat("Vida:\n\n%d", jogador.vida), 10, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Tempo\n\n%4.2f", jogador.tempo_restante), 120, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Créditos:\n\n%d", jogador.creditos_falta), 260, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Labirinto:\n\n%d", jogador.labirinto), 440, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
	DrawText(TextFormat("Pontuação:\n\n%d", jogador.pontuacao), 620, INDICADORES_Y, FONTE_INDICADORES, COR_INDICADORES);
}

// Funções de Timer
// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, double lifetime)
{
    if (timer != NULL)
        timer->Lifetime = lifetime;
}

// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != NULL && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
bool TimerDone(Timer* timer)
{
    if (timer != NULL)
        return timer->Lifetime <= 0;

	return false;
}
