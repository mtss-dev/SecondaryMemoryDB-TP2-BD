#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "string"
#include "vector"
#include "fstream"
#include <sstream>



using namespace std;

struct Registro {
    int tamanho;
    int id;
    int tamanho_title;
    string title;
    int year;
    int tamanho_authors;
    string authors;
    int citations;
    int tamanho_update;
    string update;
    int tamanho_snippet;
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
    registro->tamanho_title = title.length() + 1;
    registro->tamanho_authors = authors.length() + 1;
    registro->tamanho_update = update.length() + 1;
    registro->tamanho_snippet = snippet.length() + 1;
    registro->tamanho = registro->tamanho_title  + registro->tamanho_authors + registro->tamanho_update + registro->tamanho_snippet + sizeof(int)*8;
    return registro;
}

void imprimeRegistro(Registro *registro){
    cout << "ID: " << registro->id << endl;
    cout << "Titulo: " << registro->title << endl;
    cout << "Ano: " << registro->year << endl;
    cout << "Autores: " << registro->authors << endl;
    cout << "citações: " << registro->citations << endl;
    cout << "Atualizacao: " << registro->update << endl;
    cout << "Snippet: " << registro->snippet << endl;
    cout << endl;
}

void linhaParaRegistro3 (string linha){
    string del =  R"del(";")del";

    std::string str = R"(Hello;world;how;are;you)";
    std::string delimiter = ";";

    std::vector<std::string> tokens;
    std::size_t pos = 0;
    std::string token;
    while ((pos = linha.find(del)) != std::string::npos) {
        token = linha.substr(0, pos);
        tokens.push_back(token);
        linha.erase(0, pos + del.length());
    }
    tokens.push_back(linha);

    int i = 0;

    for (auto& s : tokens) {
        if (i == 0){
            s.erase(0, 1);
            cout << "ID: " << s << endl;
        } else if (i == 1){
            cout << "Titulo: " << s << endl;
        } else if (i == 2){
            cout << "Ano: " << s << endl;
        } else if (i == 3){
            cout << "Autores: " << s << endl;
        } else if (i == 4){
            cout << "Citações: " << s << endl;
        } else if (i == 5){
            cout << "Atualização: " << s << endl;
        } else if (i == 6){
            s.pop_back();
            cout << "Snippet: " << s << endl;
        }
        i++;
    }
}


