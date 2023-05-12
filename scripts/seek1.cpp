#include "iostream"
#include "fstream"
#include "Bplustree/bplustree.h"
#include "Hash/hash.h"

using namespace std;


int main(int argc, char const *argv[])
{
    Registro* registro = new Registro();

    // Abertura do arquivo de dados organizado por hashing
    ifstream leitura("data.bin", ios::binary | ios::in);

    leitura.seekg(216300931);

    leitura.read(reinterpret_cast<char*>(&registro->id), sizeof(int));

    cout << "ID: " << registro->id << endl;
    //1401200
    //1401200
    return 0;

    // leitura.read(reinterpret_cast<char*>(bloco->cabecalho), sizeof(BlocoCabecalho));

    // int endereco_exato = inicio_bucket + sizeof(BlocoCabecalho);

    // leitura.read(reinterpret_cast<char*>(bloco->dados), BLOCK_SIZE - sizeof(BlocoCabecalho));

    // int posicao = bloco->cabecalho->posicoes_registros[3];

    // memcpy(&registro->id, &bloco->dados[posicao], sizeof(int));

    // if(registro->id == id){
    //     cout << "Encontrou" << endl;
    //     endereco_exato += posicao;
    //     cout << "ID: " << registro->id << endl;
    //     cout << "Endereço exato: " << endereco_exato << endl;
    // }
    
    return 0;
}


// int main(int argc, char const *argv[])
// {
//     BPTree tree2(ORDER);
//     // Desserializar a árvore a partir do arquivo binário
//     cout << "Desserializando a árvore B+ do arquivo bptree.bin..." << endl;
//     tree2.deserializeTree("bptree.bin");
//     cout << "Árvore B+ desserializada.\n" << endl;

//     // Exibir a árvore desserializada
//     int tam = tree2.calculateSize(tree2.getRoot());
//     cout << "Tamanho em bytes da árvore B+ desserializada:" << tam << endl;
//     // tree2.display(tree2.getRoot(), 0);

//     // tree2.search(10);
//     return 0;
// }
