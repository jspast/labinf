#ifndef CONSTANTES_H
#define CONSTANTES_H

#define MIN(a, b) ((a)<(b)? (a) : (b))

// Resolução
#define RES_X 800
#define RES_Y 450

// Exibição
#define BLOCOS_X 50
#define BLOCOS_Y 25
#define TAM_BLOCO 16
#define COR_FUNDO (Color){ 0, 0, 0, 160}

// Opções
#define COR_INTERFACE (Color){40, 40, 40, 255}
#define COR_OPCOES (Color){150, 150, 150, 255}
#define COR_OPCAO_SELECIONADA (Color){220, 220, 220, 255}
#define TAM_MAX_OPCOES 50
#define FONTE_OPCOES 20
#define NUM_OPCOES 5
#define COR_TITULO WHITE
#define FONTE_TITULO 30
#define ESPACAMENTO (FONTE_OPCOES/2)

// Informações
#define ESPACAMENTO_INFORMACOES 16
#define NUM_INFORMACOES 10

// Dificuldades
#define NUM_DIFICULDADES 3

// Indicadores
#define FONTE_INDICADORES 20
#define COR_INDICADORES LIGHTGRAY
#define INDICADORES_FUNDO_Y (BLOCOS_Y * TAM_BLOCO)
#define INDICADORES_ALTURA (RES_Y - INDICADORES_FUNDO_Y)
#define INDICADORES_Y (INDICADORES_FUNDO_Y + 5)
#define INDICADORES_Y2 (INDICADORES_Y + FONTE_INDICADORES)

// Labirinto
#define MAX_TAMANHO_LABIRINTO 100
#define COR_JOGO (Color){30, 30, 30, 255}
//#define COR_JOGO (Color){71, 45, 60, 255} // Cor "original" do fundo dos sprites

// Fase
#define NUM_DIFICULDADES 3
#define DURACAO_FASE 300
#define NUM_MAX_FASES 10
#define MIN_CREDITOS 10
#define MAX_CREDITOS 21

// Jogador
#define MAX_VIDA 2

// Ganhadores
#define MAX_GANHADORES 10
#define TAM_MAX_NOME 50

// Perguntas
#define NUM_MAX_ALTERNATIVAS 5
#define TAM_MAX_ALTERNATIVAS 50
#define TAM_MAX_ENUNCIADO 100
#define MAX_PERGUNTAS 100
#define FONTE_ENUNCIADO 20
#define COR_ENUNCIADO WHITE

// Texturas
#define NUM_TEXTURAS 10

// Professores
#define MAX_PROFESSORES 6

// Colegas
#define NUM_COLEGAS 3
#define MAX_COLEGAS (NUM_COLEGAS * NUM_MAX_FASES)

// Movimentação
#define COOLDOWN_JOGADOR 0.05
#define COOLDOWN_PROFESSOR 0.03
#define RAIZ_2 1.41421356237

// Mensagens
#define TAM_MAX_MENSAGEM 50
#define MENSAGEM_Y 350
#define POS_MENSAGEM_DIR {RES_X * 3/4, MENSAGEM_Y}
#define POS_MENSAGEM_CEN {RES_X/2, MENSAGEM_Y}
#define POS_MENSAGEM_ESQ {RES_X/4, MENSAGEM_Y}
#define FONTE_MENSAGEM 20

// Matriz
#define M_VAZIO 0
#define M_PAREDE 1
#define M_ALUNO 2
#define M_PROFESSOR 3
#define M_COLEGA 4
#define M_CREDITO 5
#define M_CORACAO 6
#define M_BOMBA_INATIVA 7
#define M_BOMBA_ATIVA 8
#define M_RELOGIO 9
#define M_SAIDA 10

// Texturas
#define T_PAREDE (M_PAREDE - 1)
#define T_ALUNO (M_ALUNO - 1)
#define T_PROFESSOR (M_PROFESSOR - 1)
#define T_COLEGA (M_COLEGA - 1)
#define T_CREDITO (M_CREDITO - 1)
#define T_CORACAO (M_CORACAO - 1)
#define T_BOMBA_INATIVA (M_BOMBA_INATIVA - 1)
#define T_BOMBA_ATIVA (M_BOMBA_ATIVA - 1)
#define T_RELOGIO (M_RELOGIO - 1)
#define T_SAIDA (M_SAIDA - 1)

// Áudio
#define MAX_MUSICAS 99
#define ETAPA_VOLUME 1
#define VOLUME_PADRAO 5

// Sons
#define NUM_SONS 16
#define S_OPCAO 0
#define S_SELECAO 1
#define S_TRANSICAO 2
#define S_COLISAO 3
#define S_PERGUNTA 4
#define S_ACERTO 5
#define S_ERRO 6
#define S_ITEM 7
#define S_VITORIA 8
#define S_DERROTA 9
#define S_PASSO_0 10
#define S_PASSO_1 11
#define S_PASSO_2 12
#define S_PASSO_3 13
#define S_PASSO_4 14
#define S_EXPLOSAO 15

// Arquivos
#define A_COLEGAS "data/colegas.dat"
#define A_LABIRINTOS "res/labirintos.dat"
#define A_GANHADORES "data/ganhadores.dat"
#define A_PERGUNTAS "res/perguntas.txt"
#define A_SAVE "data/save.dat"

#endif