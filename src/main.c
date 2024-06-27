#include "janela.h"
#include "jogo.h"
#include "tipos.h"
#include <raylib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten.h>
#endif

JOGO jogo = {0};
JANELA janela;

void AtualizaQuadro()
{
	AtualizaMusica(&jogo.audio.musica);
	Logica(&jogo);

	IniciaQuadro(&janela);
	Desenha(jogo, &janela);
}

int main()
{
	ChangeDirectory(GetApplicationDirectory());
	ChangeDirectory("../..");

	jogo.estado.menu = MENU;

	CarregaPerguntas(&jogo.perguntas);
	if(jogo.perguntas.total == 0)
		jogo.estado.menu = ERRO_PERGUNTAS;

	CarregaColegas(jogo.colegas);
	CarregaGanhadores(&jogo.ganhadores);

	IniciaJanela(&janela);

	CarregaTexturas(jogo.texturas);
	CarregaAudio(&jogo.audio);

	PlayMusicStream(jogo.audio.musica.musicas[jogo.audio.musica.atual]);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(AtualizaQuadro, 0, 1);
#else

	while(!WindowShouldClose() && jogo.estado.menu != FIM){

		AtualizaQuadro();
	}

#endif

	FechaTexturas(jogo.texturas);
	FechaAudio(&jogo.audio);
	FechaJanela(&janela.render);

	return 0;
}