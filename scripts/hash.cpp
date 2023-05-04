#include <iostream>
#include <fstream>
#include <cstring>
#include <list>

using namespace std;
// Definir o tamanho do bloco e de Buckets
const int BLOCK_SIZE = 4096;
const int NUM_BUCKETS = 100;
const int NUM_BLOCKS = 10;

// Definição da estrutura do registro
struct Registro {
    int tamanho;
    int id;
    string title;
    int year;
    string authors;
    int citations;
    string update;
    string snippet;
};

Registro* criarRegistro(int id, string title, int year, string authors, int citations, string update, string snippet) {
    Registro* registro = new Registro();
    registro->id = id;
    registro->title = title;
    registro->year = year;
    registro->authors = authors;
    registro->citations = citations;
    registro->update = update;
    registro->snippet = snippet;
    registro->tamanho = sizeof(int) + sizeof(int) + title.size() + sizeof(int) + authors.size() + sizeof(int) + update.size() + sizeof(int) + snippet.size();
    return registro;
}

void imprimeRegistro(Registro registro) {
    cout << "ID: " << registro.id << endl;
    cout << "Titulo: " << registro.title << endl;
    cout << "Ano: " << registro.year << endl;
    cout << "Autores: " << registro.authors << endl;
    cout << "Citacoes: " << registro.citations << endl;
    cout << "Atualizacao: " << registro.update << endl;
    cout << "Snippet: " << registro.snippet << endl;
    cout << "Tamanho: " << registro.tamanho << endl;
}

struct BlocoCabecalho {
    int index_bucket;
    int quantidade_registros;
    int tamanho_disponivel;
    list<int> posicoes_registros;
};

BlocoCabecalho* criarBlocoCabecalho() {
    BlocoCabecalho* cabecalho = new BlocoCabecalho();
    cabecalho->quantidade_registros = 0;
    cabecalho->tamanho_disponivel = 4096;
    cabecalho->posicoes_registros = {0};
    return cabecalho;
}

// Definição da estrutura do bloco
struct Bloco {
    BlocoCabecalho* cabecalho;
    unsigned char dados[4096];
};

Bloco* criarBloco() {
    Bloco* bloco = new Bloco();
    bloco->cabecalho = criarBlocoCabecalho();
    return bloco;
}

// Definição da estrutura do bucket
struct Bucket {
    int quatidade_blocos = 0;
    list<Bloco*> blocos;
};

Bucket* criarBucket() {
    Bucket* bucket = new Bucket();
    return bucket;
}

struct HashTable {
    Bucket* buckets[NUM_BUCKETS];
    int quantidade_registros;
};

HashTable* criarHashTable() {
    HashTable* hashTable = new HashTable();
    hashTable->quantidade_registros = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        hashTable->buckets[i] = criarBucket();
    }
    return hashTable;
}

//Função para calcular o hash
int hashFunction(int id){
    int index = (37 * id) % NUM_BUCKETS;
    return index;
}

// Função para inserir um registro no bloco
void inserir_registro_bloco(Bloco* bloco, Registro* registro) {
    int posicao = bloco->cabecalho->posicoes_registros.front();
    cout << "Posicao: " << posicao << endl;

    // copia o valor de cada campo para a posição correta no buffer de dados
    memcpy(&bloco->dados[posicao], &registro->id, sizeof(int));
    posicao += sizeof(int);

    memcpy(&bloco->dados[posicao], registro->title.c_str(), registro->title.size());
    posicao += registro->title.size();

    memcpy(&bloco->dados[posicao], &registro->year, sizeof(int));
    posicao += sizeof(int);

    memcpy(&bloco->dados[posicao], registro->authors.c_str(), registro->authors.size());
    posicao += registro->authors.size();

    memcpy(&bloco->dados[posicao], &registro->citations, sizeof(int));
    posicao += sizeof(int);

    memcpy(&bloco->dados[posicao], registro->update.c_str(), registro->update.size());
    posicao += registro->update.size();

    memcpy(&bloco->dados[posicao], registro->snippet.c_str(), registro->snippet.size());
    posicao += registro->snippet.size();
    

    // atualiza o cabeçalho do bloco com as informações do novo registro
    bloco->cabecalho->quantidade_registros++;
    bloco->cabecalho->tamanho_disponivel -= registro->tamanho;
    
    // adiciona a posição do novo registro à lista de posições no cabeçalho
    bloco->cabecalho->posicoes_registros.push_back(posicao);
    cout << "Bloco pertence ao Bucket: " << hashFunction(registro->id) << endl;
    cout << "Tamanho disponivel: " << bloco->cabecalho->tamanho_disponivel << endl;
    cout << "Quantidade de registros: " << bloco->cabecalho->quantidade_registros << endl;
    cout << "Posicoes dos registros: ";
    for (int posicao : bloco->cabecalho->posicoes_registros) {
        cout << posicao << " ";
    }
    cout << endl;
}

// Função para inserir um registro na tabela hash
void inserir_registro_bucket(HashTable* hashtable, Registro* registro) {
    int indice_bucket = hashFunction(registro->id); // calcula o índice do bucket apropriado
    Bucket* bucket = hashtable->buckets[indice_bucket];
    for (Bloco* bloco : bucket->blocos) {
        if (bloco->cabecalho->tamanho_disponivel >= registro->tamanho) {
            inserir_registro_bloco(bloco,registro); // adiciona o registro ao bloco
            return;
        }
    }

     // nenhum bloco tinha espaço suficiente, então cria um novo bloco
    if(bucket->quatidade_blocos < NUM_BLOCKS){
        Bloco* novo_bloco = criarBloco();
        novo_bloco->cabecalho->index_bucket = indice_bucket;
        bucket->blocos.push_back(novo_bloco);
        bucket->quatidade_blocos++;
        cout << "Novo bloco criado" << endl;
        inserir_registro_bucket(hashtable, registro);
        return;
    }else{
        cout << "Bucket cheio" << endl;
        return;
    }
}

int main(int argc, char const *argv[])
{
    // Criação do arquivo de dados
    ofstream dataFile("data.bin", ios::binary | ios::out);
    if (!dataFile) {
        cerr << "Erro ao criar o arquivo de dados!" << endl;
        return 1;
    }

    // Criação da tabela hash com tamanho NUNM_BUCKETS
    HashTable* hashTable = criarHashTable();

    //Inserção de registros de exemplo
   //Loop para criar n registros
    int n = 2;
    for (int i = 1; i <= n; i++) {
        string titulo = "Titulo " + to_string(i);
        string autor = "Autor " + to_string(i);
        string atualizacao = "Atualizacao " + to_string(i);
        string snippet = "Snippet " + to_string(i);
        Registro* rec = criarRegistro(i, titulo, 2020, autor, 10, atualizacao, snippet);

        inserir_registro_bucket(hashTable, rec);
    }

    // Escrita dos buckets no arquivo de dados
    for (int i = 0; i < NUM_BUCKETS; i++) {
        dataFile.write((char*)&hashTable->buckets[i], sizeof(Bucket));
    }
    cout << "Arquivo de dados criado com sucesso!" << endl;

    // Fechamento do arquivo de dados
    dataFile.close();
    
    return 0;
}
