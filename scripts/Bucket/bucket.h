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
    list<Bloco*> blocos;
};

Bucket* criarBucket(ofstream &dataFile) {
    Bucket* bucket = new Bucket();
    bucket->quatidade_blocos = 0;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        bucket->blocos.push_back(criarBloco());
        bucket->quatidade_blocos++;
        dataFile.write((char*)bucket->blocos.back(), sizeof(Bloco));
    }
    return bucket;
}

#endif