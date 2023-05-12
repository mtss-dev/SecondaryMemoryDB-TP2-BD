#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cout << "Uso: ./findrec <id_busca>" << std::endl;
        return 1;
    }

    //Abertura do arquivo de dados organizado por hashing
    ifstream dataFileI("data.bin", ios::binary | ios::in);

    Registro* registro_busca = buscar_registro(dataFileI, atoi(argv[1]));
    if(registro_busca != NULL) {
        imprimeRegistro(*registro_busca);
    } else {
        cout << "Registro não encontrado!" << endl;
    }

    //Fechamento do arquivo de dados organizado por hashing
    dataFileI.close();

    return 0;
}