Registro* linhaParaRegistro2(string linha){
    vector<string> campos;
    campos.reserve(8);
    
    int i = 0;
    while (i < linha.size()){
        string campo = "";
        
        if (linha[i] == ';'){
            i++;
            while (linha[i] != '"'){
                campo += linha[i];
                i++;
            }
            campos.push_back(campo);
        }

        if (linha[i] == 'N'){
            campo = " ";
            campos.push_back(campo);
            break;
        }
        
        i++;
    }
    
    try {
        string snippet = "";
        int id = stoi(campos[0]);
        string title = campos[1];
        int year = stoi(campos[2]);
        string authors = campos[3];
        int citations = stoi(campos[4]);
        string update = campos[5];
        snippet = campos[6];
        
        if (snippet == ""){
            snippet = "Sem Snippet";
        }          

        Registro *registro = criarRegistro(id, title, year, authors, citations, update, snippet);
        return registro;
    }
    catch (const std::invalid_argument& ia) {
        cout << "Erro ao converter string para inteiro" << endl;
        return NULL;
    }

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

Registro * linhaParaRegistro(string linha){
    string del =  R"del(";")del";

    int id, year, citations;
    string title, authors, update, snippet;

    int i = 0, start, end = -1*del.size();

    do {
        start = end + del.size();
        end = linha.find(del, start);

        switch (i){
            case 0:
                id = stoi(linha.substr(1, end - start - 1));
                break;

            case 1:
                title = linha.substr(start, end - start - 1) + '\0';
                break;

            case 2:
                year = stoi(linha.substr(start, end - start));
                break;

            case 3:
                authors = linha.substr(start, end - start - 1)+ '\0';
                break;

            case 4:
                citations = stoi(linha.substr(start, end - start));
                break;

            case 5:
                update = linha.substr(start, end - start)+ '\0';
                break;
                
            case 6:
                snippet = linha.substr(start, end - start);
                snippet.erase(snippet.size() - 1);
                snippet += '\0';
                break;

            default:
                break;
            }      
        i++;
    } while (end != -1);
    
    return criarRegistro(id, title, year, authors, citations, update, snippet);
}

void insere_registro_saida(Registro *registro, ofstream &arquivo_saida){
    arquivo_saida.write((char *)&registro->id, sizeof(int));

    int tamanho_titulo = registro->title.size(); 
    arquivo_saida.write((char *)&tamanho_titulo, sizeof(int));
    arquivo_saida.write(registro->title.c_str(), tamanho_titulo);

    arquivo_saida.write((char *)&registro->year, sizeof(int));

    int tamanho_autores = registro->authors.size(); 
    arquivo_saida.write((char *)&tamanho_autores, sizeof(int));
    arquivo_saida.write(registro->authors.c_str(), tamanho_autores);

    arquivo_saida.write((char *)&registro->citations, sizeof(int));

    int tamanho_atualizacao = registro->update.size(); 
    arquivo_saida.write((char *)&tamanho_atualizacao, sizeof(int));
    arquivo_saida.write(registro->update.c_str(), tamanho_atualizacao);

    int tamanho_snippet = registro->snippet.size(); 
    arquivo_saida.write((char *)&tamanho_snippet, sizeof(int));
    arquivo_saida.write(registro->snippet.c_str(), tamanho_snippet);
}

Registro *ler_registro_arquivo(ifstream &arquivo_entrada, int posicao=0){
    Registro *registro = new Registro;

    arquivo_entrada.seekg(posicao, ios::beg);

    arquivo_entrada.read((char *)&registro->id, sizeof(int));
    int tamanho_titulo;
    arquivo_entrada.read((char *)&tamanho_titulo, sizeof(int));
    char *buffer_titulo = new char[tamanho_titulo + 1];
    arquivo_entrada.read(buffer_titulo, tamanho_titulo);
    buffer_titulo[tamanho_titulo] = '\0';
    registro->title = buffer_titulo;
    delete[] buffer_titulo;
    arquivo_entrada.read((char *)&registro->year, sizeof(int));
    int tamanho_autores;
    arquivo_entrada.read((char *)&tamanho_autores, sizeof(int));
    char *buffer_autores = new char[tamanho_autores + 1];
    arquivo_entrada.read(buffer_autores, tamanho_autores);
    buffer_autores[tamanho_autores] = '\0';
    registro->authors = buffer_autores;
    delete[] buffer_autores;
    arquivo_entrada.read((char *)&registro->citations, sizeof(int));
    int tamanho_atualizacao;
    arquivo_entrada.read((char *)&tamanho_atualizacao, sizeof(int));
    char *buffer_atualizacao = new char[tamanho_atualizacao + 1];
    arquivo_entrada.read(buffer_atualizacao, tamanho_atualizacao);
    buffer_atualizacao[tamanho_atualizacao] = '\0';
    registro->update = buffer_atualizacao;
    delete[] buffer_atualizacao;
    int tamanho_snippet;
    arquivo_entrada.read((char *)&tamanho_snippet, sizeof(int));
    char *buffer_snippet = new char[tamanho_snippet + 1];
    arquivo_entrada.read(buffer_snippet, tamanho_snippet);
    buffer_snippet[tamanho_snippet] = '\0';
    registro->snippet = buffer_snippet;
    delete[] buffer_snippet;
    return registro;
}


// void  buscaRegistro(ifstream &arquivo_entrada, int id){
//     int tamanho_registro = -1;
//     int posicao = 0;

//     while (tamanho_registro != 0){
//         Registro *registro = ler_registro_arquivo(arquivo_entrada, posicao);
//         tamanho_registro = calcularTamanhoRegistro(registro);
//         cout <<tamanho_registro << endl;

//         if (registro->id == id){
//             imprimeRegistro(registro);
//             break;
//         }
        
//         posicao += tamanho_registro;
//     }

// }


int main(int argc, char const *argv[])
{
    ifstream arquivo_entrada("artigo.csv"); // abre o arquivo em modo de leitura
    ofstream arquivo_saida("data.bin", std::ios::binary); // abre o arquivo em modo de escrita

    if (arquivo_entrada.is_open()) { // verifica se o arquivo foi aberto com sucesso
        string linha;

        while (getline(arquivo_entrada, linha)){
            linhaParaRegistro3(linha);
            break;
            //imprimeRegistro(registro);
            //insere_registro_saida(registro, arquivo_saida);
            //delete registro;
        }
        arquivo_entrada.close(); // fecha o arquivo de entrada
        arquivo_saida.close(); // fecha o arquivo de saida
    }

    // ifstream arquivo_entrada2("data.bin", std::ios::binary); // abre o arquivo em modo de leitura
    
    // buscaRegistro(arquivo_entrada2, 1);

    //Registro *registro = ler_registro_arquivo(arquivo_entrada2, 578);
    //imprimeRegistro(registro);
}
