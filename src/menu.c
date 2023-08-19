#include "raylib.h"

#define gameScreenWidth 640
#define gameScreenHeight 360
#define COR_TITULO WHITE
#define COR_OPCOES GRAY
#define COR_OPCAO_SELECIONADA LIGHTGRAY
#define FONTE_TITULO 30
#define FONTE_OPCOES 20
#define ESPACAMENTO (FONTE_OPCOES/2)
#define NUM_OPCOES 5
#define NUM_INFORMACOES 10
#define NUM_GANHADORES 10

typedef struct{
	char nome[20];
	int pontuacao;
} GANHADOR;

int opcao_selecionada = 0;

int MenuInicial()
{
	int menu_acao = -1;

	if(IsKeyPressed(KEY_DOWN) && opcao_selecionada < NUM_OPCOES - 1)
    	opcao_selecionada++;
    if(IsKeyPressed(KEY_UP) && opcao_selecionada > 0)
		opcao_selecionada--;
    if(IsKeyPressed(KEY_ENTER))
		menu_acao = opcao_selecionada;

	char titulo[] = {"Os Labirintos do INF"};
    char opcoes[NUM_OPCOES][18] = {"Novo Jogo","Carregar Jogo","Exibir Ganhadores","Informações","Sair"};
    int i;
    Vector2 pos_titulo = {(gameScreenWidth - MeasureText(titulo, FONTE_TITULO))/2, 50};
	Vector2 pos_opcoes;

    DrawText(titulo, pos_titulo.x, pos_titulo.y, FONTE_TITULO, COR_TITULO);

    for(i = 0; i < NUM_OPCOES; i++){
		pos_opcoes.x = (gameScreenWidth - MeasureText(opcoes[i], FONTE_OPCOES))/2;
		pos_opcoes.y = (gameScreenHeight - (NUM_OPCOES * FONTE_OPCOES + (NUM_OPCOES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO);
		if(opcao_selecionada == i)
        	DrawText(opcoes[i], pos_opcoes.x, pos_opcoes.y, FONTE_OPCOES, COR_OPCAO_SELECIONADA);
    	else
            DrawText(opcoes[i], pos_opcoes.x, pos_opcoes.y, FONTE_OPCOES, COR_OPCOES);
	}

	return menu_acao;
}

int MenuGanhadores()
{
	int sair = 0;

            /*

            Carregar ganhadores do arquivo...

            */

	if(IsKeyPressed(KEY_ENTER))
        sair = 1;

	GANHADOR ganhadores[NUM_GANHADORES] = {{"Teste", 20},{"Ok", 10}};
	int i;

	for(i = 0; i < NUM_GANHADORES; i++)
                DrawText(ganhadores[i].nome,
						 (gameScreenWidth - MeasureText(ganhadores[i].nome, FONTE_OPCOES))/2,
						 (gameScreenHeight - (NUM_GANHADORES * FONTE_OPCOES + (NUM_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
						 FONTE_OPCOES, COR_OPCOES);

	return sair;
}

int MenuInformacoes()
{
	int sair = 0;

	if(IsKeyPressed(KEY_ENTER))
        sair = 1;

	char informacoes[NUM_INFORMACOES][20] = {"Parede", "Aluno", "Professor", "Colega", "Bomba", "Relógio", "Crédito", "Coração", "Entrada", "Saída"};
	int i;

	for(i = 0; i < NUM_INFORMACOES; i++)
                DrawText(informacoes[i],
						 (gameScreenWidth - MeasureText(informacoes[i], FONTE_OPCOES))/2,
						 (gameScreenHeight - (NUM_INFORMACOES * FONTE_OPCOES + (NUM_INFORMACOES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
						 FONTE_OPCOES, COR_OPCOES);

	return sair;
}

int Menu()
{
	int acao;

	switch(MenuInicial()){
	case 0:
		acao = 3;
		break;
	case 1:
		acao = 4;
		break;
	case 2:
		acao = 1;
		break;
	case 3:
		acao = 2;
		break;
	case 4:
		acao = -1;
		break;
	default:
		acao = 0;
	}

	return acao;
}
