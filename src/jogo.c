#include "jogo.h"
#include "janela.h"
#include "tipos.h"
#include "constantes.h"
#include <raylib.h>
#include <stdio.h>

void Logica(JOGO *jogo)
{
	// Atualiza mensagem em menus
	switch(jogo->estado.menu){

	case MENU ... INFORMACOES:
		VerificaPauseMusica(&jogo->audio.musica, &jogo->mensagem);
		AtualizaVolume(&jogo->audio.volume, &jogo->mensagem);
		AtualizaMensagem(&jogo->mensagem);
		break;

	default:
		break;
	}

	//-----------------------------------------------------------------------------------
	// Separa a lógica de cada estado do jogo
	switch(jogo->estado.menu){
	//-----------------------------------------------------------------------------------
	case MENU:
		AtualizaMenuInicial(&jogo->estado, jogo->audio.sons);
		break;
	//------------------------------------------------------------------------------------
	case NOVO_JOGO:
		if (AtualizaMenuDificuldade(&jogo->save.dificuldade, &jogo->estado, jogo->audio.sons)){
			NovoJogo(jogo);
			jogo->estado.menu = M_JOGO;
		}
		break;
	//------------------------------------------------------------------------------------
	case CARREGA_JOGO:
		if(CarregaSave(&jogo->save)){
			IniciaFase(jogo);
		}
		else{
			jogo->mensagem = (MENSAGEM){POS_MENSAGEM_CEN, 3, "Não foi possível carregar jogo salvo", RED};
			jogo->estado.menu = MENU;
		}
		break;
	//------------------------------------------------------------------------------------
	case M_GANHADORES ... INFORMACOES:
		AtualizaMenuSimples(&jogo->estado.menu, jogo->audio.sons);
		break;
	//------------------------------------------------------------------------------------
	case M_JOGO:
		AtualizaJogo(jogo);
		break;
	//------------------------------------------------------------------------------------
	case ERRO_PERGUNTAS ... ERRO_LABIRINTOS:
		if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
			jogo->estado.menu = FIM;
		break;

	case FIM:
		break;
	}
}

void Desenha(JOGO jogo, JANELA *janela)
{
	//-----------------------------------------------------------------------------------
	// Separa o desenho de cada estado do jogo
	switch(jogo.estado.menu){
	//-----------------------------------------------------------------------------------
	case MENU:
		DesenhaMenuInicial(jogo.estado.opcao_selecionada);
		break;
	//------------------------------------------------------------------------------------
	case M_GANHADORES:
		DesenhaMenuGanhadores(jogo.ganhadores);
		break;
	//------------------------------------------------------------------------------------
	case INFORMACOES:
		DesenhaMenuInformacoes(jogo.texturas);
		break;
	//------------------------------------------------------------------------------------
	case NOVO_JOGO:
		DesenhaMenuDificuldade(jogo.estado.opcao_selecionada);
		break;
	//------------------------------------------------------------------------------------
	case CARREGA_JOGO:
		DesenhaMenuInicial(jogo.estado.opcao_selecionada);
		break;
	//------------------------------------------------------------------------------------
	case M_JOGO:
		DesenhaJogo(jogo);
		break;
	//------------------------------------------------------------------------------------
	case ERRO_PERGUNTAS:
		DrawText("Erro ao abrir arquivo perguntas.dat", RES_X/4, RES_Y/2, 30, WHITE);
		break;

	case ERRO_LABIRINTOS:
		DrawText("Erro ao abrir arquivo labirintos.dat", RES_X/4, RES_Y/2, 30, WHITE);
		break;

	case FIM:
		break;
	}

	// Desenha mensagens em menus
	switch (jogo.estado.menu){

	case MENU ... INFORMACOES:
		DesenhaMensagem(jogo.mensagem);
		break;

	default:
		break;
	}

	DesenhaQuadro(janela);
}

void VerificaDerrota(JOGO *jogo)
{
	if(jogo->save.vida <= 0 || jogo->fase.tempo_restante <= 0){
		Derrota(jogo->fase.jogador.pos, jogo->save.fase, jogo->colegas, &jogo->estado, jogo->audio.sons);
	}
}

