#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.hpp"
#include "Bplustree/bplustree.hpp"
#include <chrono>
#include <unistd.h>
#include <sys/resource.h>

using namespace std;

int main(int argc, char const *argv[])
{   
    system("clear");

    // Definir a estrutura rusage para armazenar as informações sobre o uso de recursos
    struct rusage usage;

    // Iniciar o cronômetro
    auto start = std::chrono::steady_clock::now();

    if (argc < 2) {
        cerr << "Erro: você deve informar o nome do arquivo de entrada como argumento" << endl;
        return 1;
    }

    // Nome do arquivo de entrada
    string arquivo_csv = argv[1];
    string arquivo_dados = "Arquivos/arquivo_de_dados.bin";

    // Criação do arquivo de dados
    ofstream dataFile(arquivo_dados, ios::binary | ios::out);
    if (!dataFile) {
        cerr << "Erro ao criar o arquivo de dados!" << endl;
        return 1;
    }

    // Criação da tabela hash 
    HashTable* hashTable = criarHashTable(dataFile);
    cout << "Tabela hash criada com sucesso!" << endl;
    //destruirHashTable(hashTable);


    //Abertura do arquivo de entrada
    ifstream entry_file(arquivo_csv, ios::in);
    if(!entry_file){
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }

    //Abertura do arquivo de dados organizado por hashing
    ifstream dataFileI(arquivo_dados, ios::binary | ios::in);
    if(!dataFileI){
        cerr << "Erro ao abrir o arquivo de dados!" << endl;
        return 1;
    }

    cout << "Inserindo registros no arquivo de dados..." << endl;

    BPlusTree arvore_primaria(MAX_KEYS);
    BPlusTree arvore_secundaria(MAX_KEYS);
    // Leitura dos registros do arquivo de entrada
    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Registro* r = lineToRegister(line);
            if(r != NULL){
                inserir_registro_bucket(r,dataFileI,dataFile,arvore_primaria, arvore_secundaria);
            }
            delete r;
            r = nullptr;
        }
    }
    cout << "Arquivo de dados criado com sucesso!" << endl;
    arvore_primaria.serializeBPlusTree(arvore_primaria, "Arquivos/indice_primario.bin");
    arvore_secundaria.serializeBPlusTree(arvore_secundaria, "Arquivos/indice_secundario.bin");
    cout << "Indice primario e secundario criado com sucesso!" << endl;

    arvore_primaria.destroyTree(arvore_primaria.getroot());
    arvore_secundaria.destroyTree(arvore_secundaria.getroot());
    

    // // Fechamento do arquivo de entrada de registros
    entry_file.close();
    //Fechamento do arquivo de entrada de dados (Para leitura)
    dataFileI.close();
    // Fechamento do arquivo de dados (Para escrita e leitura)
    dataFile.close();

    // Parar o cronômetro
    auto end = std::chrono::steady_clock::now();

    // Obter informações sobre o uso de recursos
    getrusage(RUSAGE_SELF, &usage);

    // Calcular o tempo de CPU em segundos
    double cpuTime = std::chrono::duration<double>(end - start).count();

    // Exibir informações sobre o uso de recursos
    std::cout << "Tempo de CPU: " << cpuTime << " segundos" << std::endl;
    std::cout << "Uso máximo de memória: " << usage.ru_maxrss << " kilobytes" << std::endl;


    return 0;
}