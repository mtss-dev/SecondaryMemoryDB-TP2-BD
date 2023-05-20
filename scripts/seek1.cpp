#include "iostream"
#include "fstream"
#include "Bplustree/bplustree.hpp"
#include "Hash/hash.hpp"

using namespace std;


int main(int argc, char const *argv[])
{   
    if (argc < 2) {
        std::cout << "Uso: ./seek1 <id_busca>" << std::endl;
        return 1;
    }

    //Abertura do arquivo de dados organizado por hashing
    ifstream dataFileI("Arquivos/arquivo_de_dados.bin", ios::binary | ios::in);
    string indice_primario = "Arquivos/indice_primario.bin";

    Registro* registro_busca = buscar_registro_bpt(indice_primario,dataFileI, atoi(argv[1]));
    if(registro_busca != NULL) {
        cout << "\nRegistro com id " << atoi(argv[1]) << " encontrado! " << endl;
        cout << "Campos do Registros:" << endl;
        imprimeRegistro(*registro_busca);
        cout << endl;
    } else {
        cout << "Registro " << atoi(argv[1]) << " não encontrado!" << endl;
    }

    delete registro_busca;
    //Fechamento do arquivo de dados organizado por hashing
    dataFileI.close();

    return 0;
}