void AtualizaJogoAtivo(JOGO *jogo)
{
	// Calcula o tempo de jogo
	jogo->save.tempo_total += GetFrameTime();
	jogo->fase.tempo_restante -= GetFrameTime();

	VerificaDerrota(jogo);

	// Verifica se, na movimentação, o jogador encostou num colega
	if(AtualizaJogador(jogo))
	{
		jogo->perguntas.aleatoria = PerguntaAleatoria(jogo->perguntas);
		jogo->estado.opcao_selecionada = 0;
		jogo->estado.jogo = PERGUNTA_COLEGA;
		jogo->fase.jogador.cooldown = 0;
		PlaySound(jogo->audio.sons[S_PERGUNTA]);
	}

	// De acordo com a movimentação analisada, verifica se o jogador passou de fase
	if(jogo->fase.jogador.passou_fase)
	{
		jogo->fase.jogador.passou_fase = false;
		jogo->save.fase++;
		if(jogo->save.fase == NUM_MAX_FASES) {
			PlaySound(jogo->audio.sons[8]);
			jogo->ganhadores.posicao = PosicaoGanhador(jogo->ganhadores, jogo->fase.jogador.pontuacao);
			if(jogo->ganhadores.posicao < MAX_GANHADORES)
				jogo->estado.jogo = DIGITE_NOME;
			else
			 	jogo->estado.jogo = VITORIA;
		}
		else
			IniciaFase(jogo);
	}

	// Verifica se, na atualização, um professor encostou no jogador
	if(AtualizaProfessores(jogo))
	{
		jogo->perguntas.aleatoria = PerguntaAleatoria(jogo->perguntas);
		jogo->estado.opcao_selecionada = 0;
		jogo->estado.jogo = 2;
		PlaySound(jogo->audio.sons[3]);
	}

	if(jogo->fase.jogador.bomba.ativa)
		AtualizaBomba(&jogo->fase.jogador, &jogo->save, &jogo->fase, jogo->audio.sons[S_EXPLOSAO]);
	else if(IsKeyPressed(KEY_SPACE) && jogo->save.bombas > 0)
		JogarBomba(&jogo->fase.jogador, &jogo->save, &jogo->fase);

	if(IsKeyPressed(KEY_ESCAPE)){
		jogo->estado.opcao_selecionada = 0;
		jogo->estado.jogo = 1;
	}
}

// Lógica do menu de pause
void Pause(JOGO *jogo)
{
	if(AtualizaSelecao(&jogo->estado.opcao_selecionada, NUM_OPCOES, jogo->audio.sons))
		jogo->estado.pause = jogo->estado.opcao_selecionada;
	else
	 	jogo->estado.pause = -1;
}

void DesenhaPause(char opcao_selecionada)
{
	char opcoes_pause[NUM_OPCOES][TAM_MAX_OPCOES] = {"Voltar", "Salvar", "Reiniciar", "Menu", "Sair"};

	// Escurece o fundo
	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);

	// Desenha fundo para as opções de pause
	DrawRectangle(RES_X/2 - 80, RES_Y/2 - 90, 160, 180, COR_INTERFACE);

	DesenhaSelecao(opcao_selecionada, NUM_OPCOES, opcoes_pause);
}

void AtualizaJogoPausado(JOGO *jogo)
{
	if(IsKeyPressed(KEY_ESCAPE)){
		jogo->estado.jogo = JOGO_ATIVO;
		jogo->mensagem.tempo_restante = 0;
	}
	else{
		Pause(jogo);

		VerificaPauseMusica(&jogo->audio.musica, &jogo->mensagem);
		AtualizaVolume(&jogo->audio.volume, &jogo->mensagem);
		AtualizaMensagem(&jogo->mensagem);

		switch(jogo->estado.pause){

		case CONTINUA:
			jogo->estado.jogo = JOGO_PAUSADO;
			break;

		case VOLTAR:
			jogo->mensagem.tempo_restante = 0;
			jogo->estado.jogo = JOGO_ATIVO;
			break;

		case SALVAR:
			jogo->estado.jogo = JOGO_PAUSADO;

			if(SalvaSave(jogo->save))
				jogo->mensagem = (MENSAGEM){POS_MENSAGEM_DIR, 2, "Jogo salvo!", GREEN};
			else
				jogo->mensagem = (MENSAGEM){POS_MENSAGEM_DIR, 4, "Erro ao salvar!", RED};
			
			break;

		case REINICIAR:
			jogo->estado.jogo = JOGO_ATIVO;
			NovoJogo(jogo);
			break;

		case P_MENU:
			jogo->estado.menu = MENU;
			jogo->estado.jogo = JOGO_ATIVO;
			jogo->estado.opcao_selecionada = 0;
			break;

		case SAIR:
			jogo->estado.menu = FIM;
		}
	}
}

void AtualizaPerguntaProfessor(JOGO *jogo)
{
	switch(AtualizaPergunta(jogo->perguntas, &jogo->estado.opcao_selecionada, jogo->audio.sons)){
	// Se errou
	case ERROU:
		jogo->save.vida--;
		jogo->estado.jogo = JOGO_ATIVO;
		break;
	// Se acertou
	case ACERTOU:
		IncrementaCreditos(&jogo->fase.jogador, &jogo->save);
		jogo->estado.jogo = JOGO_ATIVO;
		break;
	// Se não respondeu ainda
	default:
		jogo->estado.jogo = PERGUNTA_PROFESSOR;
		break;
	}
}

