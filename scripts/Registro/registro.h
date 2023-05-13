#ifndef REGISTRO_H
#define REGISTRO_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

// Definição da estrutura do registro
struct Registro {
    int id;
    string title;
    int year;
    string authors;
    int citations;
    string update;
    string snippet;
    int tamanho;
};

// Função para criar um registro
Registro* criarRegistro(int id, string title, int year, string authors, int citations, string update, string snippet) {
    Registro* registro = new Registro();
    registro->id = id;
    registro->title = title;
    registro->year = year;
    registro->authors = authors;
    registro->citations = citations;
    registro->update = update;
    registro->snippet = snippet;
    registro->tamanho = registro->title.size() + 4 + sizeof(int) + sizeof(int) + registro->authors.size() + sizeof(int) + sizeof(int) + registro->update.size() + registro->snippet.size();
    return registro;
}

// Função para imprimir um registro
void imprimeRegistro(Registro registro) {
    cout << "------------------------" << endl;
    cout << "ID: " << registro.id << endl;
    cout << "Titulo: " << registro.title << endl;
    cout << "Ano: " << registro.year << endl;
    cout << "Autores: " << registro.authors << endl;
    cout << "Citacoes: " << registro.citations << endl;
    cout << "Atualizacao: " << registro.update << endl;
    cout << "Snippet: " << registro.snippet << endl;
    cout << "Tamanho do registro em bytes: " << registro.tamanho << endl;
    cout << "------------------------" << endl;
}

string remove_unicode(string str){
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < 0 || str[i] > 127) {
            str[i] = ' '; // substitui por um espaço em branco
        }
    }
    return str;
}

// Função para serializar um registro
Registro* lineToRegister(string entry_line){
    vector<string> fields;
    string field = "";
    
    string line = remove_unicode(entry_line);

    for (int i = 0; i < line.size()-1; i++){    
            
        if (line[i] == '"'){
            i++;
            while(!(line[i] == '"' && line[i+1] == ';') && i < line.size()-2){
                field += line[i];
                i++;
            }
            fields.push_back(field);
            field = "";
        } else if (line[i] == 'N' || (line[i] == ';' && line[i+1] == ';')){
            fields.push_back("NULL");
        }
    }

    if (fields.size() != 7){
        // cout << fields[0] << endl;
        // cout << "ERRO: faltou campos" << endl;
        return NULL;
    }
    // for(int i = 0; i < fields.size(); i++){
    //     cout << fields[i] << endl;
    // }
    try{
       return criarRegistro(stoi(fields[0]), fields[1], stoi(fields[2]), fields[3], stoi(fields[4]), fields[5], fields[6]);
    }
    catch(const std::exception& e){
       
        // cout << fields[0] << endl;
        // cout << "ERRO: faltou campos, algum falhou na conversão" << endl;
        return NULL;
    }
}

#endif // REGISTRO_H