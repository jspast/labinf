# Os Labirintos do INF

Jogo de labirintos desenvolvido com [raylib](https://www.raylib.com/) em C na disciplina Algoritmos e Programação no semestre 2023/1.

Inspirado no jogo El Laberinto del Saber, de 1988.

## Como jogar

### Web
Versão para web disponível [aqui](https://jspast.github.io/labinf)

### Desktop
É necessário compilar para o seu sistema

## Como compilar

**Instale as ferramentas necessárias**:

- **gcc**, **make** e **git**

**Clone o repositório**
```bash
git clone https://github.com/jspast/labinf
cd labinf
```

**Compile para a plataforma desejada**

### Web

Para compilar para a Web, é necessário instalar o [**emscripten**](https://github.com/emscripten-core/emsdk)

Indique a pasta de instalação do emscripten (emsdk) após EMSDK_PATH=

```bash
make PLATFORM=PLATFORM_WEB EMSDK_PATH=caminho/para/emsdk
```

O resultado da compilação estará em build/web/

### Desktop
```bash
make
```

O resultado da compilação estará em build/desktop/
