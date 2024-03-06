#include <raylib.h>
#include <stdio.h>
#include "menu.h"
#include "game.h"
#include "defines.h"

int CarregaPerguntas(PERGUNTA perguntas[]);

/* Preenche o vetor de perguntas com as perguntas do arquivo de texto
 * Formatação das perguntas:
 *
 * enunciado
 * num_alternativas alternativa_correta
 * alternativa[0]
 * alternativa[1]
 * ...
 * alternativa[num_alternativas-1]
 */
int CarregaPerguntas(PERGUNTA perguntas[])
{
	int num_perguntas = 0;
	int i;

	FILE *arq;
	arq = fopen("perguntas.txt", "r");
	if(arq == NULL)
		return 0;
	else{
		while(!feof(arq)){
			fgets(perguntas[num_perguntas].enunciado, TAM_MAX_ENUNCIADO, arq);
			fscanf(arq, "%d%d\n", &perguntas[num_perguntas].num_alternativas, &perguntas[num_perguntas].alternativa_correta);
			for(i = 0; i < perguntas[num_perguntas].num_alternativas; i++)
				fgets(perguntas[num_perguntas].alternativas[i], TAM_MAX_ALTERNATIVAS, arq);
			num_perguntas++;
		}
		fclose(arq);
	}
	return num_perguntas - 1;
}

// Mecânica das perguntas
int Pergunta(PERGUNTA perguntas[], int aleatorio, int *alt_selecionada, Sound sons[])
{
	int resposta;
	int acertou = -1;

	// Lógica da seleção da alternativa
	resposta = Selecao(alt_selecionada, perguntas[aleatorio].num_alternativas, sons);

	// Verifica acerto
	if(resposta != -1){
		if(resposta + 1 == perguntas[aleatorio].alternativa_correta){
			acertou = 1;
			PlaySound(sons[5]);
		}
		else {
			acertou = 0;
			PlaySound(sons[6]);
		}
	}

	// Desenha fundo para a pergunta
	DrawRectangle(0, 0, RES_X, RES_Y, COR_FUNDO);

	// Desenha o enunciado e as alternativas
	DrawText(perguntas[aleatorio].enunciado, (RES_X - MeasureText(perguntas[aleatorio].enunciado, FONTE_ENUNCIADO))/2, 50, FONTE_ENUNCIADO, COR_ENUNCIADO);
	DesenhaSelecao(*alt_selecionada, perguntas[aleatorio].num_alternativas, perguntas[aleatorio].alternativas);

	return acertou;
}
