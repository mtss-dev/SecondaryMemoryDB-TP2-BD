#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.hpp"
#include "Bplustree/bplustree.hpp"
#include <chrono>
#include <sys/resource.h>
#include <sys/sysinfo.h>

using namespace std;

int main(int argc, char const *argv[])
{   
    system("clear");
    // Registrar o tempo de início
    auto start = chrono::high_resolution_clock::now();
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

    int cont = 0;
    bool inserido = true;
    BPlusTree arvore_primaria(MAX_KEYS);
    BPlusTree arvore_secundaria(MAX_KEYS);
    // Leitura dos registros do arquivo de entrada
    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Registro* r = lineToRegister(line);
            if(r != NULL){
                inserido = inserir_registro_bucket(hashTable, r,dataFileI,dataFile,arvore_primaria, arvore_secundaria);
            }
            if(!inserido){
                cout << "Erro ao inserir registro!" << endl;
                cout << "Bucket cheio!" << endl;
                cout << "Total de registros inseridos: " << cont << endl;
                return 1;
            }else{
                cont++;
            }
        }
    }
    cout << "Arquivo de dados criado com sucesso!" << endl;
    cout << "Total de registros inseridos: " << cont << endl;
    arvore_primaria.serializeBPlusTree(arvore_primaria, "Arquivos/indice_primario.bin");
    cout << "Indice primario criado com sucesso!" << endl;
    arvore_secundaria.serializeBPlusTree(arvore_secundaria, "Arquivos/indice_secundario.bin");
    cout << "Indice secundario criado com sucesso!" << endl;


    // // Fechamento do arquivo de entrada de registros
    entry_file.close();
    //Fechamento do arquivo de entrada de dados (Para leitura)
    dataFileI.close();
    // Fechamento do arquivo de dados (Para escrita e leitura)
    dataFile.close();
    // Registrar o tempo de término
    auto end = chrono::high_resolution_clock::now();

    // Calcular a duração em segundos
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();

    // Calcular minutos e segundos
    int minutes = static_cast<int>(seconds) / 60;
    int remainingSeconds = static_cast<int>(seconds) % 60;

    // Imprimir o tempo de execução
    cout << "Tempo de execução: " << minutes << " minuto(s) e " << remainingSeconds << " segundos" << endl;

    return 0;
}