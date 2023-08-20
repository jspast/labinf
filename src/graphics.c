#include "raylib.h"
#include "menu.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

RenderTexture2D render;

const int gameScreenWidth = 800; // Resolução de renderização do jogo
const int gameScreenHeight = 480;

void IniciaJanela()
{
    const int windowWidth = 800; // Dimensões da janela
    const int windowHeight = 480;

	// Configura a janela para ser redimensionável e com Vsync
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(windowWidth, windowHeight, "Os Labirintos do INF"); // Inicialização da janela
	SetWindowMinSize(800, 480);
    SetTargetFPS(60);

	// Inicializa a textura de renderização, usada para guardar o resultado da renderização
	// para ser possível redimensionar a janela sem alterar a resolução do jogo
    render = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(render.texture, TEXTURE_FILTER_POINT);

	SetExitKey(KEY_NULL); // Garante que nenhuma tecla fecha o jogo diretamente)

    HideCursor();
    InitAudioDevice();
}

void FechaJanela()
{
	UnloadRenderTexture(render); // Descarrega a textura de renderização
	CloseWindow();
	CloseAudioDevice();
}

void IniciaQuadro()
{
    BeginTextureMode(render); // Desenha tudo na textura renderização
    ClearBackground(BLACK); // Limpa a tela e define cor de fundo
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
