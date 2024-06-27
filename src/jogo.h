#ifndef GAME_H
#define GAME_H

#include "tipos.h"
#include "constantes.h"
#include <raylib.h>

// Áudio
void CarregaAudio(AUDIO *audio);
void FechaAudio(AUDIO *audio);
void AtualizaMusica(MUSICA *m);
void VerificaPauseMusica(MUSICA *m, MENSAGEM *msg);
void AtualizaVolume(char *v, MENSAGEM *m);

// Texturas
void CarregaTexturas(Texture2D texturas[]);
void FechaTexturas(Texture2D texturas[]);

// Seleção
bool AtualizaSelecao(char *opcao_selecionada, char num_opcoes, Sound sons[]);
void DesenhaSelecao(char opcao_selecionada, char num_opcoes, char opcoes[][TAM_MAX_OPCOES]);

// Mensagem
void AtualizaMensagem(MENSAGEM *m);
void DesenhaMensagem(MENSAGEM m);

void AtualizaMenuInicial(ESTADO *estado, Sound sons[]);
void DesenhaMenuInicial(char opcao_selecionada);

void AtualizaMenuInformacoes(ESTADO_MENU *estado_menu, Sound sons[]);
void DesenhaMenuInformacoes(Texture2D texturas[]);

bool AtualizaMenuDificuldade(char *dificuldade, ESTADO *estado, Sound sons[]);
void DesenhaMenuDificuldade(char opcao_selecionada);

void AtualizaMenuSimples(ESTADO_MENU *estado_menu, Sound sons[]);
void DesenhaMenuGanhadores(GANHADORES g);

void Logica(JOGO *jogo);
void Desenha(JOGO jogo, JANELA *janela);

void AtualizaJogo(JOGO *jogo);
void DesenhaJogo(JOGO jogo);

// Jogo
bool NovoJogo(JOGO *jogo);
void Jogo(JOGO *jogo);
void Pause(JOGO *jogo);

// Save
bool CarregaSave(SAVE *s);
bool SalvaSave(SAVE s);

// Fase
bool CarregaFase(FASE *fase_atual, char num_fase);
bool IniciaFase(JOGO *jogo);
void Derrota(POSICAO pos, char num_fase, COLEGA colegas[], ESTADO *estado, Sound sons[]);
int MenuDerrota(char *opcao_selecionada, Sound sons[]);
int Vitoria(int pontuacao, char *opcao_selecionada, Sound sons[]);

// Jogador
bool AtualizaJogador(JOGO *jogo);
void JogarBomba(JOGADOR *j, SAVE *s, FASE *fase);
void AtualizaBomba(JOGADOR *j, SAVE *save, FASE *fase, Sound som);
void ExplosaoBomba(JOGADOR *j, SAVE *s, FASE *fase, Sound som);
void IncrementaCreditos(JOGADOR *j, SAVE *save);
void CalculaPontuacao(JOGADOR *jogador, SAVE jogo_atual);

// Colega
void RecompensaColega(JOGADOR j, FASE *fase);
void CarregaColegas(COLEGA colegas[]);
void PosicionaColegas(FASE *fase, COLEGA colegas[], char num_fase);
void SalvaColegas(COLEGA colegas[]);
void AdicionaColega(POSICAO pos, char num_labirinto, COLEGA colegas[]);
void RemoveColega(POSICAO pos, char num_labirinto, COLEGA colegas[]);

// Professor
bool AtualizaProfessores(JOGO *jogo);

// Perguntas
void CarregaPerguntas(PERGUNTAS *p);
short PerguntaAleatoria(PERGUNTAS p);
char AtualizaPergunta(PERGUNTAS p, char *alt_selecionada, Sound sons[]);
void DesenhaPergunta(PERGUNTAS p, char alt_selecionada);
void DesenhaTextoCentralizado(const char *text, POSICAO pos, short largura_max, float tam_fonte, Color cor);

// Ganhadores
void CarregaGanhadores(GANHADORES *g);
void SalvaGanhadores(GANHADORES g);
char PosicaoGanhador(GANHADORES g, int pontuacao);
void InsereGanhador(GANHADORES *g, int posicao, JOGADOR j);
bool VerificaGanhador(GANHADORES *g, JOGADOR *j);

// Desenho
void DesenhaFase(JOGO jogo);
POSICAO CalculaExibicao(JOGADOR jogador);
void DesenhaLabirinto(LABIRINTO labirinto, POSICAO exibicao, Texture2D texturas[]);
void DesenhaJogador(JOGADOR jogador, POSICAO exibicao, Texture2D textura);
void DesenhaProfessores(PROFESSOR professores[], FASE fase, POSICAO exibicao, Texture2D textura);
void DesenhaIndicadores(JOGADOR jogador, FASE fase, Texture2D texturas[]);

#endif