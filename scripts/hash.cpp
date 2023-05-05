#include <iostream>
#include <fstream>
#include <cstring>
#include "Registro/registro.h"
#include "Constantes/constantes.h"
#include "Bloco/bloco.h"
#include "Bucket/bucket.h"
#include "Hash/hash.h"

using namespace std;

void imprimirRegistrosBloco(Bloco *bloco)
{
    int posicaoAtual = 0;
    int blocos_lidos = 0;
    Registro *registro = new Registro();

    while (blocos_lidos < bloco->cabecalho->quantidade_registros)
    {

        // Deserializa o próximo registro no bloco
        memcpy(&registro->id, &bloco->dados[posicaoAtual], sizeof(int));
        posicaoAtual += sizeof(int);

        registro->title = string((char *)&bloco->dados[posicaoAtual]);
        posicaoAtual += registro->title.size() + 1;

        memcpy(&registro->year, &bloco->dados[posicaoAtual], 2);
        posicaoAtual += sizeof(int);

        registro->authors = string((char *)&bloco->dados[posicaoAtual]);
        posicaoAtual += registro->authors.size() + 1;

        memcpy(&registro->citations, &bloco->dados[posicaoAtual], 1);
        posicaoAtual += sizeof(int);

        registro->update = string((char *)&bloco->dados[posicaoAtual]);
        posicaoAtual += registro->update.size() + 1;

        registro->snippet = string((char *)&bloco->dados[posicaoAtual]);
        posicaoAtual += registro->snippet.size() + 1;

        registro->tamanho = registro->title.size() + sizeof(int) + registro->authors.size() + sizeof(int) + sizeof(int) + registro->update.size() + registro->snippet.size() + 4;

        // Imprime o registro
        imprimeRegistro(*registro);
        blocos_lidos++;
    }
    delete registro;
}

// Função para inserir um registro no bloco
void inserir_registro_bloco(Bloco *bloco, Registro *registro)
{
    int posicao = bloco->cabecalho->posicoes_registros.back();

    // copia o valor de cada campo para a posição correta no buffer de dados
    memcpy(&bloco->dados[posicao], &registro->id, sizeof(int));
    posicao += sizeof(int);

    // copiar a string de titulo com o caractere nulo no final
    memcpy(&bloco->dados[posicao], registro->title.c_str(), registro->title.size() + 1);
    posicao += registro->title.size() + 1;

    // copiar o ano
    memcpy(&bloco->dados[posicao], &registro->year, sizeof(int));
    posicao += sizeof(int);

    // copiar a string de autores com o caractere nulo no final
    memcpy(&bloco->dados[posicao], registro->authors.c_str(), registro->authors.size() + 1);
    posicao += registro->authors.size() + 1;

    // copiar o numero de citacoes
    memcpy(&bloco->dados[posicao], &registro->citations, sizeof(int));
    posicao += sizeof(int);

    // copiar a string de atualizacao com o caractere nulo no final
    memcpy(&bloco->dados[posicao], registro->update.c_str(), registro->update.size() + 1);
    posicao += registro->update.size() + 1;

    // copiar a string de snippet com o caractere nulo no final
    memcpy(&bloco->dados[posicao], registro->snippet.c_str(), registro->snippet.size() + 1);
    posicao += registro->snippet.size() + 1;

    // atualiza o cabeçalho do bloco com as informações do novo registro
    bloco->cabecalho->quantidade_registros++;
    bloco->cabecalho->tamanho_disponivel -= registro->tamanho;

    // adiciona a posição do novo registro à lista de posições no cabeçalho
    bloco->cabecalho->posicoes_registros.push_back(posicao);
}

int cont = 0;
bool cheio = false;

