#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.h"

using namespace std;

int main(int argc, char const *argv[])
{

    // Nome do arquivo de entrada
    string arquivo_csv = "artigo.csv";
    string arquivo_dados = "data.bin";

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


    BPTree tree(ORDER);
    int cont = 0;
    bool inserido = true;

    // Leitura dos registros do arquivo de entrada
    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Registro* r = lineToRegister(line);
            if(r != NULL){        
             inserido = inserir_registro_bucket(hashTable, &tree, r,dataFileI,dataFile);
            }
            if(!inserido){
                cout << "Erro ao inserir registro!" << endl;
                cout << "Bucket cheio!" << endl;
                cout << "Total de registros inseridos: " << cont << endl;
            }else{
                cont++;
            }
        }
    }
    //tree.display(tree.getRoot(), 0);

    cout << "Arquivo de dados criado com sucesso!" << endl;
    cout << "Total de registros inseridos: " << cont << endl;

    // // Serializar a árvore em um arquivo binário
    // cout << "Serializando a árvore B+ no arquivo bptree.bin..." << endl;    
    // tree.serializeTree("bptree.bin");
    // cout << "Árvore B+ serializada.\n" << endl;
    // // Limpar a árvore existente
    // cout << "Limpando a árvore B+..." << endl;
    // delete tree.getRoot();
    // tree.root = nullptr;
    // cout << "Árvore B+ limpa.\n" << endl;

    // cout << "-----------------------------------" << endl;
    // cout << "Desserializando a árvore B+ do arquivo bptree.bin..." << endl;
    // tree.deserializeTree("bptree.bin");
    // cout << "Árvore B+ desserializada.\n" << endl;
    // tree.display(tree.getRoot(), 0);
    // cout << "-----------------------------------" << endl;

    // // Fechamento do arquivo de entrada de registros
    entry_file.close();
    //Fechamento do arquivo de entrada de dados (Para leitura)
    dataFileI.close();
    // Fechamento do arquivo de dados (Para escrita e leitura)
    dataFile.close();

    return 0;
}
