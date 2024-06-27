#ifndef TIPOS_H
#define TIPOS_H

#include "constantes.h"
#include <raylib.h>

// Estados:

typedef enum{
	MENU = -1,
	NOVO_JOGO,
	CARREGA_JOGO,
	M_GANHADORES,
	INFORMACOES,
	FIM,
	M_JOGO,
	ERRO_PERGUNTAS,
	ERRO_LABIRINTOS
} ESTADO_MENU;

typedef enum{
	JOGO_ATIVO = 0,
	JOGO_PAUSADO,
	PERGUNTA_PROFESSOR,
	PERGUNTA_COLEGA,
	DERROTA,
	DIGITE_NOME,
	VITORIA
} ESTADO_JOGO;

typedef enum{
	CONTINUA = -1,
	VOLTAR = 0,
	SALVAR,
	REINICIAR,
	P_MENU,
	SAIR
} ESTADO_PAUSE;

typedef enum{
	ERROU = 0,
	ACERTOU,
	PENSANDO
} ESTADO_PERGUNTA;

typedef struct{
	ESTADO_MENU menu;
	ESTADO_JOGO jogo;
	ESTADO_PAUSE pause;
	ESTADO_PERGUNTA pergunta;
	char opcao_selecionada;
} ESTADO;


// Arquivos:

typedef struct{
	char fase;
	char dificuldade;
	short total_creditos;
	float tempo_total;
	char vida;
	char bombas;
} SAVE;

typedef struct{
	short x;
	short y;
} POSICAO;

typedef POSICAO TAMANHO;

typedef struct{
	short inic_x, fim_x;
	short inic_y, fim_y;
} SAIDA;

typedef struct{
	TAMANHO tam;
	POSICAO entrada;
	SAIDA saida;
	char m[MAX_TAMANHO_LABIRINTO][MAX_TAMANHO_LABIRINTO];
} LABIRINTO;

typedef struct{
	POSICAO pos;
	bool ocupado;
} COLEGA;

typedef struct{
	char nome[TAM_MAX_NOME];
	short pontuacao;
} GANHADOR;


// Fase:

typedef struct{
	POSICAO pos;
	float tempo;
	bool ativa;
} BOMBA;

typedef struct{
	char x;
	char y;
} MOVIMENTO;

typedef struct{
	POSICAO pos;
	MOVIMENTO mov;
	MOVIMENTO ult_mov;
	bool mov_continuo;
	float cooldown;
	char creditos;
	short pontuacao;
	BOMBA bomba;
	char nome[TAM_MAX_NOME];
	int tam_nome;
	bool passou_fase;
} JOGADOR;

typedef struct{
	POSICAO pos;
	MOVIMENTO mov;
	float cooldown;
	bool ativo;
} PROFESSOR;

typedef struct{
	float tempo;
	float tempo_restante;
	int max_tempo;
	int max_professores;
	char min_creditos;
	LABIRINTO labirinto;
	JOGADOR jogador;
	PROFESSOR professores[MAX_PROFESSORES];
} FASE;


// Jogo:

typedef struct{
	GANHADOR ganhadores[MAX_GANHADORES];
	int total;
	int posicao;
} GANHADORES;

typedef struct{
	char enunciado[TAM_MAX_ENUNCIADO];
	int num_alternativas;
	int alternativa_correta;
	char alternativas[NUM_MAX_ALTERNATIVAS][TAM_MAX_ALTERNATIVAS];
} PERGUNTA;

typedef struct{
	PERGUNTA perguntas[MAX_PERGUNTAS];
	int total;
	int aleatoria;
} PERGUNTAS;

typedef struct{
	Music musicas[MAX_MUSICAS];
	int atual;
	int total;
	bool pausado;
} MUSICA;

typedef struct{
	MUSICA musica;
	Sound sons[NUM_SONS];
	char volume;
} AUDIO;

typedef struct{
	POSICAO pos;
	float tempo_restante;
	char texto[TAM_MAX_MENSAGEM];
	Color cor;
} MENSAGEM;

typedef struct{
	ESTADO estado;
	SAVE save;
	FASE fase;
	PERGUNTAS perguntas;
	COLEGA colegas[MAX_COLEGAS];
	GANHADORES ganhadores;
	MENSAGEM mensagem;
	Texture2D texturas[NUM_TEXTURAS];
	AUDIO audio;
} JOGO;

typedef struct{
	RenderTexture2D render;
	bool redimensionada;
} JANELA;

#endif