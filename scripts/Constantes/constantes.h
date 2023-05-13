#ifndef CONSTANTE_H
#define CONSTANTE_H

// Definir o tamanho do bloco e de Buckets
const int BLOCK_SIZE = 4096; // 4KB
const int NUM_BUCKETS = 15000; // 10 buckets de 4096 bytes = 40960 bytes
const int NUM_BLOCKS = 12; // 2 blocos de 4096 bytes = 8192 bytes
const int MAX_KEYS = 510; // 510 chaves por bloco na arvore b+

#endif