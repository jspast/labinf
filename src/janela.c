#include "janela.h"
#include "tipos.h"
#include "constantes.h"
#include <raylib.h>

void IniciaJanela(JANELA *janela)
{
	const int windowWidth = RES_X; // Dimensões da janela
	const int windowHeight = RES_Y;

	// Configura a janela para ser redimensionável e com Vsync
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	janela->escala = 1;

	InitWindow(windowWidth, windowHeight, "Os Labirintos do INF"); // Inicialização da janela

#if defined(PLATFORM_DESKTOP)
	SetWindowMinSize(RES_X, RES_Y);
#endif

	// Inicializa a textura de renderização, usada para guardar o resultado da renderização
	// para ser possível redimensionar a janela sem alterar a resolução do jogo
	janela->render = LoadRenderTexture(RES_X, RES_Y);
	SetTextureFilter(janela->render.texture, TEXTURE_FILTER_POINT);

	SetExitKey(KEY_NULL); // Garante que nenhuma tecla fecha o jogo diretamente

	// Define o icone do jogo no sistema operacional
	Image icon = LoadImage("res/texturas/aluno.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);
	UnloadImage(icon);
}

void FechaJanela(RenderTexture2D *render)
{
	UnloadRenderTexture(*render); // Descarrega a textura de renderização
	CloseWindow();
}

void VerificaTelaCheia()
{
 	if (IsKeyPressed(KEY_F11))
 		ToggleFullscreen();
}

void IniciaQuadro(JANELA *janela)
{
#if defined(PLATFORM_DESKTOP)
	VerificaTelaCheia();
#endif

	BeginTextureMode(janela->render); // Desenha tudo na textura renderização
	ClearBackground(COR_JOGO); // Limpa a tela e define cor de fundo
}

void DesenhaQuadro(JANELA *j)
{
	// Calcula o redimensionamento do framebuffer somente se mudou
	if(IsWindowResized())
		j->escala = MIN((float)GetRenderWidth()/RES_X, (float)GetRenderHeight()/RES_Y);

	EndTextureMode();

	BeginDrawing(); // Inicia o ambiente de desenho na tela
	ClearBackground(BLACK); // Limpa a tela e define cor de fundo (fora da textura de renderização)

	// Desenha a textura de renderização na tela, redimensionada corretamente
	DrawTexturePro(j->render.texture, (Rectangle){ 0.0f, 0.0f, (float)j->render.texture.width, (float)-j->render.texture.height },
				(Rectangle){ (GetRenderWidth() - ((float)RES_X*j->escala))*0.5f, (GetRenderHeight() - ((float)RES_Y*j->escala))*0.5f,
				(float)RES_X*j->escala, (float)RES_Y*j->escala }, (Vector2){ 0, 0 }, 0.0f, WHITE);

	EndDrawing(); // Finaliza o ambiente de desenho na tela
}

