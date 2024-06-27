#include "constantes.h"
#include "jogo.h"
#include "tipos.h"
#include <raylib.h>

// Gera lista de seleção
bool AtualizaSelecao(char *opcao_selecionada, char num_opcoes, Sound sons[])
{
	bool selecionado = false;

	if(IsKeyPressed(KEY_DOWN) && *opcao_selecionada < num_opcoes - 1){
		(*opcao_selecionada)++;
		PlaySound(sons[0]);
	}
	else if(IsKeyPressed(KEY_UP) && *opcao_selecionada > 0){
		(*opcao_selecionada)--;
		PlaySound(sons[0]);
	}
	else if(IsKeyPressed(KEY_ENTER)){
		PlaySound(sons[1]);
		selecionado = true;
	}
	
	return selecionado;
}

// Desenha lista de seleção
void DesenhaSelecao(char opcao_selecionada, char num_opcoes, char opcoes[][TAM_MAX_OPCOES])
{
	POSICAO pos;

	for(int i = 0; i < num_opcoes; i++){
		pos.x = (RES_X - MeasureText(opcoes[i], FONTE_OPCOES))/2;
		pos.y = (RES_Y - (num_opcoes * FONTE_OPCOES + (num_opcoes - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO);
		if(opcao_selecionada == i)
			DrawText(opcoes[i], pos.x, pos.y, FONTE_OPCOES, COR_OPCAO_SELECIONADA);
		else
	 		DrawText(opcoes[i], pos.x, pos.y, FONTE_OPCOES, COR_OPCOES);
	}
}

// Tela inicial
void AtualizaMenuInicial(ESTADO *estado, Sound sons[])
{
	if(AtualizaSelecao(&estado->opcao_selecionada, NUM_OPCOES, sons)){
		estado->menu = estado->opcao_selecionada;

		if(estado->menu == NOVO_JOGO)
			estado->opcao_selecionada = 1;
	}
}

void DesenhaMenuInicial(char opcao_selecionada)
{
	char titulo[] = {"Os Labirintos do INF"};
	char opcoes[NUM_OPCOES][TAM_MAX_OPCOES] = {"Novo Jogo","Carregar Jogo","Exibir Ganhadores","Informações","Sair"};
	Vector2 pos_titulo = {(RES_X - MeasureText(titulo, FONTE_TITULO))/2.0, 50};

	DrawText(titulo, pos_titulo.x, pos_titulo.y, FONTE_TITULO, COR_TITULO);
	DesenhaSelecao(opcao_selecionada, NUM_OPCOES, opcoes);
}

// Tela da dificuldade do novo jogo
bool AtualizaMenuDificuldade(char *dificuldade, ESTADO *estado, Sound sons[])
{
	bool escolhido = false;

	if(AtualizaSelecao(&estado->opcao_selecionada, NUM_DIFICULDADES, sons)){
		*dificuldade = estado->opcao_selecionada;
		escolhido = true;
	}

	return escolhido;
}

void DesenhaMenuDificuldade(char opcao_selecionada)
{
	char dificuldades[3][TAM_MAX_OPCOES] = {"Fácil", "Médio", "Difícil"};

	DrawText("Escolha a dificuldade", RES_X / 2 - MeasureText("Escolha a dificuldade", FONTE_TITULO) / 2, 50, FONTE_TITULO, WHITE);

	DesenhaSelecao(opcao_selecionada, NUM_DIFICULDADES, dificuldades);
}

// Tela das informações
void AtualizaMenuSimples(ESTADO_MENU *estado_menu, Sound sons[])
{
	if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)){
		*estado_menu = MENU;
		PlaySound(sons[S_SELECAO]);
	}
}

