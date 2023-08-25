#include <raylib.h>

#define gameScreenWidth 800
#define gameScreenHeight 450

#define COR_TITULO WHITE
#define COR_OPCOES GRAY
#define COR_OPCAO_SELECIONADA LIGHTGRAY

#define FONTE_TITULO 30
#define FONTE_OPCOES 20
#define ESPACAMENTO (FONTE_OPCOES/2)

#define NUM_OPCOES 5
#define NUM_INFORMACOES 10
#define NUM_GANHADORES 10
#define NUM_DIFICULDADES 3

#define TAM_MAX_OPCOES 18

typedef struct{
	char nome[50];
	int pontuacao;
} GANHADOR;

int MenuInicial(int *opcao_selecionada);
int MenuGanhadores();
int MenuInformacoes();
int MenuNovoJogo(int *opcao_selecionada);

int Selecao(int *opcao_selecionada, int num_opcoes);
void DesenhaSelecao(int opcao_selecionada, int num_opcoes, char opcoes[][TAM_MAX_OPCOES]);

// Tela inicial
int MenuInicial(int *opcao_selecionada)
{
	int menu_acao;

	char titulo[] = {"Os Labirintos do INF"};
    char opcoes[NUM_OPCOES][TAM_MAX_OPCOES] = {"Novo Jogo","Carregar Jogo","Exibir Ganhadores","Informações","Sair"};
    Vector2 pos_titulo = {(gameScreenWidth - MeasureText(titulo, FONTE_TITULO))/2, 50};

	menu_acao = Selecao(opcao_selecionada, NUM_OPCOES);

    DrawText(titulo, pos_titulo.x, pos_titulo.y, FONTE_TITULO, COR_TITULO);
	DesenhaSelecao(*opcao_selecionada, NUM_OPCOES, opcoes);

	// Define o retorno para o main
	switch(menu_acao){
	// NOVO JOGO
	case 0:
		*opcao_selecionada = 1;
		menu_acao = 3;
		break;
	// CARREGAR JOGO
	case 1:
		menu_acao = 4;
		break;
	// EXIBIR GANHADORES
	case 2:
		menu_acao = 1;
		break;
	// INFORMAÇÕES
	case 3:
		menu_acao = 2;
		break;
	// SAIR
	case 4:
		menu_acao = -1;
		break;
	// CONTINUA NO MENU SE NÃO PRESSIONOU NENHUMA OPÇÃO
	default:
		menu_acao = 0;
	}

	return menu_acao;
}

// Tela dos ganhadores
int MenuGanhadores()
{
	int sair = 0;

            /*

            Carregar ganhadores do arquivo...

            */

	if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
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

// Tela das informações
int MenuInformacoes()
{
	int sair = 0;

	if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
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

// Tela da dificuldade do novo jogo
int MenuNovoJogo(int *opcao_selecionada){

	int dificuldade = -1;
	char dificuldades[3][TAM_MAX_OPCOES] = {"Fácil", "Médio", "Difícil"};

	dificuldade = Selecao(opcao_selecionada, NUM_DIFICULDADES);

	DesenhaSelecao(*opcao_selecionada, NUM_DIFICULDADES, dificuldades);

	return dificuldade;
}

// Gera lista de seleção
int Selecao(int *opcao_selecionada, int num_opcoes){

	if(IsKeyPressed(KEY_DOWN) && *opcao_selecionada < num_opcoes - 1)
    	(*opcao_selecionada)++;
    if(IsKeyPressed(KEY_UP) && *opcao_selecionada > 0)
		(*opcao_selecionada)--;
	if(IsKeyPressed(KEY_ENTER))
		return *opcao_selecionada;
	else
		return -1;
}

// Desenha lista de seleção
void DesenhaSelecao(int opcao_selecionada, int num_opcoes, char opcoes[][TAM_MAX_OPCOES]){

	int posX, posY;
	int i;

	for(i = 0; i < num_opcoes; i++){
		posX = (gameScreenWidth - MeasureText(opcoes[i], FONTE_OPCOES))/2;
		posY = (gameScreenHeight - (num_opcoes * FONTE_OPCOES + (num_opcoes - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO);
		if(opcao_selecionada == i)
    		DrawText(opcoes[i], posX, posY, FONTE_OPCOES, COR_OPCAO_SELECIONADA);
		else
     		DrawText(opcoes[i], posX, posY, FONTE_OPCOES, COR_OPCOES);
	}
}