void AtualizaPerguntaColega(JOGO *jogo)
{
	switch(AtualizaPergunta(jogo->perguntas, &jogo->estado.opcao_selecionada, jogo->audio.sons)){
	// Se errou
	case ERROU:
		// Remove estátua
		jogo->fase.labirinto.m[jogo->fase.jogador.pos.x + jogo->fase.jogador.mov.x]
							  [jogo->fase.jogador.pos.y + jogo->fase.jogador.mov.y] = M_VAZIO;
		jogo->estado.jogo = JOGO_ATIVO;
		RemoveColega((POSICAO){jogo->fase.jogador.pos.x + jogo->fase.jogador.mov.x, jogo->fase.jogador.pos.y + jogo->fase.jogador.mov.y}, 
					 jogo->save.fase, jogo->colegas);
		SalvaColegas(jogo->colegas);
		break;
	// Se acertou
	case ACERTOU:
		// Recompensa por ajudar o colega
		RecompensaColega(jogo->fase.jogador, &jogo->fase);
		jogo->estado.jogo = JOGO_ATIVO;
		RemoveColega((POSICAO){jogo->fase.jogador.pos.x + jogo->fase.jogador.mov.x, jogo->fase.jogador.pos.y + jogo->fase.jogador.mov.y},
					 jogo->save.fase, jogo->colegas);
		SalvaColegas(jogo->colegas);
		break;
	// Se não respondeu ainda
	default:
		jogo->estado.jogo = PERGUNTA_COLEGA;
		break;
	}
}

void AtualizaMenuDerrota(JOGO *jogo)
{
	if(AtualizaSelecao(&jogo->estado.opcao_selecionada, 3, jogo->audio.sons)){
		switch(jogo->estado.opcao_selecionada){
		// Se a opção pressionada foi reiniciar
		case 0:
			jogo->estado.jogo = JOGO_ATIVO;
			jogo->estado.menu = M_JOGO;
			NovoJogo(jogo);
			break;
		// Se a opção pressionada foi menu
		case 1:
			jogo->estado.jogo = JOGO_ATIVO;
			jogo->estado.opcao_selecionada = 0;
			jogo->estado.menu = MENU;
			break;
		// Se a opção pressionada foi sair
		case 2:
			jogo->estado.menu = FIM;
			break;
		// Se não respondeu ainda
		default:
			jogo->estado.jogo = DERROTA;
			break;
		}
	}
}

void DesenhaMenuDerrota(JOGO jogo)
{
	char opcoes[3][TAM_MAX_OPCOES] = {"Reiniciar", "Menu", "Sair"};
	char derrota[8] = "DERROTA";

	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);
	DrawText(derrota, (RES_X - MeasureText(derrota, FONTE_TITULO))/2, 50, FONTE_TITULO, RED);
	DesenhaSelecao(jogo.estado.opcao_selecionada, 3, opcoes);
}

void AtualizaMenuVitoria(JOGO *jogo)
{

	if(AtualizaSelecao(&jogo->estado.opcao_selecionada, 2, jogo->audio.sons))
		switch(jogo->estado.opcao_selecionada){
		// Se a opção pressionada foi menu
		case 0:
			jogo->estado.jogo = JOGO_ATIVO;
			jogo->estado.opcao_selecionada = 0;
			jogo->estado.menu = MENU;
			break;
		// Se a opção pressionada foi sair
		case 1:
			jogo->estado.menu = FIM;
			break;
		// Se não respondeu ainda
		default:
			jogo->estado.jogo = VITORIA;
			break;
		}
}

void DesenhaMenuVitoria(JOGO jogo)
{
	char opcoes[2][TAM_MAX_OPCOES] = {"Menu", "Sair"};
	char vitoria[10] = "VITÓRIA";

	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);
	DrawText(vitoria, (RES_X - MeasureText(vitoria, FONTE_TITULO))/2, 50, FONTE_TITULO, GREEN);
	DesenhaSelecao(jogo.estado.opcao_selecionada, 2, opcoes);
}

