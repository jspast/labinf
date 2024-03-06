#ifndef JANELA_H
#define JANELA_H

#include <raylib.h>

void IniciaJanela();
void FechaJanela();

void AtualizaFPS(int *monitor);

void IniciaQuadro();
void DesenhaQuadro();

void TelaCheia();

void CarregaTexturas(Texture2D texturas[]);
void FechaTexturas(Texture2D texturas[]);

#endif
