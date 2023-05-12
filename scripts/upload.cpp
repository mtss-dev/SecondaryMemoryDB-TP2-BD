#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.h"

using namespace std;

int main(int argc, char const *argv[])
{

    // Nome do arquivo de entrada
    string arquivo_csv = "artigo.csv";
    string arquivo_dados = "data2.bin";

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

    bool primeira_insercao = true;

    BPlusTree tree;
    vector<char> serializedData;

    // Leitura dos registros do arquivo de entrada
    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Registro* r = lineToRegister(line);
            if(r != NULL){

                if(primeira_insercao){
                    inserido = inserir_registro_bucket(hashTable, &tree, r,dataFileI,dataFile);
                    serializedData = tree.serialize();

                    // Armazenar a estrutura serializada em um arquivo binário
                    ofstream outFile("bplustree.bin", ios::binary);
                    if (!outFile) {
                        cerr << "Erro ao abrir o arquivo de saída." << endl;
                        return 1;
                    }

                    outFile.write(serializedData.data(), serializedData.size());
                    outFile.close();

                    primeira_insercao = false;
                }else{
                    
                    // Desserializar a árvore a partir do arquivo binário
                    ifstream inFile("bplustree.bin", ios::binary);
                    if (!inFile) {
                        cerr << "Erro ao abrir o arquivo de entrada. " << endl;
                        return 1;
                    }

                    // Obter o tamanho do arquivo
                    inFile.seekg(0, ios::end);
                    size_t fileSize = inFile.tellg();
                    inFile.seekg(0, ios::beg);

                    // Ler os dados do arquivo
                    vector<char> fileData(fileSize);
                    for(int i = 0; i < fileSize; i++){
                        cout << fileData[i];
                    }

                    inFile.read(fileData.data(), fileSize);
                    inFile.close();

                    inserido = inserir_registro_bucket(hashTable, &tree, r,dataFileI,dataFile);

                    serializedData = tree.serialize();

                    // Armazenar a estrutura serializada em um arquivo binário
                    ofstream outFile("bplustree.bin", ios::binary);
                    if (!outFile) {
                        cerr << "Erro ao abrir o arquivo de saída." << endl;
                        return 1;
                    }

                    outFile.write(serializedData.data(), serializedData.size());
                    outFile.close();
                }
            }
            if(!inserido){
                cout << "Erro ao inserir registro!" << endl;
                cout << "Bucket cheio!" << endl;
                cout << "Total de registros inseridos: " << cont << endl;
                return 1;
            }else{
                cont++;
            }
            delete r;
        }
    }

    cout << "Arquivo de dados criado com sucesso!" << endl;
    cout << "Total de registros inseridos: " << cont << endl;

    // // Fechamento do arquivo de entrada de registros
    entry_file.close();
    //Fechamento do arquivo de entrada de dados (Para leitura)
    dataFileI.close();
    // Fechamento do arquivo de dados (Para escrita e leitura)
    dataFile.close();

    return 0;
}
