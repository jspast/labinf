#include <raylib.h>
#include "menu.h"
#include "janela.h"
#include "game.h"
#include "defines.h"

int main()
{
	int monitor = -1;
	int dificuldade;
	int opcao_selecionada = 0;
	int estado_jogo = 0;

	ESTADO estado = MENU;
	FASE fase_atual;
	JOGADOR jogador = {0};
	PROFESSOR professores[MAX_PROFESSORES] = {0};
	PERGUNTA perguntas[MAX_PERGUNTAS];
	SAVE jogo_atual;

	// Carrega as perguntas do arquivo guardando o número de perguntas
	int num_perguntas = CarregaPerguntas(perguntas);
	if(!num_perguntas)
		estado = ERRO;

	IniciaJanela();

	Texture2D texturas[NUM_TEXTURAS];
	CarregaTexturas(texturas);

	while(!WindowShouldClose() && estado != FIM){

		AtualizaFPS(&monitor);

		IniciaQuadro();

		// Verifica se é para mudar para tela cheia
		TelaCheia();

		//-----------------------------------------------------------------------------------
		// Separa a lógica de cada estado do jogo
		switch(estado){
		//-----------------------------------------------------------------------------------
		case MENU:
			estado = MenuInicial(&opcao_selecionada);
			break;
		//------------------------------------------------------------------------------------
		case GANHADORES:
			if(MenuGanhadores() == 1)
				estado = MENU;
			break;
		//------------------------------------------------------------------------------------
		case INFORMACOES:
			if(MenuInformacoes(texturas) == 1)
				estado = MENU;
			break;
		//------------------------------------------------------------------------------------
		case NOVO_JOGO:
			dificuldade = (MenuNovoJogo(&opcao_selecionada));
			if(dificuldade != -1){
				if(NovoJogo(&jogador, &fase_atual, professores, dificuldade, &jogo_atual))
					estado = JOGO;
				else
					estado = ERRO;
			}
			break;
		//------------------------------------------------------------------------------------
		case CARREGA_JOGO:
			if(CarregaJogo(&jogador, &fase_atual, professores, &jogo_atual))
				estado = JOGO;
			else
				estado = ERRO;
			break;
		//------------------------------------------------------------------------------------
		case JOGO:
			estado = Jogo(&estado_jogo, &jogador, &fase_atual, professores, perguntas, num_perguntas, texturas, &jogo_atual);
			break;
		//------------------------------------------------------------------------------------
		case ERRO:
			DrawText("Erro ao lidar com arquivos", RES_X/4, RES_Y/2, 30, WHITE);
			if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
				estado = FIM;
			break;

		case FIM:
			break;
		}

		DesenhaQuadro();
	}

	FechaTexturas(texturas);
	FechaJanela();

	return 0;
}
