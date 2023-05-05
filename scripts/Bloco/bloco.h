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
    unsigned char dados[4096];
};

Bloco* criarBloco() {
    Bloco* bloco = new Bloco();
    bloco->cabecalho = criarBlocoCabecalho();
    return bloco;
}

#endif BLOCO_H