#include "string"
#include "iostream"
#include "vector"
#include "fstream"
#include "sstream"

using namespace std;

int cont = 0;

struct Register{
    int tam;
    int id;
    int year;
    int citations;
    string title;
    string authors;
    string update_date;
    string snippet;
};


Register* newRegister(int id, string title, int year, string authors, int citations, string update_date, string snippet){
    Register* r = new Register;
    r->id = id;
    r->year = year;
    r->citations = citations;
    r->title = title;
    r->authors = authors;
    r->update_date = update_date;
    r->snippet = snippet;
    r->tam = sizeof(int)*4 + r->title.size() + r->authors.size() + r->update_date.size() + r->snippet.size();
    return r;
}

Register* lineToRegister(string line){
    vector<string> fields;
    string field = "";
    
    for (int i = 0; i < line.length(); i++) {
        if (line[i] < 0 || line[i] > 127) {
            line[i] = ' '; // substitui por um espaço em branco
        }
    }

    for (int i = 0; i < line.size()-1; i++){
        if (line[i] == '"'){
            i++;
            while(!(line[i] == '"' && line[i+1] == ';') && i < line.size()-1){
                
                field += line[i];
                i++;
            }
            fields.push_back(field);
            field = "";
        } else if (line[i] == 'N'){
            fields.push_back("NULL");
        }
    }

    if (fields.size() != 7){
        cout << fields[0] << endl;
        cont ++;
        cout << "ERRO: faltou campos" << endl;
        return NULL;
    }
    try{
       return newRegister(stoi(fields[0]), fields[1], stoi(fields[2]), fields[3], stoi(fields[4]), fields[5], fields[6]);
    }
    catch(const std::exception& e){
       
        cout << fields[0] << endl;
        cout << "ERRO: faltou campos, algum falhou na conversão" << endl;
        return NULL;
    }
    
    
}

void printRegister(Register* r){

    if(r != NULL){
        cout << "ID: " << r->id << endl;
        cout << "Year: " << r->year << endl;
        cout << "Citations: " << r->citations << endl;
        cout << "Title: " << r->title << endl;
        cout << "Authors: " << r->authors << endl;
        cout << "Update Date: " << r->update_date << endl;
        cout << "Snippet: " << r->snippet << endl;
        cout << "Tam: " << r->tam << endl;
        cout << endl;
    }
   
}

int main(){
    ifstream entry_file("/home/vinicius/trabalhos/bd/tp2-bd/entradas/teste.csv");

    if (entry_file.is_open()) {
        string line;
        while (getline(entry_file, line)){
            Register* r = lineToRegister(line);
            printRegister(r);
            delete r;
        }
    }
    
    return 0;
}