void AtualizaMenuNome(JOGO *jogo)
{
	int tecla = 0;
	tecla = GetCharPressed();

	while (tecla > 0) {

		// Tecla de caractere pressionada
		if ((tecla >= 32) && (tecla <= 125) && (jogo->fase.jogador.tam_nome < TAM_MAX_NOME))
		{
			jogo->fase.jogador.nome[jogo->fase.jogador.tam_nome] = (char)tecla;
			jogo->fase.jogador.tam_nome++;
			jogo->fase.jogador.nome[jogo->fase.jogador.tam_nome] = '\0';
		}

		tecla = GetCharPressed();
	}

	// Tecla backspace pressionada
	if ((IsKeyPressed(KEY_BACKSPACE)) && (jogo->fase.jogador.tam_nome > 0)) 
	{
		jogo->fase.jogador.tam_nome--;
		jogo->fase.jogador.nome[jogo->fase.jogador.tam_nome] = '\0';
	}

	if (IsKeyPressed(KEY_ENTER) && (jogo->fase.jogador.tam_nome < TAM_MAX_NOME)) {
		jogo->ganhadores.total++;
		InsereGanhador(&jogo->ganhadores, jogo->ganhadores.posicao, jogo->fase.jogador);
		SalvaGanhadores(jogo->ganhadores);
		jogo->estado.jogo = VITORIA;
	}
}

void DesenhaMenuNome(JOGO jogo)
{
	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);
	DrawText("Digite seu nome:", RES_X/2 - 100, RES_Y/2 - 50, FONTE_OPCOES, LIGHTGRAY);
	DrawText(jogo.fase.jogador.nome, RES_X/2 - 100, RES_Y/2, FONTE_OPCOES, LIGHTGRAY);
}

// Lógica principal do jogo
void AtualizaJogo(JOGO *jogo)
{
	jogo->estado.menu = M_JOGO;

	switch(jogo->estado.jogo){

	case JOGO_ATIVO:
		AtualizaJogoAtivo(jogo);
		break;

	case JOGO_PAUSADO:
		AtualizaJogoPausado(jogo);
		break;

	case PERGUNTA_PROFESSOR:
		AtualizaPerguntaProfessor(jogo);
		break;

	case PERGUNTA_COLEGA:
		AtualizaPerguntaColega(jogo);
		break;

	case DERROTA:
		AtualizaMenuDerrota(jogo);
		break;

	case DIGITE_NOME:
		AtualizaMenuNome(jogo);
		break;

	case VITORIA:
		AtualizaMenuVitoria(jogo);
		break;
	}
}

void DesenhaJogo(JOGO jogo)
{
	DesenhaFase(jogo);

	//-----------------------------------------------------------------------------------
	// Separa o desenho de cada estado do jogo
	switch(jogo.estado.jogo){
	//-----------------------------------------------------------------------------------
	// JOGO
	case JOGO_ATIVO:
		break;
	//------------------------------------------------------------------------------------
	// PAUSE
	case JOGO_PAUSADO:
		DesenhaPause(jogo.estado.opcao_selecionada);
		DesenhaMensagem(jogo.mensagem);
		break;
	//------------------------------------------------------------------------------------
	// PERGUNTA PROFESSOR ou COLEGA
	case PERGUNTA_PROFESSOR ... PERGUNTA_COLEGA:
		DesenhaPergunta(jogo.perguntas, jogo.estado.opcao_selecionada);
		break;
	//------------------------------------------------------------------------------------
	// DERROTA
	case DERROTA:
		DesenhaMenuDerrota(jogo);
		break;
	//------------------------------------------------------------------------------------
	// VITORIA
	case DIGITE_NOME:
		DesenhaMenuNome(jogo);
		break;

	case VITORIA:
		DesenhaMenuVitoria(jogo);
		break;
	}
}

// Inicia um jogo com a dificuldade fornecida
bool NovoJogo(JOGO *jogo)
{
	jogo->save.tempo_total = 0;
	jogo->save.fase = 0;
	jogo->save.total_creditos = 0;
	jogo->save.vida = MAX_VIDA + 2 - jogo->save.dificuldade;
	jogo->save.bombas = 0;

	return IniciaFase(jogo);
}

// Tenta carregar save de arquivo
// Retorna true se conseguir e false caso contrário
bool CarregaSave(SAVE *s)
{
	FILE *arq;

	if(!FileExists(A_SAVE))
		return false;
	arq = fopen(A_SAVE,"rb");
	if(arq == NULL)
		return false;
	else
		fread(s, sizeof(SAVE), 1, arq);

	return true;
}

// Tenta salvar save em arquivo
// Retorna true se conseguir e false caso contrário
bool SalvaSave(SAVE s)
{
	FILE *arq;

	arq = fopen(A_SAVE, "wb");
	if(arq == NULL)
		return false;
	else
		fwrite(&s, sizeof(SAVE), 1, arq);
		
	return true;
}

void Derrota(POSICAO pos, char num_fase, COLEGA colegas[], ESTADO *estado, Sound sons[])
{
	estado->jogo = 4;
	estado->opcao_selecionada = 0;
	PlaySound(sons[9]);
	AdicionaColega(pos, num_fase, colegas);
	SalvaColegas(colegas);
}