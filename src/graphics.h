#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <raylib.h>

void IniciaJanela();
void FechaJanela();

void IniciaQuadro();
void DesenhaQuadro();

void CarregaTexturas(Texture2D texturas[]);
void FechaTexturas(Texture2D texturas[]);

void TelaCheia();

#endif
