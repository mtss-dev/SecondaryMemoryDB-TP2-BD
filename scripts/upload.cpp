#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "string"
#include "vector"
#include "fstream"
#include <sstream>



using namespace std;

typedef struct registro{
    int id;
    string titulo; // no máximo 300 caracteres
    int ano;
    string autores; //no máximo 150 caracteres
    int citacoes;
    string atualizacao; //data e hora em string
    string snippet; //string entre 100 e 1024 caracteres
}TRegistro;

TRegistro *criaRegistro(int id, string titulo, int ano, string autores, int citacoes, string atualizacao, string snippet){
    TRegistro *registro = (TRegistro *)malloc(sizeof(TRegistro));
    registro->id = id;
    registro->titulo = titulo;
    registro->ano = ano;
    registro->autores = autores;
    registro->citacoes = citacoes;
    registro->atualizacao = atualizacao;
    registro->snippet = snippet;
    return registro;
}

void imprimeRegistro(TRegistro *registro){
    cout << "ID: " << registro->id << endl;
    cout << "Titulo: " << registro->titulo << endl;
    cout << "Ano: " << registro->ano << endl;
    cout << "Autores: " << registro->autores << endl;
    cout << "Citacoes: " << registro->citacoes << endl;
    cout << "Atualizacao: " << registro->atualizacao << endl;
    cout << "Snippet: " << registro->snippet << endl;
}


void separador(string s, string del = " ")
{
    int start, end = -1*del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        cout << s.substr(start, end - start) << endl;
    } 
    while (end != -1);

}

TRegistro * linhaParaRegistro(string linha){
    string del =  R"del(";")del";
    string regVal;

    TRegistro *registro = criaRegistro(0," ",0," ",0," "," ");
    int i = 0, start, end = -1*del.size();

    do {
        start = end + del.size();
        end = linha.find(del, start);

        switch (i){
            case 0:
                registro->id = stoi(linha.substr(1, end - start - 1));
                break;

            case 1:
                registro->titulo = linha.substr(start, end - start - 1);
                break;

            case 2:
                registro->ano = stoi(linha.substr(start, end - start));
                break;

            case 3:
                registro->autores = linha.substr(start, end - start - 1);
                break;

            case 4:
                registro->citacoes = stoi(linha.substr(start, end - start));
                break;

            case 5:
                registro->atualizacao = linha.substr(start, end - start);
                break;
                
            case 6:
                registro->snippet = linha.substr(start, end - start);
                registro->snippet.erase(registro->snippet.size() - 2);
                break;

            default:
                break;
            }      
        i++;
    } while (end != -1);
    imprimeRegistro(registro);
    return registro;
}



int main(int argc, char const *argv[])
{
    ifstream arquivo_entrada("teste.csv"); // abre o arquivo em modo de leitura
   
    if (arquivo_entrada.is_open()) { // verifica se o arquivo foi aberto com sucesso
        string linha;

        while (getline(arquivo_entrada, linha)){
            linhaParaRegistro(linha);
            
        }
    }
}
