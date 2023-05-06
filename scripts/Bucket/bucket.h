#ifndef BUCKET_H
#define BUCKET_H

#include "list"
#include "../Bloco/bloco.h"
#include "../Constantes/constantes.h"
#include "iostream"

using namespace std;

// Definição da estrutura do bucket
struct Bucket {
    int quatidade_blocos;
    int ultimo_bloco;
    list<Bloco*> blocos;
};

// Função para criar um bucket
Bucket* criarBucket(ofstream &dataFile) {
    Bucket* bucket = new Bucket();
    bucket->quatidade_blocos = 0;
    bucket->ultimo_bloco = 0;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        bucket->blocos.push_back(criarBloco(i+1));
        bucket->quatidade_blocos++;
        // Criar um buffer temporário para armazenar o cabeçalho e os dados
        char buffer[BLOCK_SIZE];
        // Copiar o cabeçalho para o início do buffer
        memcpy(buffer, bucket->blocos.back()->cabecalho, sizeof(BlocoCabecalho));
        // Copiar os dados para o restante do buffer
        memcpy(buffer + sizeof(BlocoCabecalho), bucket->blocos.back()->dados, BLOCK_SIZE - sizeof(BlocoCabecalho));
        // Escrever o buffer no arquivo
        dataFile.write(buffer, BLOCK_SIZE);
    }
    return bucket;
}

#endif