// Função para inserir um registro na tabela hash
void inserir_registro_bucket(HashTable *hashtable, Registro *registro)
{
    int indice_bucket = hashFunction(registro->id); // calcula o índice do bucket apropriado
    Bucket *bucket = hashtable->buckets[indice_bucket];
    for (Bloco *bloco : bucket->blocos)
    {
        if (bloco->cabecalho->tamanho_disponivel >= registro->tamanho)
        {
            inserir_registro_bloco(bloco, registro); // adiciona o registro ao bloco
            cont++;
            return;
        }
    }

    // nenhum bloco tinha espaço suficiente, então cria um novo bloco
    if (bucket->quatidade_blocos < NUM_BLOCKS)
    {
        Bloco *novo_bloco = criarBloco();
        novo_bloco->cabecalho->index_bucket = indice_bucket;
        bucket->blocos.push_back(novo_bloco);
        bucket->quatidade_blocos++;
        // cout << "Novo bloco criado" << endl;
        inserir_registro_bucket(hashtable, registro);
        return;
    }
    else
    {
        cout << "Bucket cheio" << endl;
        cout << "Total de registros inseridos: " << cont << endl;
        cheio = true;
        return;
    }
}

// Registro *buscarRegistro(Bucket *bucket, int valorBusca)
// {

//     ifstream dataFile("fdp.bin", ios::binary);

//     // Verifica se o arquivo foi aberto corretamente
//     if (!dataFile.is_open())
//     {
//         cerr << "Não foi possível abrir o arquivo data.bin" << endl;
//         exit(1);
//     }

//     // Calcula o índice do bucket correspondente ao valor de busca
//     int indiceBucket = hashFunction(valorBusca);

//     // Posiciona o ponteiro de leitura do arquivo no início do bucket
//     dataFile.seekg(indiceBucket * sizeof(Bucket), ios::beg);

//     // Lê o cabeçalho do bucket
//     int quantidadeBlocos;
//     dataFile.read((char *)&quantidadeBlocos, sizeof(int));

//     // Para cada bloco no bucket
//     for (int i = 0; i < quantidadeBlocos; i++)
//     {
//         // Lê o bloco do arquivo
//         Bloco *bloco = new Bloco();
//         dataFile.read((char *)bloco->dados, sizeof(Bloco));

//         // Percorre os registros do bloco
//         int posicaoAtual = 0;
//         int blocosLidos = 0;
//         while (blocosLidos < bloco->cabecalho->quantidade_registros)
//         {
//             int posicaoAtual = 0;
//             int registros_lidos = 0;
//             Registro *registro = new Registro();

//             while (registros_lidos < bloco->cabecalho->quantidade_registros)
//             {
//                 // Deserializa o próximo registro no bloco
//                 memcpy(&registro->id, &bloco->dados[posicaoAtual], sizeof(int));
//                 posicaoAtual += sizeof(int);

//                 registro->title = string((char *)&bloco->dados[posicaoAtual]);
//                 posicaoAtual += registro->title.size() + 1;

//                 memcpy(&registro->year, &bloco->dados[posicaoAtual], 2);
//                 posicaoAtual += sizeof(int);

//                 registro->authors = string((char *)&bloco->dados[posicaoAtual]);
//                 posicaoAtual += registro->authors.size() + 1;

//                 memcpy(&registro->citations, &bloco->dados[posicaoAtual], 1);
//                 posicaoAtual += sizeof(int);

//                 registro->update = string((char *)&bloco->dados[posicaoAtual]);
//                 posicaoAtual += registro->update.size() + 1;

//                 registro->snippet = string((char *)&bloco->dados[posicaoAtual]);
//                 posicaoAtual += registro->snippet.size() + 1;

//                 registro->tamanho = registro->title.size() + sizeof(int) + registro->authors.size() + sizeof(int) + sizeof(int) + registro->update.size() + registro->snippet.size() + 4;

//                 // Verifica se o registro é o que estamos procurando
//                 if (registro->id == valorBusca)
//                 {
//                     // Retorna o registro encontrado
//                     delete registro;
//                     return registro;
//                 }

//                 registros_lidos++;
//             }

//             blocosLidos++;
//             delete registro;
//         }

