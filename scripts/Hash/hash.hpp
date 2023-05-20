#ifndef HASH_HPP
#define HASH_HPP

#include "../Registro/registro.hpp"
#include "../Constantes/constantes.hpp"
#include "../Bloco/bloco.hpp"
#include "../Bucket/bucket.hpp"
#include <iostream>

using namespace std;

// Função que cria a estrtura de uma hash table e escreve em disco
void escreveHashTable(ofstream& dataFile) {
    for (int i = 0; i < NUM_BUCKETS; i++) {
        criarBucket(dataFile);
    }
}

//Função para calcular o hash
int hashFunction(int id){
    int index = (37 * id) % NUM_BUCKETS;
    return index;
}

// Função para inserir um registro em um bloco
void inserir_registro_bloco(ifstream& leitura, ofstream& escrita, Bloco* bloco, Registro* registro, int ultimo_bloco, int index_bucket) {

    // Insere o registro no bloco
    int posicao = bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros];
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
    bloco->cabecalho->quantidade_registros++;
    bloco->cabecalho->tamanho_disponivel -= registro->tamanho;
    bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros] = posicao;

    // Criar um buffer temporário para armazenar o cabeçalho atualizado e os dados do bloco
    char buffer[BLOCK_SIZE];
    // Copiar o cabeçalho atualizado para o buffer
    memcpy(buffer, bloco->cabecalho, sizeof(BlocoCabecalho));

    // Copiar os dados do bloco para o restante do buffer
    memcpy(buffer + sizeof(BlocoCabecalho), bloco->dados, BLOCK_SIZE - sizeof(BlocoCabecalho));

    // Reposiciona o ponteiro de escrita para a posição correta
    escrita.seekp(index_bucket * BLOCK_SIZE * NUM_BLOCKS + (ultimo_bloco * BLOCK_SIZE));
    // Escreve o buffer no arquivo
    escrita.write(reinterpret_cast<char*>(buffer), BLOCK_SIZE);
}

// Função para inserir um registro em um bucket e retornar o endereço do registro no arquivo de dados
int inserir_registro_bucket(Registro *registro, ifstream &entrada, ofstream &saida)
{   
    int indice_bucket = hashFunction(registro->id); // calcula o índice do bucket apropriado
    int ultimo_bloco = 0; // variável para armazenar o último bloco do bucket que foi percorrido
    int inicio_bucket = indice_bucket * BLOCK_SIZE * NUM_BLOCKS; //Inicio do Bucket no arquivo de dados
    entrada.seekg(inicio_bucket);
    
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        Bloco* bloco = new Bloco();
        bloco->cabecalho = new BlocoCabecalho();
        entrada.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(BlocoCabecalho));
        entrada.read(reinterpret_cast<char*>(bloco->dados), BLOCK_SIZE - sizeof(BlocoCabecalho));
    
        int tam = bloco->cabecalho->tamanho_disponivel;
        if (tam >= registro->tamanho)
        {   
            int addr =  inicio_bucket; //Calcula o endereço do bloco no arquivo de dados
            addr += (ultimo_bloco * BLOCK_SIZE) + sizeof(BlocoCabecalho) + bloco->cabecalho->posicoes_registros[bloco->cabecalho->quantidade_registros];

            inserir_registro_bloco(entrada, saida, bloco, registro, ultimo_bloco, indice_bucket); // adiciona o registro ao bloco
            destruirBloco(bloco); // desaloca o bloco da memória
            return addr;
        }else{
            ultimo_bloco++;
            destruirBloco(bloco); // desaloca o bloco da memória
        }
        if(i + 1 >= NUM_BLOCKS){
            cout << "Erro: Não há espaço disponível para inserir o registro" << endl;
            cout << "Registros inseridos: " << registro->id -1 << endl;
            cout << "Não foi possivel gerar os arquivos de indice" << endl;
            exit(1);
        }
    }
    return -1;
}

//Função para buscar um registro no arquivo de dados
Registro* buscar_registro(ifstream& leitura, int id_busca) {
    // Percorre os blocos do bucket
    for (int ultimo_bloco = 0; ultimo_bloco < NUM_BLOCKS; ultimo_bloco++) {
        Bloco* bloco = criarBloco();
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
                    posicao += sizeof(int);
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
                    destruirBloco(bloco);

                    // Retorna o registro encontrado
                    return registro;
                }
            }
        }
        destruirBloco(bloco);
    }

    // Retorna NULL se o registro não for encontrado
    return nullptr;
}

#endif