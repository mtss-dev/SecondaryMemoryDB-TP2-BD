#ifndef HASH_HPP
#define HASH_HPP

#include "../Bplustree/bplustree.hpp"
#include "../Registro/registro.hpp"
#include "../Constantes/constantes.hpp"
#include "../Bloco/bloco.hpp"
#include "../Bucket/bucket.hpp"
#include "../Registro/registro.hpp"
#include <iostream>

using namespace std;

// Definição da estrutura da hash table
struct HashTable {
    Bucket* buckets[NUM_BUCKETS];
    int quantidade_registros;
};

// Função para criar uma hash table
HashTable* criarHashTable(ofstream& dataFile) {
    HashTable* hashTable = new HashTable();
    hashTable->quantidade_registros = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        hashTable->buckets[i] = criarBucket(dataFile);
    }
    return hashTable;
}

//Função para calcular o hash
int hashFunction(int id){
    int index = (37 * id) % NUM_BUCKETS;
    return index;
}

// Função para inserir um registro em um bloco
void inserir_registro_bloco(ifstream& leitura, ofstream& escrita, BlocoCabecalho* cabecalho, Registro* registro, int ultimo_bloco, int index_bucket) {
    // Lê o bloco do arquivo
    Bloco* bloco = criarBloco(ultimo_bloco);
    leitura.seekg(index_bucket * BLOCK_SIZE * NUM_BLOCKS + (ultimo_bloco * BLOCK_SIZE) + sizeof(BlocoCabecalho));
    leitura.read(reinterpret_cast<char*>(bloco->dados), BLOCK_SIZE - sizeof(BlocoCabecalho));

    // Insere o registro no bloco
    int posicao = cabecalho->posicoes_registros[cabecalho->quantidade_registros];
    memcpy(&bloco->dados[posicao], &registro->id, sizeof(int));
    posicao += sizeof(int);
    memcpy(&bloco->dados[posicao], registro->title.c_str(), registro->title.size() + 1);
    posicao += registro->title.size() + 1;
    memcpy(&bloco->dados[posicao], &registro->year, sizeof(int));
    posicao += sizeof(int);
    memcpy(&bloco->dados[posicao], registro->authors.c_str(), registro->authors.size() + 1);
    posicao += registro->authors.size() + 1;
    memcpy(&bloco->dados[posicao], &registro->citations, sizeof(int));
    posicao += sizeof(int);
    memcpy(&bloco->dados[posicao], registro->update.c_str(), registro->update.size() + 1);
    posicao += registro->update.size() + 1;
    memcpy(&bloco->dados[posicao], registro->snippet.c_str(), registro->snippet.size() + 1);
    posicao += registro->snippet.size() + 1;

    // Atualiza o cabeçalho do bloco
    cabecalho->quantidade_registros++;
    cabecalho->tamanho_disponivel -= registro->tamanho;
    cabecalho->posicoes_registros[cabecalho->quantidade_registros] = posicao;

    // Criar um buffer temporário para armazenar o cabeçalho atualizado e os dados do bloco
    char buffer[BLOCK_SIZE];
    // Copiar o cabeçalho atualizado para o buffer
    memcpy(buffer, cabecalho, sizeof(BlocoCabecalho));

    // Copiar os dados do bloco para o restante do buffer
    memcpy(buffer + sizeof(BlocoCabecalho), bloco->dados, BLOCK_SIZE - sizeof(BlocoCabecalho));

    // Reposiciona o ponteiro de escrita para a posição correta
    escrita.seekp(index_bucket * BLOCK_SIZE * NUM_BLOCKS + (ultimo_bloco * BLOCK_SIZE));
    // Escreve o buffer no arquivo
    escrita.write(reinterpret_cast<char*>(buffer), BLOCK_SIZE);

    // Libera a memória alocada para o bloco
    delete bloco;
}

int gerar_inteiro(string titulo)
{
    int chave = 0;
    int g = 31;
    int tam = titulo.size();

    for (int i = 0; i < tam; i++)
        chave = g * chave + (int)titulo[i];

    if (chave < 0)
        return (chave * -1) + titulo.size();
    else
        return chave + titulo.size();
}

