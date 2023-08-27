#include "main.h"

int main()
{
	int estado = 0;
	int dificuldade;
	int opcao_selecionada = 0;
	int estado_jogo = 0;

    FASE fase_atual;
	JOGADOR jogador = {0};
	PROFESSOR professores[MAX_PROFESSORES] = {0};
	PERGUNTA perguntas[MAX_PERGUNTAS];
	int num_perguntas;

	// Define a seed dos números aleatórios
	SetRandomSeed(srand(time(NULL)));

	// Carrega as perguntas do arquivo guardando o número de perguntas
	num_perguntas = CarregaPerguntas(perguntas);
	if(!num_perguntas)
		estado = -2;

	IniciaJanela();

	while(!WindowShouldClose() && estado != -1){

		IniciaQuadro();

		//-----------------------------------------------------------------------------------
        // Separa a lógica de cada estado do jogo
        switch(estado){
        //-----------------------------------------------------------------------------------
        // MENU
        case 0:
			estado = MenuInicial(&opcao_selecionada);
            break;
        //------------------------------------------------------------------------------------
        // GANHADORES
        case 1:
			if(MenuGanhadores() == 1)
				estado = 0;
            break;
        //------------------------------------------------------------------------------------
        // INFORMAÇÕES
        case 2:
			if(MenuInformacoes() == 1)
				estado = 0;
            break;
        //------------------------------------------------------------------------------------
		// NOVO JOGO
		case 3:
			dificuldade = (MenuNovoJogo(&opcao_selecionada));
			if(dificuldade != -1){
				if(NovoJogo(&jogador, &fase_atual, professores, dificuldade))
					estado = 5;
				else
					estado = -2;
			}
            break;
		//------------------------------------------------------------------------------------
		// CARREGA JOGO
		case 4:
			//if(CarregaJogo() == 1)
			//	estado = 5;
            break;
		//------------------------------------------------------------------------------------
		// JOGO
		case 5:
			estado = Jogo(&estado_jogo, &jogador, &fase_atual, professores, perguntas, num_perguntas);
            break;
		//------------------------------------------------------------------------------------
		// ERRO AO LIDAR COM ARQUIVOS
		case -2:
			DrawText("Erro ao lidar com arquivos", RES_X/4, RES_Y/2, 30, WHITE);
			if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
				estado = -1;
			break;
        }

		DesenhaQuadro();
    }

	FechaJanela();
    return 0;
}
