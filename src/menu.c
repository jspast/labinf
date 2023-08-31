#include "menu.h"

// Tela inicial
int MenuInicial(int *opcao_selecionada)
{
	int menu_acao;

	char titulo[] = {"Os Labirintos do INF"};
    char opcoes[NUM_OPCOES][TAM_MAX_OPCOES] = {"Novo Jogo","Carregar Jogo","Exibir Ganhadores","Informações","Sair"};
    Vector2 pos_titulo = {(RES_X - MeasureText(titulo, FONTE_TITULO))/2, 50};

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
bool MenuGanhadores()
{
	bool sair = false;

            /*

            Carregar ganhadores do arquivo...

            */

	if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
        sair = true;

	GANHADOR ganhadores[NUM_GANHADORES] = {{"Santiago", 1234567890},{"Ribas", 10101011}};
	int i;

	for(i = 0; i < NUM_GANHADORES; i++){
		DrawText(TextFormat("%d", ganhadores[i].pontuacao),
			50,
			(RES_Y - (NUM_GANHADORES * FONTE_OPCOES + (NUM_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
			FONTE_OPCOES, LIGHTGRAY);
        DrawText(ganhadores[i].nome,
			200,
			(RES_Y - (NUM_GANHADORES * FONTE_OPCOES + (NUM_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
			FONTE_OPCOES, LIGHTGRAY);
	}
	return sair;
}

// Tela das informações
bool MenuInformacoes(Texture2D texturas[])
{
	bool sair = false;

	if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
        sair = true;

	char informacoes[NUM_INFORMACOES][20] = {"Parede", "Aluno", "Professor", "Colega", "Crédito", "Bomba", "Saída"};
	int i;

	for(i = 0; i < NUM_INFORMACOES; i++){
		DrawTexture(texturas[i], RES_X/2 - 32,
					(RES_Y - (NUM_INFORMACOES * FONTE_OPCOES + (NUM_INFORMACOES - 1) * ESPACAMENTO_INFORMACOES))/2 + i * (FONTE_OPCOES + ESPACAMENTO_INFORMACOES),
					WHITE);
		DrawText(informacoes[i], RES_X/2,
				 (RES_Y - (NUM_INFORMACOES * FONTE_OPCOES + (NUM_INFORMACOES - 1) * ESPACAMENTO_INFORMACOES))/2 + i * (FONTE_OPCOES + ESPACAMENTO_INFORMACOES),
				 FONTE_OPCOES, LIGHTGRAY);
	}
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
		posX = (RES_X - MeasureText(opcoes[i], FONTE_OPCOES))/2;
		posY = (RES_Y - (num_opcoes * FONTE_OPCOES + (num_opcoes - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO);
		if(opcao_selecionada == i)
    		DrawText(opcoes[i], posX, posY, FONTE_OPCOES, COR_OPCAO_SELECIONADA);
		else
     		DrawText(opcoes[i], posX, posY, FONTE_OPCOES, COR_OPCOES);
	}
}
