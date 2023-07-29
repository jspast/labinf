#include "raylib.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

#define COR_TITULO BLACK
#define COR_OPCOES GRAY
#define COR_OPCAO_SELECIONADA DARKGRAY
#define FONTE_TITULO 30
#define FONTE_OPCOES 20
#define ESPACAMENTO (FONTE_OPCOES/2)
#define NUM_OPCOES 5

int main(void)
{
    char titulo[] = {"Os Labirintos do INF"};
    char opcoes[NUM_OPCOES][18] = {"Novo Jogo","Carregar Jogo","Exibir Ganhadores","Informações","Sair"};
    int opcao_selecionada = 0;
    int menu_acao = 0;
    int estado = 0;
    int i;

    const int windowWidth = 800;    // Dimensões da janela
    const int windowHeight = 600;

    // Configura a janela para ser redimensionável e com Vsync
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(windowWidth, windowHeight, titulo);  // Inicializa janela, com certo tamanho e título
    SetTargetFPS(60);                               // Ajusta a execução do jogo para 60 frames por segundo

    SetWindowMinSize(480, 360);     // Dimensões da renderização do jogo
    const int gameScreenWidth = 480;
    const int gameScreenHeight = 360;

    // Inicializa a textura de renderização, usada para guardar o resultado da renderização para ser possível redimensionar a janela sem alterar a resolução do jogo
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);  // Configura o filtro usado ao redimensionar a textura

    //--------------------------------------------------------------------------------------
    //Laço principal do jogo
    while (!WindowShouldClose() && menu_acao != 5) // Detecta fechamento da janela, ESC ou opção sair
    {
        // Calcula o redimensionamento do framebuffer
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        if(IsKeyPressed(KEY_F11))   // Tela-cheia com F11
            ToggleFullscreen();

        //-----------------------------------------------------------------------------------
        // Separa a lógica de cada estado do jogo
        switch(estado){
        //-----------------------------------------------------------------------------------
        // MENU
        case 0:
            if(IsKeyPressed(KEY_DOWN) && opcao_selecionada < NUM_OPCOES - 1)
                opcao_selecionada++;
            if(IsKeyPressed(KEY_UP) && opcao_selecionada > 0)
                opcao_selecionada--;
            if(IsKeyPressed(KEY_ENTER))
                menu_acao = opcao_selecionada + 1;
            estado = menu_acao;
            break;
        //------------------------------------------------------------------------------------
        case 1:
            break;
        //------------------------------------------------------------------------------------
        case 2:
        }

        //----------------------------------------------------------------------------------
        // Atualiza a representação visual a partir do estado do jogo
        //----------------------------------------------------------------------------------

        // Desenha tudo na textura renderização
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);  // Limpa a tela e define cor de fundo

        //-----------------------------------------------------------------------------------
        // Separa a renderização de cada estado do jogo
        switch(estado){
        //-----------------------------------------------------------------------------------
        // MENU
        case 0:
            DrawText(titulo, (gameScreenWidth - MeasureText(titulo, FONTE_TITULO))/2, (gameScreenHeight - (NUM_OPCOES * FONTE_OPCOES + (NUM_OPCOES - 1) * ESPACAMENTO))/2 - FONTE_TITULO - 2 * ESPACAMENTO, FONTE_TITULO, COR_TITULO);

            for(i = 0; i < NUM_OPCOES; i++)
                if(opcao_selecionada == i)
                    DrawText(opcoes[i], (gameScreenWidth - MeasureText(opcoes[i], FONTE_OPCOES))/2, (gameScreenHeight - (NUM_OPCOES * FONTE_OPCOES + (NUM_OPCOES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO), FONTE_OPCOES, COR_OPCAO_SELECIONADA);
                else
                    DrawText(opcoes[i], (gameScreenWidth - MeasureText(opcoes[i], FONTE_OPCOES))/2, (gameScreenHeight - (NUM_OPCOES * FONTE_OPCOES + (NUM_OPCOES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO), FONTE_OPCOES, COR_OPCOES);
            break;
        //-----------------------------------------------------------------------------------
        case 1:
            break;
        //-----------------------------------------------------------------------------------
        case 2:
        }

        EndTextureMode();

        BeginDrawing(); // Inicia o ambiente de desenho na tela
        ClearBackground(BLACK); // Limpa a tela e define cor de fundo (fora da textura de renderização)

        // Desenha a textura de renderização na tela, redimensionada corretamente
        DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                    (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                    (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        EndDrawing(); // Finaliza o ambiente de desenho na tela
        //----------------------------------------------------------------------------------
    }
    UnloadRenderTexture(target);    // Descarrega a textura de renderização

    CloseWindow(); // Fecha a janela e o contexto OpenGL
    return 0;
}
