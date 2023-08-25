#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "menu.h"

#define gameScreenWidth 800
#define gameScreenHeight 480

#define MAX_TAMANHO_LABIRINTO 200
#define MAX_LABIRINTOS 20

#define NUM_OPCOES 5
#define TAM_MAX_OPCOES 18

#define MAX_PERGUNTAS 10
#define NUM_MAX_ALTERNATIVAS 5
#define TAM_MAX_ALTERNATIVAS 18
#define TAM_MAX_ENUNCIADO 50
#define FONTE_ENUNCIADO 30
#define COR_ENUNCIADO WHITE

#define COR_FUNDO (Color){ 0, 0, 0, 128}

typedef struct{
	int x;
	int y;
} POSICAO;

typedef struct{
	int tamanho;
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
	// ...
} JOGADOR;

typedef struct{
	POSICAO pos;
	int movX;
	int movY;
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

int CarregaLabirintos();

void MovimentacaoJogador();
void MovimentacaoProfessor();
int ProfessorAoLado();

void DesenhaLabirinto();

int Pause(int *opcao_selecionada);


LABIRINTO labirintos[MAX_LABIRINTOS];
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
		MovimentacaoJogador();
		MovimentacaoProfessor();
		if(ProfessorAoLado()){
			*estado = 2;
			alt_selecionada = 0;
        }

		DesenhaLabirinto();
		if(IsKeyPressed(KEY_ESCAPE)){
			opcao_selecionada_pause = 0;
			*estado = 1;
		}
        break;
    //------------------------------------------------------------------------------------
    // PAUSE
    case 1:
		DesenhaLabirinto();

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
		switch(Pergunta(&alt_selecionada)){
		// Se errou
		case 0:
            *estado == 0;
            break;
        // Se acertou
		case 1:
            *estado == 0;
            break;
		// Se nao respondeu ainda
		default:
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
	int i;

	jogador.pos.x = 1;
	jogador.pos.y = 1;
	labirintos[0].matriz[1][1] = 2;

	professor.pos.x = 10;
	professor.pos.y = 10;
	labirintos[0].matriz[10][10] = 3;

	perguntas[0].enunciado[0] = 'A';
    perguntas[0].num_alternativas = 2;
    perguntas[0].alternativa_correta = 2;
    perguntas[0].alternativas[0][0] = '1';
    perguntas[0].alternativas[1][0] = '2';

	labirintos[0].tamanho = 100;

	for(i = 0; i < labirintos[0].tamanho; i += 3)
		labirintos[0].matriz[i][0] = 1;

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

// Carrega labirintos do arquivo
int CarregaLabirintos()
{
	int num_labirintos = -1;
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

void MovimentacaoJogador()
{
	char ultimo_mov;

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
	if(labirintos[0].matriz[jogador.pos.x + jogador.movX][jogador.pos.y] != 1 &&
	   jogador.pos.x + jogador.movX != -1 && jogador.pos.x + jogador.movX != labirintos[0].tamanho)
	{
		labirintos[0].matriz[jogador.pos.x][jogador.pos.y] = 0;
		jogador.pos.x += jogador.movX;
	}
	if(labirintos[0].matriz[jogador.pos.x][jogador.pos.y + jogador.movY] != 1 &&
	   jogador.pos.y + jogador.movY != -1 && jogador.pos.y + jogador.movY != labirintos[0].tamanho)
	{
		labirintos[0].matriz[jogador.pos.x][jogador.pos.y] = 0;
		jogador.pos.y += jogador.movY;
	}
	labirintos[0].matriz[jogador.pos.x][jogador.pos.y] = 2;

	// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
	WaitTime(0.05);
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

	if(labirintos[0].matriz[pos.x + professor.movX][pos.y + professor.movY] == 1)
		return 0;
	else{

		return ProfessorTemVisao();
	}
}
*/

void MovimentacaoProfessor()
{
	// Define a direção da movimentação do professor
	DirecaoProfessor();

	// Se o movimento for válido, atualiza a matriz com a nova posição do professor
	if(labirintos[0].matriz[professor.pos.x + professor.movX][professor.pos.y] != 1 &&
	   labirintos[0].matriz[professor.pos.x + professor.movX][professor.pos.y] != 2 &&
	   professor.pos.x + professor.movX != -1 &&
	   professor.pos.x + professor.movX != labirintos[0].tamanho)
	{
		labirintos[0].matriz[professor.pos.x][professor.pos.y] = 0;
		professor.pos.x += professor.movX;
	}
	else
        professor.movX = 0;

	if(labirintos[0].matriz[professor.pos.x][professor.pos.y + professor.movY] != 1 &&
	   labirintos[0].matriz[professor.pos.x][professor.pos.y + professor.movY] != 2 &&
	   professor.pos.y + professor.movY != -1 &&
	   professor.pos.y + professor.movY != labirintos[0].tamanho)
	{
		labirintos[0].matriz[professor.pos.x][professor.pos.y] = 0;
		professor.pos.y += professor.movY;
	}
	else
        professor.movY = 0;

	labirintos[0].matriz[professor.pos.x][professor.pos.y] = 3;

	// Controla a velocidade da movimentação pelo tempo de espera entre os movimentos
	WaitTime(0.05);
}

// Verifica se o professor está em um bloco adjacente ao jogador
int ProfessorAoLado()
{
	if (labirintos[0].matriz[jogador.pos.x][jogador.pos.y-1] == 3 ||
		labirintos[0].matriz[jogador.pos.x][jogador.pos.y+1] == 3 ||
		labirintos[0].matriz[jogador.pos.x-1][jogador.pos.y] == 3 ||
		labirintos[0].matriz[jogador.pos.x+1][jogador.pos.y] == 3)
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

    for(i = exibicaoX ; i < labirintos[0].tamanho && i < exibicaoX + 50; i++){
		for(j = exibicaoY; j < labirintos[0].tamanho && j < exibicaoY + 25; j++){
            switch(labirintos[0].matriz[i][j]){
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
