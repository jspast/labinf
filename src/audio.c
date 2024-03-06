#include "audio.h"
#include "defines.h"
#include <raylib.h>

void CarregaAudio(Music musica[], Sound sons[])
{
	InitAudioDevice();

	ChangeDirectory("res/musicas");

	musica[0] = LoadMusicStream("menu.ogg");

	ChangeDirectory(GetApplicationDirectory());

	ChangeDirectory("res/sons");

	sons[0] = LoadSound("opcao.ogg");
	sons[1] = LoadSound("selecao.ogg");
	sons[2] = LoadSound("transicao.ogg");
	sons[3] = LoadSound("colisao.ogg");
	sons[4] = LoadSound("pergunta.ogg");
	sons[5] = LoadSound("acerto.ogg");
	sons[6] = LoadSound("erro.ogg");
	sons[7] = LoadSound("moeda.ogg");
	sons[8] = LoadSound("vitoria.ogg");
	sons[9] = LoadSound("derrota.ogg");
	sons[10] = LoadSound("passo_0.ogg");
	sons[11] = LoadSound("passo_1.ogg");
	sons[12] = LoadSound("passo_2.ogg");
	sons[13] = LoadSound("passo_3.ogg");
	sons[14] = LoadSound("passo_4.ogg");

	ChangeDirectory(GetApplicationDirectory());

}

void FechaAudio(Music musicas[], Sound sons[])
{
	int i;

	for(i = 0; i < NUM_MUSICAS; i++)
		UnloadMusicStream(musicas[i]);
	
	for(i = 0; i < NUM_SONS; i++)
		UnloadSound(sons[i]);
	
	CloseAudioDevice();
}