#include "iostream"
#include "fstream"
#include "Bplustree/bplustree.h"
#include "Hash/hash.h"

using namespace std;


int main(int argc, char const *argv[])
{   
    system("clear");
    if (argc < 2) {
        std::cout << "Uso: ./seek2 <titulo>" << std::endl;
        return 1;
    }

    //Abertura do arquivo de dados organizado por hashing
    ifstream dataFileI("arquivo_de_dados.bin", ios::binary | ios::in);
    string indice_primario = "indice_secundario.bin";

    Registro* registro_busca = buscar_registro_bpt(indice_primario,dataFileI, gerar_inteiro(argv[1]));
    if(registro_busca != NULL) {
        cout << "\nCampos do Registros:" << endl;
        imprimeRegistro(*registro_busca);
    } else {
        cout << "Registro não encontrado!" << endl;
    }

    //Fechamento do arquivo de dados organizado por hashing
    dataFileI.close();

    return 0;
}