#include "jogo.h"
#include "tipos.h"
#include "constantes.h"
#include <raylib.h>

void CarregaMusicas(MUSICA *m)
{
	int i = 0;
	bool existe = true;

	do{
		if(FileExists(TextFormat("res/musicas/%.2d.ogg", i + 1))){
			m->musicas[i] = LoadMusicStream(TextFormat("res/musicas/%.2d.ogg", i + 1));
			m->musicas[i].looping = false;
			i++;
		}
		else {
			existe = false;
		}
	}while(i < MAX_MUSICAS && existe);

	m->total = i;

	m->atual = GetRandomValue(0, m->total - 1);
}

void CarregaSons(Sound sons[])
{
	sons[S_OPCAO] = LoadSound("res/sons/opcao.ogg");
	sons[S_SELECAO] = LoadSound("res/sons/selecao.ogg");
	sons[S_TRANSICAO] = LoadSound("res/sons/transicao.ogg");
	sons[S_COLISAO] = LoadSound("res/sons/colisao.ogg");
	sons[S_PERGUNTA] = LoadSound("res/sons/pergunta.ogg");
	sons[S_ACERTO] = LoadSound("res/sons/acerto.ogg");
	sons[S_ERRO] = LoadSound("res/sons/erro.ogg");
	sons[S_ITEM] = LoadSound("res/sons/item.ogg");
	sons[S_VITORIA] = LoadSound("res/sons/vitoria.ogg");
	sons[S_DERROTA] = LoadSound("res/sons/derrota.ogg");
	sons[S_PASSO_0] = LoadSound("res/sons/passo_0.ogg");
	sons[S_PASSO_1] = LoadSound("res/sons/passo_1.ogg");
	sons[S_PASSO_2] = LoadSound("res/sons/passo_2.ogg");
	sons[S_PASSO_3] = LoadSound("res/sons/passo_3.ogg");
	sons[S_PASSO_4] = LoadSound("res/sons/passo_4.ogg");
	sons[S_EXPLOSAO] = LoadSound("res/sons/explosao.ogg");
}

void CarregaAudio(AUDIO *audio)
{
	InitAudioDevice();

	audio->volume = VOLUME_PADRAO;
	SetMasterVolume(audio->volume / 10.0);

	CarregaSons(audio->sons);
	CarregaMusicas(&audio->musica);
}

void FechaMusica(MUSICA *m)
{
	for(int i = 0; i < m->total; i++)
		UnloadMusicStream(m->musicas[i]);
}

void FechaSons(Sound sons[])
{
	for(int i = 0; i < NUM_SONS; i++)
		UnloadSound(sons[i]);
}

void FechaAudio(AUDIO *audio)
{
	FechaMusica(&audio->musica);
	FechaSons(audio->sons);
	
	CloseAudioDevice();
}

void ProximaMusica(MUSICA *m)
{
	m->atual++;

	if(m->atual == m->total)
		m->atual = 0;
}

void AtualizaMusica(MUSICA *m)
{
	if (m->total > 0){
		
		if(!m->pausado && !IsMusicStreamPlaying(m->musicas[m->atual])){

			ProximaMusica(m);

			PlayMusicStream(m->musicas[m->atual]);
		}

		UpdateMusicStream(m->musicas[m->atual]);
	}
}

void PausaMusica(MUSICA *m, MENSAGEM *msg)
{
	m->pausado = !m->pausado;

	if (m->pausado){
		PauseMusicStream(m->musicas[m->atual]);
		*msg = (MENSAGEM){POS_MENSAGEM_ESQ, 2, "Música pausada", WHITE};
	}
	else {
		ResumeMusicStream(m->musicas[m->atual]);
		*msg = (MENSAGEM){POS_MENSAGEM_ESQ, 2, "Música retomada", WHITE};
	}
}

void VerificaPauseMusica(MUSICA *m, MENSAGEM *msg)
{
	if (IsKeyPressed(KEY_M))
		PausaMusica(m, msg);
}

void AtualizaVolume(char *v, MENSAGEM *m)
{
	int tecla = GetKeyPressed();

	if (tecla == KEY_LEFT || tecla == KEY_RIGHT){

		if (tecla == KEY_LEFT){
			*v -= ETAPA_VOLUME;
		}
		else if (tecla == KEY_RIGHT){
			*v += ETAPA_VOLUME;
		}

		if (*v > 10)
			*v = 10;
		else if (*v < 0)
			*v = 0;

		SetMasterVolume(*v / 10.0);
		*m = (MENSAGEM){POS_MENSAGEM_ESQ, 2};
		m->cor = WHITE;
		TextCopy(m->texto, TextFormat("Volume %d%%", *v * 10));
	}
}