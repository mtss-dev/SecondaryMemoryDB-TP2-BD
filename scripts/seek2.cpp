#include "iostream"
#include "fstream"
#include "Bplustree/bplustree.hpp"
#include "Hash/hash.hpp"

using namespace std;


int main(int argc, char const *argv[])
{   
    if (argc < 2) {
        std::cout << "Uso: ./seek2 <titulo>" << std::endl;
        return 1;
    }

    //Abertura do arquivo de dados organizado por hashing
    ifstream dataFileI("Arquivos/arquivo_de_dados.bin", ios::binary | ios::in);
    string indice_primario = "Arquivos/indice_secundario.bin";

    Registro* registro_busca = buscar_registro_bpt(indice_primario,dataFileI, gerar_inteiro(remove_unicode(argv[1])));
    if(registro_busca != NULL) {
        cout << "\nRegistro com titulo " << argv[1] << " encontrado! " << endl;
        cout << "Campos do Registros:" << endl;
        imprimeRegistro(*registro_busca);
        cout << endl;
    } else {
        cout << "Registro com titulo " << argv[1] << " não encontrado!" << endl;
    }

    delete registro_busca;  
    //Fechamento do arquivo de dados organizado por hashing
    dataFileI.close();

    return 0;
}