void DesenhaMenuInformacoes(Texture2D texturas[])
{
	char informacoes[NUM_INFORMACOES][20] = {"Parede", "Aluno", "Professor", "Colega", "Crédito", "Coração", "Bomba Inativa", "Bomba Ativa", "Relógio", "Saída"};

	DrawText("Informações", RES_X / 2 - MeasureText("Informações", FONTE_TITULO) / 2, 25, FONTE_TITULO, WHITE);

	for(int i = 0; i < NUM_INFORMACOES; i++){
		DrawTexture(texturas[i], 25,
					(RES_Y - (NUM_INFORMACOES * FONTE_OPCOES + (NUM_INFORMACOES - 1) * ESPACAMENTO_INFORMACOES))/2 + i * (FONTE_OPCOES + ESPACAMENTO_INFORMACOES),
					WHITE);
		DrawText(informacoes[i], 25 + 32,
				 (RES_Y - (NUM_INFORMACOES * FONTE_OPCOES + (NUM_INFORMACOES - 1) * ESPACAMENTO_INFORMACOES))/2 + i * (FONTE_OPCOES + ESPACAMENTO_INFORMACOES),
				 FONTE_OPCOES, LIGHTGRAY);
	}

	DesenhaTextoCentralizado("Use as setas para se mover pelos labirintos, coletando os créditos necessários para passar pela saída. Professores te seguirão para fazer perguntas, que podem te tirar vida ou dar crédito. Colegas que ficaram pelos labirintos podem ser ajudados para conseguir recompensas! Bombas são jogadas com espaço e destroem paredes, mas cuidado com o raio da explosão! Complete todos os 10 labirintos para ganhar!", (POSICAO){500, 80}, 510, FONTE_OPCOES, LIGHTGRAY);

	DesenhaTextoCentralizado("Em menus, use as setas para direita e esquerda para controlar o volume e a tecla M para desativar a música.", (POSICAO){475, 350}, 625, FONTE_OPCOES, LIGHTGRAY);
}

void DesenhaMenuGanhadores(GANHADORES g)
{
	DrawText("Ganhadores", RES_X / 2 - MeasureText("Ganhadores", FONTE_TITULO) / 2, 25, FONTE_TITULO, WHITE);

	for(int i = 0; i < g.total; i++){
		DrawText(TextFormat("%d", i + 1), 50, 
				 (RES_Y - (MAX_GANHADORES * FONTE_OPCOES + (MAX_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
				  FONTE_OPCOES, LIGHTGRAY);
		DrawText(g.ganhadores[i].nome, 100,
			(RES_Y - (MAX_GANHADORES * FONTE_OPCOES + (MAX_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
			FONTE_OPCOES, LIGHTGRAY);
		DrawText(TextFormat("%d", g.ganhadores[i].pontuacao), RES_X - 100,
			(RES_Y - (MAX_GANHADORES * FONTE_OPCOES + (MAX_GANHADORES - 1) * ESPACAMENTO))/2 + i * (FONTE_OPCOES + ESPACAMENTO),
			FONTE_OPCOES, LIGHTGRAY);
		
	}

	if(g.total == 0)
		DrawText("Nenhum ganhador ainda :(", RES_X / 2 - MeasureText("Nenhum ganhador ainda :(", FONTE_OPCOES) / 2, 100, FONTE_OPCOES, WHITE);
}

void AtualizaMensagem(MENSAGEM *m)
{
	if(m->tempo_restante >= 0)
		m->tempo_restante -= GetFrameTime();
}

void DesenhaMensagem(MENSAGEM m)
{
	int tam_texto = MeasureText(m.texto, FONTE_MENSAGEM);

	POSICAO pos;

	pos.x = m.pos.x - tam_texto / 2;
	pos.y = m.pos.y - FONTE_MENSAGEM / 2;

	if(m.tempo_restante > 0){
		DrawRectangle(pos.x - FONTE_MENSAGEM/2, pos.y - FONTE_MENSAGEM/2, 
					  tam_texto + FONTE_MENSAGEM, FONTE_MENSAGEM * 2, COR_INTERFACE);
		DrawText(m.texto, pos.x, pos.y, FONTE_MENSAGEM, m.cor);
	}
}