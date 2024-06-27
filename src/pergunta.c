#include "jogo.h"
#include "tipos.h"
#include "constantes.h"
#include <raylib.h>
#include <stdio.h>

/* Preenche o vetor de perguntas com as perguntas do arquivo de texto
 * Formatação das perguntas:
 *
 * enunciado
 * num_alternativas alternativa_correta
 * alternativa[0]
 * alternativa[1]
 * ...
 * alternativa[num_alternativas - 1]
 */
void CarregaPerguntas(PERGUNTAS *p)
{
	p->total = 0;
	int i;

	FILE *arq;
	arq = fopen(A_PERGUNTAS, "r");
	if(arq != NULL){
		while(!feof(arq)){
			fgets(p->perguntas[p->total].enunciado, TAM_MAX_ENUNCIADO, arq);
			fscanf(arq, "%d %d\n", &p->perguntas[p->total].num_alternativas, &p->perguntas[p->total].alternativa_correta);
			for(i = 0; i < p->perguntas[p->total].num_alternativas; i++)
				fgets(p->perguntas[p->total].alternativas[i], TAM_MAX_ALTERNATIVAS, arq);
			p->total++;
		}
		fclose(arq);
		p->total--;
	}
}

short PerguntaAleatoria(PERGUNTAS p)
{
	return GetRandomValue(0, p.total - 1);
}

void DesenhaTextoEnunciado(const char *text)
{
	DesenhaTextoCentralizado(text, (POSICAO){RES_X/2, 50}, RES_X - 100, FONTE_ENUNCIADO, COR_ENUNCIADO);
}

// Mecânica das perguntas
char AtualizaPergunta(PERGUNTAS p, char *alt_selecionada, Sound sons[])
{
	char resposta = -1;
	char acertou = -1;

	// Lógica da seleção da alternativa
	if(AtualizaSelecao(alt_selecionada, p.perguntas[p.aleatoria].num_alternativas, sons))
		resposta = *alt_selecionada;

	// Verifica acerto
	if(resposta != -1){
		if(resposta + 1 == p.perguntas[p.aleatoria].alternativa_correta){
			acertou = 1;
			PlaySound(sons[5]);
		}
		else {
			acertou = 0;
			PlaySound(sons[6]);
		}
	}
	return acertou;
}

void DesenhaPergunta(PERGUNTAS p, char alt_selecionada)
{
	// Desenha fundo para a pergunta
	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);

	// Desenha o enunciado e as alternativas
	DesenhaTextoEnunciado(p.perguntas[p.aleatoria].enunciado);
	DesenhaSelecao(alt_selecionada, p.perguntas[p.aleatoria].num_alternativas, p.perguntas[p.aleatoria].alternativas);
}