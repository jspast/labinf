#ifndef JANELA_H
#define JANELA_H

#include "tipos.h"
#include <raylib.h>

void IniciaJanela(JANELA *janela);
void FechaJanela(RenderTexture2D *render);

void IniciaQuadro(JANELA *janela);
void DesenhaQuadro(JANELA *j);

#endif
