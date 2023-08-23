#include <stdio.h>
#include <raylib.h>
#include "menu.h"

#define gameScreenWidth 800
#define gameScreenHeight 480

#define MAX_TAMANHO_LABIRINTO 200
#define MAX_LABIRINTOS 20

#define NUM_OPCOES 5
#define TAM_MAX_OPCOES 18

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
	int posX;
	int posY;
	// ...
} JOGADOR;

int NovoJogo(int dificulade);
int CarregaJogo();
int CarregaLabirintos();
void MovimentacaoJogador();
void Jogo(int *estado);
void DesenhaLabirinto();
int Pause(int *opcao_selecionada);
int ProfessorAoLado();

LABIRINTO labirintos[MAX_LABIRINTOS];
JOGADOR jogador;

int opcao_selecionada_pause = 0;

// Inicia um jogo com a dificuldade fornecida
int NovoJogo(int dificulade)
{
	int i;

	jogador.posX = 1;
	jogador.posY = 1;
	labirintos[0].matriz[1][1] = 2;
	labirintos[0].matriz[10][10] = 3;

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
	if (IsKeyDown(KEY_UP) && jogador.posY > 0 && labirintos[0].matriz[jogador.posX][jogador.posY-1] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posY--;
	}  if (IsKeyDown(KEY_DOWN) && jogador.posY < labirintos[0].tamanho-1 && labirintos[0].matriz[jogador.posX][jogador.posY+1] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posY++;
	}  if (IsKeyDown(KEY_LEFT) && jogador.posX > 0 && labirintos[0].matriz[jogador.posX-1][jogador.posY] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posX--;
	}  if (IsKeyDown(KEY_RIGHT) && jogador.posX < labirintos[0].tamanho-1 && labirintos[0].matriz[jogador.posX+1][jogador.posY] != 1){
		labirintos[0].matriz[jogador.posX][jogador.posY] = 0;
		jogador.posX++;
	}

	labirintos[0].matriz[jogador.posX][jogador.posY] = 2;
	WaitTime(0.05);
}

// Lógica principal do jogo
void Jogo(int *estado)
{
	//-----------------------------------------------------------------------------------
    // Separa a lógica de cada estado do jogo
    switch(*estado){
    //-----------------------------------------------------------------------------------
    // JOGO
    case 0:
		MovimentacaoJogador();
		DesenhaLabirinto();
		if(IsKeyPressed(KEY_ESCAPE)){
			opcao_selecionada_pause = 0;
			*estado = 1;
		}
		if(ProfessorAoLado())
			*estado = 2;
        break;
    //------------------------------------------------------------------------------------
    // PAUSE
    case 1:
		DesenhaLabirinto();
		if(IsKeyPressed(KEY_ESCAPE))
			*estado = 0;
		else
			*estado = Pause(&opcao_selecionada_pause);
        break;
	//------------------------------------------------------------------------------------
    // PERGUNTA
    case 2:
		DesenhaLabirinto();

        break;
	}
}

void DesenhaLabirinto()
{
	int quadrado_tamanho = 16;
    int exibicaoX = 0;
	int exibicaoY = 0;
    int i, j;

    exibicaoX = 50 * (jogador.posX / 50);
	exibicaoY = 25 * (jogador.posY / 25);

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

int Pause(int *opcao_selecionada)
{
	int acao;
	char opcoes_pause[NUM_OPCOES][TAM_MAX_OPCOES] = {"Voltar", "Salvar", "Reiniciar", "Novo Jogo", "Sair"};

	// Define o retorno para função Jogo
	switch(Selecao(opcao_selecionada, NUM_OPCOES)){
    // VOLTAR
	case 0:
		acao = 0;
		break;
	// SALVAR
	case 1:

		break;
	// REINICIAR
	case 2:

		break;
	// NOVO JOGO
	case 3:

		break;
	// SAIR
	case 4:

		break;
	// CONTINUA NO PAUSE SE NÃO PRESSIONOU NENHUMA OPÇÃO
	default:
		acao = 1;
	}

	DrawRectangle(gameScreenWidth/2 - 100, gameScreenHeight/2 - 100, 200, 200, BLACK);	// Desenha fundo para as opções de pause
	DesenhaSelecao(*opcao_selecionada, NUM_OPCOES, opcoes_pause);

	return acao;
}

int ProfessorAoLado()
{
	if (labirintos[0].matriz[jogador.posX][jogador.posY-1] == 3 ||
		labirintos[0].matriz[jogador.posX][jogador.posY+1] == 3 ||
		labirintos[0].matriz[jogador.posX-1][jogador.posY] == 3 ||
		labirintos[0].matriz[jogador.posX+1][jogador.posY] == 3)
		return 1;
	else
		return 0;
}


