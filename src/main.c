#include "raylib.h"
#include "menu.h"
#include "graphics.h"



#define gameScreenWidth 480
#define gameScreenHeight 360

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
        }

		DesenhaQuadro();
		//------------------------------------------------------------------------------------
    }

	FechaJanela();
    return 0;
}
