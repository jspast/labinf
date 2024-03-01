#ifndef DESENHA_H
#define DESENHA_H

#include "game.h"

void DesenhaJogo(FASE fase, JOGADOR jogador, PROFESSOR professores[], Texture2D texturas[]);
POSICAO CalculaExibicao(JOGADOR jogador);
void DesenhaLabirinto(LABIRINTO labirinto, POSICAO exibicao, Texture2D texturas[]);
void DesenhaJogador(JOGADOR jogador, POSICAO exibicao, Texture2D textura);
void DesenhaProfessores(PROFESSOR professores[], FASE fase, POSICAO exibicao, Texture2D textura);
void DesenhaIndicadores(JOGADOR jogador, FASE fase, Texture2D textura);

#endif