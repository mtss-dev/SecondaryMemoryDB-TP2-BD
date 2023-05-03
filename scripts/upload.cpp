#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "string"


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




int main(int argc, char const *argv[])
{
    TRegistro *registro = criaRegistro(1, "Poster: 3D sketching and flexible input for surface design: A case study."
                                    , 2013, "Anamary Leal|Doug A. Bowman", 0, "28/07/2016  09:36:00", "Poster: 3D sketching and flexible input for surface design: A case study. A Leal, DA Bowman -  Interfaces (3DUI), 2013 IEEE Symposium , 2013 - ieeexplore.ieee.org. ABSTRACT Designing three-dimensional (3D) surfaces is difficult in both the physical world  and in 3D modeling software, requiring background knowledge and skill. The goal of this  work is to make 3D surface design easier and more accessible through natural and  ..");

    imprimeRegistro(registro);
    return 0;
}
