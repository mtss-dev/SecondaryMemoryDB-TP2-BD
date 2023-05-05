#ifndef BUCKET_H
#define BUCKET_H

#include "list"
#include "../Bloco/bloco.h"

using namespace std;

// Definição da estrutura do bucket
struct Bucket {
    int quatidade_blocos = 0;
    list<Bloco*> blocos;
};

Bucket* criarBucket() {
    Bucket* bucket = new Bucket();
    return bucket;
}

#endif