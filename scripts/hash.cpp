#include <iostream>
#include <fstream>
#include <cstring>
#include "Registro/registro.h"
#include "Constantes/constantes.h"
#include "Bloco/bloco.h"
#include "Bucket/bucket.h"
#include "Hash/hash.h"

using namespace std;

int cont = 0;
bool cheio = false;

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

// Função para inserir um registro em um bloco
void inserir_registro_bloco(ifstream& leitura, ofstream& escrita, BlocoCabecalho* cabecalho, Registro* registro, int ultimo_bloco, int index_bucket) {
    // Lê o bloco do arquivo
    Bloco* bloco = criarBloco(ultimo_bloco);
    leitura.seekg(index_bucket * BLOCK_SIZE * NUM_BLOCKS + (ultimo_bloco * BLOCK_SIZE) + sizeof(BlocoCabecalho));
    leitura.read(reinterpret_cast<char*>(bloco->dados), BLOCK_SIZE);

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

// Função para inserir um registro em um bucket
void inserir_registro_bucket(HashTable *hashtable, Registro *registro, ifstream &entrada, ofstream &saida)
{
    int indice_bucket = hashFunction(registro->id); // calcula o índice do bucket apropriado
    Bucket *bucket = hashtable->buckets[indice_bucket];

    for (Bloco *bloco : bucket->blocos)
    {
        int tam = bloco->cabecalho->tamanho_disponivel;
        //cout << "Valor tamanho disponivel: " << tam << " No Bloco " << bucket->ultimo_bloco << endl;
        if (tam >= registro->tamanho)
        {   
            inserir_registro_bloco(entrada, saida, bloco->cabecalho, registro, bucket->ultimo_bloco, indice_bucket); // adiciona o registro ao bloco
            cont++;
            bucket->ultimo_bloco = 0;
            return;
        }else{
            cout << "Registro de ID " << registro->id << " não cabe no bloco " << bucket->ultimo_bloco << endl;
            cout << "Tamanho disponivel: " << tam << endl;
            cout << "Tamanho do registro: " << registro->tamanho << endl;
            bucket->ultimo_bloco++;
        }
    }
    cout << "Bucket cheio" << endl;
    cheio = true;
    return;
}

int main(int argc, char const *argv[])
{
    // Criação do arquivo de dados
    ofstream dataFile("data.bin", ios::binary | ios::in | ios::out);
    if (!dataFile) {
        cerr << "Erro ao criar o arquivo de dados!" << endl;
        return 1;
    }

    // Criação da tabela hash com tamanho NUNM_BUCKETS e armaazenamento no arquivo de dados
    HashTable* hashTable = criarHashTable(dataFile);
    cout << "Tabela hash criada com sucesso!" << endl;

    //Inserção de registros de exemplo
    ifstream entry_file("artigo.csv");
    ifstream dataFileI("data.bin", ios::binary | ios::in);

    // Leitura dos registros do arquivo de entrada
    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Registro* r = lineToRegister(line);
            if(r != NULL){
                inserir_registro_bucket(hashTable, r,dataFileI,dataFile);
            }
            if(cheio){
                break;
            }
        }
    }

    // Fechamento do arquivo de entrada de registros
    entry_file.close();
    // Fechamento do arquivo de entrada de dados (Para leitura)
    dataFileI.close();
    // Fechamento do arquivo de dados (Para escrita e leitura)
    dataFile.close();
    cout << "Total de registros inseridos: " << cont << endl;

    return 0;
}