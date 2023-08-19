#include "raylib.h"
#include "menu.h"
#include "graphics.h"
#include "game.h"

int main()
{
	int estado = 0;

	IniciaJanela();

	while(!WindowShouldClose() && estado != -1){

		IniciaQuadro();

		//-----------------------------------------------------------------------------------
        // Separa a lógica de cada estado do jogo
        switch(estado){
        //-----------------------------------------------------------------------------------
        // MENU
        case 0:
			estado = Menu();
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
			if(NovoJogo() == 1)
				estado = 5;
            break;
		//------------------------------------------------------------------------------------
		// CARREGA JOGO
		case 4:
			if(CarregaJogo() == 1)
				estado = 5;
            break;
		//------------------------------------------------------------------------------------
		// CARREGA JOGO
		case 5:
			Jogo();
            break;
		//------------------------------------------------------------------------------------
		// ERRO
		case -1:
			FechaJanela();
			break;
        }

		DesenhaQuadro();
    }

	FechaJanela();
    return 0;
}
