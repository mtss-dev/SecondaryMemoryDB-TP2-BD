#ifndef BLOCO_H
#define BLOCO_H

#include "vector"

using namespace std;

//Definição da estrutura do cabeçalho
struct BlocoCabecalho {
    int quantidade_registros;
    int tamanho_disponivel;
    int posicoes_registros[18];
};

// Função para criar um Cabeçalho
BlocoCabecalho* criarBlocoCabecalho() {
    BlocoCabecalho* cabecalho = new BlocoCabecalho();
    cabecalho->quantidade_registros = 0;
    for (int i = 0; i < 18; i++) {
        cabecalho->posicoes_registros[i] = 0;
    }
    cabecalho->tamanho_disponivel = BLOCK_SIZE - sizeof(int) * 18 - sizeof(int) * 2;

    return cabecalho;
}

// Definição da estrutura do bloco
struct Bloco {
    int id_block;
    BlocoCabecalho* cabecalho;
    unsigned char dados[BLOCK_SIZE];
};

// Função para criar um bloco
Bloco* criarBloco(int id_block) {
    Bloco* bloco = new Bloco();
    bloco->cabecalho = criarBlocoCabecalho();
    bloco->id_block = id_block;
    for(int i = 0; i < BLOCK_SIZE; i++) {
        bloco->dados[i] = 0;
    }
    return bloco;
}

#endif