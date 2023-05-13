#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.h"
#include "BPlusTree/bPlusTree.h"

using namespace std;

int main(int argc, char const *argv[])
{   
    if (argc < 2) {
        cerr << "Erro: você deve informar o nome do arquivo de entrada como argumento" << endl;
        return 1;
    }

    // Nome do arquivo de entrada
    string arquivo_csv = argv[1];
    string arquivo_dados = "arquivo_de_dados.bin";

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
    // Leitura dos registros do arquivo de entrada
    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Registro* r = lineToRegister(line);
            if(r != NULL){
                inserido = inserir_registro_bucket(hashTable, r,dataFileI,dataFile,arvore_primaria);
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
    arvore_primaria.serializeBPlusTree(arvore_primaria, "indice_primario.bin");
    cout << "Indice primario criado com sucesso!" << endl;


    // // Fechamento do arquivo de entrada de registros
    entry_file.close();
    //Fechamento do arquivo de entrada de dados (Para leitura)
    dataFileI.close();
    // Fechamento do arquivo de dados (Para escrita e leitura)
    dataFile.close();

    return 0;
}