//         // Registro não encontrado
//         return NULL;
//     }
// }

// Registro* buscarRegistro(Bucket* bucket, int id) {
//     // Percorre todos os blocos do bucket
//     for (int i = 0; i < bucket->quatidade_blocos; i++) {
//         Bloco* bloco = bucket->blocos[i];
//         int posicaoAtual = 0;
//         int blocos_lidos = 0;
//         while (blocos_lidos < bloco->cabecalho->quantidade_registros) {
//             // Deserializa o próximo registro no bloco
//             int registro_id;
//             memcpy(&registro_id, &bloco->dados[posicaoAtual], sizeof(int));
//             posicaoAtual += sizeof(int);
//             string title = string((char*)&bloco->dados[posicaoAtual]);
//             posicaoAtual += title.size()+1;
//             string year = string((char*)&bloco->dados[posicaoAtual]);
//             posicaoAtual +=year.size()+1;
//             string authors = string((char*)&bloco->dados[posicaoAtual]);
//             posicaoAtual += authors.size()+1;
//             int citations;
//             memcpy(&citations, &bloco->dados[posicaoAtual], sizeof(int));
//             posicaoAtual += sizeof(int);
//             string update = string((char*)&bloco->dados[posicaoAtual]);
//             posicaoAtual += update.size()+1;
//             string snippet = string((char*)&bloco->dados[posicaoAtual]);
//             posicaoAtual += snippet.size()+1;

//             // Verifica se o registro encontrado corresponde ao ID desejado
//             if (registro_id == id) {
//                 Registro* registro = new Registro();
//                 registro->id = registro_id;
//                 registro->title = title;
//                 registro->year = year;
//                 registro->authors = authors;
//                 registro->citations = citations;
//                 registro->update = update;
//                 registro->snippet = snippet;
//                 return registro;
//             }

//             blocos_lidos++;
//         }
//     }
//     // Se o registro não foi encontrado, retorna NULL
//     return NULL;
// }

int main(int argc, char const *argv[])
{


    // Criação do arquivo de dados
    ofstream dataFile("data.bin", ios::binary | ios::out);
    if (!dataFile) {
        cerr << "Erro ao criar o arquivo de dados!" << endl;
        return 1;
    }

    // Criação da tabela hash com tamanho NUNM_BUCKETS
    HashTable* hashTable = criarHashTable(dataFile);

    // //Inserção de registros de exemplo
    // ifstream entry_file("artigo.csv");

    // // Leitura dos registros do arquivo de entrada
    // if (entry_file.is_open()) {
    //     string line;
    //     while (getline(entry_file, line)){
    //         Registro* r = lineToRegister(line);
    //         if(r != NULL){
    //             inserir_registro_bucket(hashTable, r);
    //         }
    //         if(cheio){
    //             break;
    //         }
    //     }
    // }

    // // Escrita dos buckets no arquivo de dados
    // for (int i = 0; i < NUM_BUCKETS; i++) {
    //     for(int j = 0; j < hashTable->buckets[i]->quatidade_blocos; j++){
    //         Bloco* bloco = hashTable->buckets[i]->blocos.front();
    //         hashTable->buckets[i]->blocos.pop_front();
    //         dataFile.write((char*) bloco->dados, sizeof(bloco->dados));
    //     }
    // }
    // cout << "Total de registros inseridos: " << cont << endl;
    // cout << "Arquivo de dados criado com sucesso!" << endl;

    // // Fechamento do arquivo de dados
    // dataFile.close();

    // int idBucado;
    // cin >> idBucado;
    // while (idBucado != -1) {
    //     Registro* registro = buscarRegistro(hashTable->buckets[0], idBucado);
    //     if (registro != nullptr) {
    //         cout << "Registro encontrado: " << registro->id << endl;
    //     } else {
    //         cout << "Registro não encontrado!" << endl;
    //     }
    //     cin >> idBucado;
    // }

    return 0;
}