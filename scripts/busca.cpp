#include <iostream>
#include <fstream>
#include <cstring>
#include "Registro/registro.h"
#include "Constantes/constantes.h"
#include "Bloco/bloco.h"
#include "Bucket/bucket.h"
#include "Hash/hash.h"

using namespace std;

Registro* buscarRegistro(Bucket* bucket, int valorBusca) {

    ifstream dataFile("fdp.bin", ios::binary);

    // Verifica se o arquivo foi aberto corretamente
    if (!dataFile.is_open()) {
        cerr << "Não foi possível abrir o arquivo data.bin" << endl;
        exit(1);
    }

    // Calcula o índice do bucket correspondente ao valor de busca
    int indiceBucket = hashFunction(valorBusca);

    // Posiciona o ponteiro de leitura do arquivo no início do bucket
    dataFile.seekg(indiceBucket * sizeof(Bucket), ios::beg);

    // Lê o cabeçalho do bucket
    int quantidadeBlocos;
    dataFile.read((char*) &quantidadeBlocos, sizeof(int));

    // Para cada bloco no bucket
    for (int i = 0; i < quantidadeBlocos; i++) {
        // Lê o bloco do arquivo
        Bloco* bloco = new Bloco();
        dataFile.read((char*) bloco->dados, sizeof(Bloco));

        // Percorre os registros do bloco
        int posicaoAtual = 0;
        int blocosLidos = 0;
        while (blocosLidos < bloco->cabecalho->quantidade_registros) {
            int posicaoAtual = 0;
            int registros_lidos = 0;
            Registro* registro = new Registro();

            while (registros_lidos < bloco->cabecalho->quantidade_registros) {
                // Deserializa o próximo registro no bloco
                memcpy(&registro->id, &bloco->dados[posicaoAtual], sizeof(int));
                posicaoAtual += sizeof(int);

                registro->title = string((char*)&bloco->dados[posicaoAtual]);
                posicaoAtual += registro->title.size()+1;

                memcpy(&registro->year, &bloco->dados[posicaoAtual], 2);
                posicaoAtual += sizeof(int);

                registro->authors = string((char*)&bloco->dados[posicaoAtual]);
                posicaoAtual += registro->authors.size()+1;

                memcpy(&registro->citations, &bloco->dados[posicaoAtual],1);
                posicaoAtual += sizeof(int);

                registro->update = string((char*)&bloco->dados[posicaoAtual]);
                posicaoAtual += registro->update.size()+1;

                registro->snippet = string((char*)&bloco->dados[posicaoAtual]);
                posicaoAtual += registro->snippet.size()+1;

                
                registro->tamanho = registro->title.size() + sizeof(int) + registro->authors.size() + sizeof(int) + sizeof(int) + registro->update.size() + registro->snippet.size() + 4;

                // Verifica se o registro é o que estamos procurando
                if (registro->id == valorBusca) {
                    // Retorna o registro encontrado
                    delete registro;
                    return registro;
                }

                registros_lidos++;
            }

            blocosLidos++;
            delete registro;
        }

        // Registro não encontrado
        return nullptr;
    }
}


int main(int argc, char const *argv[])
{
    buscarRegistro(atoi(argv[1]));
    return 0;
}
