#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
// Definir o tamanho do bloco e de Buckets
const int BLOCK_SIZE = 4096;
const int NUM_BUCKETS = 100;

// Definição da estrutura do registro
struct Registro {
    int id;
    char title[301];
    int year;
    char authors[151];
    int citations;
    char update[20];
    char snippet[1025];
};

void imprimeRegistro(Registro registro) {
    cout << "ID: " << registro.id << endl;
    cout << "Titulo: " << registro.title << endl;
    cout << "Ano: " << registro.year << endl;
    cout << "Autores: " << registro.authors << endl;
    cout << "Citacoes: " << registro.citations << endl;
    cout << "Atualizacao: " << registro.update << endl;
    cout << "Snippet: " << registro.snippet << endl;
}

// Definição da estrutura do bucket
struct Bucket {
    int count;
    Registro registros[10];
};

void insereRegistro(Bucket hashtable[], Registro registro){
    int bucketIndex;
    bucketIndex = registro.id % NUM_BUCKETS;
    if(hashtable[bucketIndex].count < 10){
        hashtable[bucketIndex].registros[hashtable[bucketIndex].count] = registro;
        hashtable[bucketIndex].count++;
    }else{
        cout << "Erro ao inserir registro no bucket " << bucketIndex << endl;
    }
}

int main(int argc, char const *argv[]){
    // Criação do arquivo de dados
    ofstream dataFile("data.bin", ios::binary | ios::out);
    if (!dataFile) {
        cerr << "Erro ao criar o arquivo de dados!" << endl;
        return 1;
    }

    // Criação da tabela hash com tamanho NUNM_BUCKETS
    Bucket hashTable[NUM_BUCKETS];

    // Inicialização dos buckets
    for (int i = 0; i < NUM_BUCKETS; i++) {
        hashTable[i].count = 0;
    }

    //Inserção de registros de exemplo
   // Loop para criar n registros
    int n = 100;
    for (int i = 1; i <= n; i++) {
        Registro rec;
        rec.id = i;
        sprintf(rec.title, "Artigo %d", i);
        rec.year = 2021 + i % 3;
        sprintf(rec.authors, "Autor %d, Autor %d", i*2-1, i*2);
        rec.citations = i % 10;
        sprintf(rec.update, "2022-05-03 00:00:%02d", i%60);
        sprintf(rec.snippet, "Resumo do artigo %d", i);
        insereRegistro(hashTable, rec);
    }

    // Escrita dos buckets no arquivo de dados
    for (int i = 0; i < NUM_BUCKETS; i++) {
        dataFile.write((char*)&hashTable[i], sizeof(Bucket));
    }
    cout << "Arquivo de dados criado com sucesso!" << endl;

    // Fechamento do arquivo de dados
    dataFile.close();
    
    return 0;
}
