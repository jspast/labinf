# Os Labirintos do INF

Trabalho Prático Final - Semestre 2023/1

INF01202 - Algoritmos e Programação

Prof. Santiago V. Ravelo

## Como jogar

### Web
Versão para web disponível [aqui](jspast.github.io/labinf)

### Desktop
É necessário compilar para o seu sistema

## Como compilar

### Linux

**Instale as ferramentas necessárias**:

- **gcc**, **make** e **git**

**Clone o repositório**
```bash
git clone https://github.com/jspast/labinf
cd Os-Labirintos-do-INF
```

**Compile para a plataforma desejada**

- **Web**

Para compilar para a Web, é necessário instalar o **emscripten**

As instruções estão disponíveis no [Github](https://github.com/emscripten-core/emsdk)

Indique a pasta de instalação do emscripten (emsdk) após EMSDK_PATH=

```bash
make PLATFORM=PLATFORM_WEB EMSDK_PATH=caminho/para/emsdk
```

O resultado da compilação estará em build/web/

- **Desktop**
```bash
make
```

O resultado da compilação estará em build/desktop/
