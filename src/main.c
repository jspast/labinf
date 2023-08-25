#include <raylib.h>
#include <time.h>
#include "menu.h"
#include "graphics.h"
#include "game.h"

int main()
{
	int estado = 0;
	int dificuldade;
	int opcao_selecionada = 0;
	int estado_jogo = 0;

	SetRandomSeed(srand(time(NULL)));

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
				NovoJogo(dificuldade);
				estado = 5;
			}
            break;
		//------------------------------------------------------------------------------------
		// CARREGA JOGO
		case 4:
			if(CarregaJogo() == 1)
				estado = 5;
            break;
		//------------------------------------------------------------------------------------
		// JOGO
		case 5:
			estado = Jogo(&estado_jogo);
            break;
		//------------------------------------------------------------------------------------
		// ERRO AO LIDAR COM ARQUIVOS
		case -2:
			FechaJanela();
			break;
        }

		DesenhaQuadro();
    }

	FechaJanela();
    return 0;
}
