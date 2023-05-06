#ifndef BLOCO_H
#define BLOCO_H

#include <list>

using namespace std;

struct BlocoCabecalho {
    int index_bucket;
    int quantidade_registros;
    int tamanho_disponivel;
    list<int> posicoes_registros;
};

BlocoCabecalho* criarBlocoCabecalho() {
    BlocoCabecalho* cabecalho = new BlocoCabecalho();
    cabecalho->quantidade_registros = 0;
    cabecalho->tamanho_disponivel = 4096;
    cabecalho->posicoes_registros = {0};
    return cabecalho;
}

// Definição da estrutura do bloco
struct Bloco {
    BlocoCabecalho* cabecalho;
    unsigned char dados[BLOCK_SIZE];
};

Bloco* criarBloco(int index) {
    Bloco* bloco = new Bloco();
    bloco->cabecalho = criarBlocoCabecalho();
    bloco->cabecalho->index_bucket = index;
    for(int i = 0; i < BLOCK_SIZE; i++) {
        bloco->dados[i] = 0;
    }
    
    return bloco;
}

#endif