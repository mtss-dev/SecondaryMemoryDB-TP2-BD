#ifndef HASH_H
#define HASH_H

#include "../Bucket/bucket.h"
#include "../Constantes/constantes.h"

struct HashTable {
    Bucket* buckets[NUM_BUCKETS];
    int quantidade_registros;
};

HashTable* criarHashTable() {
    HashTable* hashTable = new HashTable();
    hashTable->quantidade_registros = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        hashTable->buckets[i] = criarBucket();
    }
    return hashTable;
}

//Função para calcular o hash
int hashFunction(int id){
    int index = (37 * id) % NUM_BUCKETS;
    return index;
}

#endif