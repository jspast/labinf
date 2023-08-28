#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "main.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

#define RES_X 800
#define RES_Y 450

void IniciaJanela();
void FechaJanela();

void IniciaQuadro();
void DesenhaQuadro();

void CarregaTexturas(Texture2D texturas[]);

#endif