// Função para inserir um registro em um bucket
bool inserir_registro_bucket(HashTable *hashtable, Registro *registro, ifstream &entrada, ofstream &saida, BPlusTree &btree1, BPlusTree &btree2)
{
    int indice_bucket = hashFunction(registro->id); // calcula o índice do bucket apropriado
    Bucket *bucket = hashtable->buckets[indice_bucket];

    for (Bloco *bloco : bucket->blocos)
    {
        int tam = bloco->cabecalho->tamanho_disponivel;
        if (tam >= registro->tamanho)
        {   

            int soma =  indice_bucket * BLOCK_SIZE * NUM_BLOCKS; //Inicio do Bucket
            soma += (bucket->ultimo_bloco * BLOCK_SIZE) + sizeof(BlocoCabecalho) + bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros];

            RegArvore *reg = new RegArvore(registro->id, soma); // adiciona o registro à árvore b+ do indice primario;
            RegArvore *reg2 = new RegArvore(gerar_inteiro(registro->title), soma); // adiciona o registro à árvore b+ do indice secundario;
            
            btree1.insert(reg);
            btree2.insert(reg2);
            inserir_registro_bloco(entrada, saida, bloco->cabecalho, registro, bucket->ultimo_bloco, indice_bucket); // adiciona o registro ao bloco
            bucket->ultimo_bloco = 0;
            return true;
        }else{
            bucket->ultimo_bloco++;
        }
    }
    return false;
}

//Função para buscar um registro no arquivo de dados
Registro* buscar_registro(ifstream& leitura, int id_busca) {
    // Percorre os blocos do bucket
    for (int ultimo_bloco = 0; ultimo_bloco < NUM_BLOCKS; ultimo_bloco++) {
        Bloco* bloco = criarBloco(ultimo_bloco);
        // Lê o cabeçalho do bloco
        int index_bucket = hashFunction(id_busca);
        leitura.seekg(index_bucket * BLOCK_SIZE * NUM_BLOCKS + (ultimo_bloco * BLOCK_SIZE));
        leitura.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(BlocoCabecalho));
        leitura.read(reinterpret_cast<char*>(bloco->dados), BLOCK_SIZE - sizeof(BlocoCabecalho));

        // Verifica se há registros no bloco
        if (bloco->cabecalho->quantidade_registros > 0) {
            // Verifica cada registro no bloco
            Registro* registro = new Registro();
            for (int i = 0; i < bloco->cabecalho->quantidade_registros; i++) {
                int posicao = bloco->cabecalho->posicoes_registros[i];

                // Verifica se o id do registro é igual ao id buscado
                memcpy(&registro->id, &bloco->dados[posicao], sizeof(int));
                if(registro->id == id_busca) {
                    posicao = sizeof(int);
                    // Deserializa o registro no bloco
                    registro->title = string((char *)&bloco->dados[posicao]);
                    posicao += registro->title.size() + 1;

                    memcpy(&registro->year, &bloco->dados[posicao], 2);
                    posicao += sizeof(int);

                    registro->authors = string((char *)&bloco->dados[posicao]);
                    posicao += registro->authors.size() + 1;

                    memcpy(&registro->citations, &bloco->dados[posicao], 1);
                    posicao += sizeof(int);

                    registro->update = string((char *)&bloco->dados[posicao]);
                    posicao += registro->update.size() + 1;

                    registro->snippet = string((char *)&bloco->dados[posicao]);
                    posicao += registro->snippet.size() + 1;

                    registro->tamanho = registro->title.size() + sizeof(int) + registro->authors.size() + sizeof(int) + sizeof(int) + registro->update.size() + registro->snippet.size() + 4;

                    cout << "Quantidade de blocos lidos para encontrar o registro: " << ultimo_bloco + 1 << endl;
                    cout << "Total de blocos no arquivo de dados: " << NUM_BLOCKS * NUM_BUCKETS << endl;

                    // Libera a memória alocada para o bloco
                    delete bloco;

                    // Retorna o registro encontrado
                    return registro;
                }
            }
        }
        delete bloco;
    }

    // Retorna NULL se o registro não for encontrado
    return nullptr;
}

#endif