#include "raylib.h"
#define COR_TITULO BLACK
#define COR_OPCOES DARKGRAY
#define FONTE_TITULO 50
#define FONTE_OPCOES 30
#define ESPACAMENTO FONTE_OPCOES/2
#define NUM_TEXTOS 4

int main(void)
{
    const int screenWidth = 800;    // Largura
    const int screenHeight = 450;   // Altura
    char texto[NUM_TEXTOS][50] = {"Os Labiritos do INF","Novo Jogo","Carregar Jogo","Sair"};
    int i;

    InitWindow(screenWidth, screenHeight, "Os Labirintos do INF");  //Inicializa janela, com certo tamanho e título
    SetTargetFPS(60);                                               // Ajusta a execução do jogo para 60 frames por segundo


    //--------------------------------------------------------------------------------------
    //Laço principal do jogo
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {



        //----------------------------------------------------------------------------------
        // Atualiza a representação visual a partir do estado do jogo
        //----------------------------------------------------------------------------------
        BeginDrawing();//Inicia o ambiente de desenho na tela
        ClearBackground(RAYWHITE);//Limpa a tela e define cor de fundo

        DrawText(texto[0], (screenWidth - MeasureText(texto[0], FONTE_TITULO))/2, 80, FONTE_TITULO, COR_TITULO);

        for(i = 1; i < NUM_TEXTOS; i++)
            DrawText(texto[i], (screenWidth - MeasureText(texto[i], FONTE_OPCOES))/2, (screenHeight - (NUM_TEXTOS * FONTE_OPCOES + (NUM_TEXTOS - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO), FONTE_OPCOES, COR_OPCOES);

        EndDrawing();//Finaliza o ambiente de desenho na tela
        //----------------------------------------------------------------------------------
    }
    CloseWindow(); // Fecha a janela e o contexto OpenGL
    return 0;
}
