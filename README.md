# Os-Labirintos-do-INF

Trabalho Prático Final - Semestre 2023/1

INF01202 - Algoritmos e Programação

Prof. Santiago V. Ravelo

---

- 28/08/2023 - Entrega

## ToDo:

 - [X] Menu inicial 
 - [x] Informações
 - [x] Ganhadores
---
 - [x] Arquivo binário de save
 - [x] Arquivo binário dos labirintos
 - [ ] Arquivo binário das estátuas
 - [ ] Arquivo binário dos ganhadores
 - [x] Arquivo de texto das perguntas
---
 - [x] Interface nos labirintos
 - [X] Menu de pause
 - [x] Interface das perguntas
---
 - [X] Aluno/jogador
 - [x] Professor/guardião
 - [x] Colega/estáta
---
 - [x] Bomba
 - [x] Crédito
---
 - [X] Saída do labirinto
 - [x] Pontuação
 - [x] Níveis de dificuldade
---

## Documentação:

### /res:

É onde ficam os sprites do jogo.

### /src:

É onde fica o código do jogo, dividido em arquivos .c.

### main.c:

Código base do jogo, chama as funções dos outros arquivos.

### janela.c

Código da parte gráfica de inicializar/fechar a janela do jogo e de usar uma "resolução interna".

### menu.c:

Código de tudo relacionado ao menu: tela inicial, dos ganhadores e de informações.

### game.c:

Código do "jogo em si". Chama as funções dos arquivos jogador.c, professores.c, perguntas.c, labirintos.c, desenha.c.

### desenha.c:

Código para exibir o estado do jogo na tela.

### defines.h:

Arquivo com todas as constantes usadas no jogo.