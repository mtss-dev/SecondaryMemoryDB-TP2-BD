#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <iostream>
#include <vector>
#include "../Bloco/bloco.hpp"
#include "../Constantes/constantes.hpp"

using namespace std;

// Definição da estrutura do bucket
struct Bucket {
    int quatidade_blocos;
    int ultimo_bloco;
    Bloco* blocos[NUM_BLOCKS];
};

// Função para destruir um bucket
void destruirBucket(Bucket* bucket) {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        destruirBloco(bucket->blocos[i]);  // Desalocar o bloco
    }
    delete bucket;  // Desalocar o bucket em si
}

// Função para criar um bucket, criar os blocos escrever no arquivo de dados, em seguida desaloca o bucket e os blocos da memória principal
void criarBucket(ofstream &dataFile) {
    Bucket* bucket = new Bucket();
    bucket->quatidade_blocos = 0;
    bucket->ultimo_bloco = 0;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        bucket->blocos[i] = criarBloco();

        bucket->quatidade_blocos++;
        // Criar um buffer temporário para armazenar o cabeçalho e os dados
        char buffer[BLOCK_SIZE];
        // Copiar o cabeçalho para o início do buffer
        memcpy(buffer, bucket->blocos[i]->cabecalho, sizeof(BlocoCabecalho));
        // Copiar os dados para o restante do buffer
        memcpy(buffer + sizeof(BlocoCabecalho), bucket->blocos[i]->dados, BLOCK_SIZE - sizeof(BlocoCabecalho));
        // Escrever o buffer no arquivo
        dataFile.write(buffer, BLOCK_SIZE);
    }
    destruirBucket(bucket); // Desalocar o bucket e os blocos da memória principal
}

#endif