#include <raylib.h>
#include "janela.h"
#include "defines.h"

RenderTexture2D render;

const int gameScreenWidth = RES_X; // Resolução de renderização do jogo
const int gameScreenHeight = RES_Y;

void IniciaJanela()
{
	const int windowWidth = RES_X; // Dimensões da janela
	const int windowHeight = RES_Y;

	// Configura a janela para ser redimensionável e com Vsync
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

	InitWindow(windowWidth, windowHeight, "Os Labirintos do INF"); // Inicialização da janela
	SetWindowMinSize(RES_X, RES_Y);

	// Inicializa a textura de renderização, usada para guardar o resultado da renderização
	// para ser possível redimensionar a janela sem alterar a resolução do jogo
	render = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
	SetTextureFilter(render.texture, TEXTURE_FILTER_POINT);

	SetExitKey(KEY_NULL); // Garante que nenhuma tecla fecha o jogo diretamente

	HideCursor();
	//InitAudioDevice();

	// Define o icone do jogo no sistema operacional
	Image icon = LoadImage("res/aluno.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);
	UnloadImage(icon);
}

void FechaJanela()
{
	UnloadRenderTexture(render); // Descarrega a textura de renderização
	CloseWindow();
	//CloseAudioDevice();
}

void AtualizaFPS(int *monitor)
{
	int novo_monitor = GetCurrentMonitor();
	if(novo_monitor != *monitor){
		SetTargetFPS(GetMonitorRefreshRate(novo_monitor));
		*monitor = novo_monitor;
	}
}

void IniciaQuadro()
{
	BeginTextureMode(render); // Desenha tudo na textura renderização
	ClearBackground(COR_TELA); // Limpa a tela e define cor de fundo
}

void DesenhaQuadro()
{
	// Calcula o redimensionamento do framebuffer
	float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

	EndTextureMode();

	BeginDrawing(); // Inicia o ambiente de desenho na tela
	ClearBackground(BLACK); // Limpa a tela e define cor de fundo (fora da textura de renderização)

	// Desenha a textura de renderização na tela, redimensionada corretamente
	DrawTexturePro(render.texture, (Rectangle){ 0.0f, 0.0f, (float)render.texture.width, (float)-render.texture.height },
				(Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
				(float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);

	EndDrawing(); // Finaliza o ambiente de desenho na tela
}

void TelaCheia()
{
 	if (IsKeyPressed(KEY_F11))
	{
 		ToggleFullscreen();
	}
}

void CarregaTexturas(Texture2D texturas[])
{
	ChangeDirectory("res/sprites");
	texturas[0] = LoadTexture("parede.png");
	texturas[1] = LoadTexture("aluno.png");
	texturas[2] = LoadTexture("professor.png");
	texturas[3] = LoadTexture("colega.png");
	texturas[4] = LoadTexture("credito.png");
	texturas[5] = LoadTexture("bomba.png");
	texturas[6] = LoadTexture("saida.png");
	ChangeDirectory(GetApplicationDirectory());

}

void FechaTexturas(Texture2D texturas[])
{
	int i;

	for(i = 0; i < 6; i++)
		UnloadTexture(texturas[i]);
}