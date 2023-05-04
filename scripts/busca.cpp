#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

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

// Definir o tamanho do bloco
const int BLOCK_SIZE = 4096;
const int NUM_BUCKETS = 100;

int hashFunction(int id){
    int index = (37 * id) % NUM_BUCKETS;
    cout << "Hash function: " << index << endl;
    return index;
}

void buscaRegistro(int idBusca){
    // Abertura do arquivo de dados
    ifstream dataFile("data.bin", ios::binary | ios::in);
    if (!dataFile) {
        cerr << "Erro ao abrir o arquivo de dados!" << endl;
        exit(1);
    }

    // Busca de registro por ID
    int bucketIndex = hashFunction(idBusca); // Cálculo do índice do bucket
    dataFile.seekg(bucketIndex * sizeof(Bucket)); // Posicionamento do ponteiro do arquivo
    Bucket bucket; // Declaração do bucket para leitura do arquivo
    dataFile.read((char*)&bucket, sizeof(Bucket)); // Leitura do bloco do arquivo
    bool flag = false;
    for (int i = 0; i < bucket.count; i++) {
        if (bucket.registros[i].id == idBusca) {
            // Registro encontrado
            Registro registro = bucket.registros[i];
            imprimeRegistro(registro); // Carregando na memória somente o registro encontrado
            flag = true;
            break;
        }
    }
    if (!flag) {
        cout << "Registro nao encontrado!" << endl;
    }

    // Fechamento do arquivo de dados
    dataFile.close();
}

int main(int argc, char const *argv[])
{
    buscaRegistro(atoi(argv[1]));
    return 0;
}
