#ifndef HASHING_EXTENSIVEL_H
#define HASHING_EXTENSIVEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "algorithm"
#include "cstring"
#include "map"
#include <math.h>
#include "stdlib.h"
#include "stdio.h"

using namespace std;

#define TAM_BLOCO 4096
#define TAM_CABECALHO 12

// Estrutura para um par chave-valor
struct Registro {
    int chave;
    int posicao;
};

// Estrutura para a página de disco
struct Pagina {
    int n;
    int folha;
    Registro p[TAM_BLOCO / sizeof(Registro)];
    int c[TAM_BLOCO / sizeof(int)];
};

// Estrutura para o cabeçalho do arquivo de índice
struct Cabecalho {
    int profundidade;
    int raiz;
    int primeiroLivre;
};

class Bucket {
  private:
    int profundidadeLocal;
    int tamRegistro;
    int tamBucket;
    int qtdRegistros;
    char* dados;

  public:
    Bucket(int profundidadeLocal, int tamRegistro) {
        this->profundidadeLocal = profundidadeLocal;
        this->tamRegistro = tamRegistro;
        this->tamBucket = pow(2, profundidadeLocal) * tamRegistro + 8;
        this->qtdRegistros = 0;
        this->dados = new char[tamBucket];
        memset(this->dados, 0, tamBucket);
    }

    ~Bucket() {
        delete[] dados;
    }

    bool inserir(char* registro) {
        if (qtdRegistros == pow(2, profundidadeLocal)) {
            return false; // bucket está cheio
        }

        int pos = qtdRegistros * tamRegistro;

        memcpy(&dados[pos], registro, tamRegistro);
        qtdRegistros++;

        return true;
    }

    int buscar(char* registro) {
        int pos = 0;
        while (pos < qtdRegistros * tamRegistro) {
            if (memcmp(&dados[pos], registro, tamRegistro) == 0) {
                return pos / tamRegistro;
            }
            pos += tamRegistro;
        }
        return -1;
    }

    bool remover(char* registro) {
        int pos = buscar(registro);
        if (pos == -1) {
            return false;
        }

        // Move os registros após o registro removido para frente
        int inicio = (pos + 1) * tamRegistro;
        int fim = qtdRegistros * tamRegistro;
        memmove(&dados[pos * tamRegistro], &dados[inicio], fim - inicio);

        qtdRegistros--;
        return true;
    }

    int getProfundidadeLocal() const {
        return profundidadeLocal;
    }

    void setProfundidadeLocal(int profundidadeLocal) {
        this->profundidadeLocal = profundidadeLocal;
    }

    int getTamRegistro() const {
        return tamRegistro;
    }

    void setTamRegistro(int tamRegistro) {
        this->tamRegistro = tamRegistro;
    }

    int getTamBucket() const {
        return tamBucket;
    }

    void setTamBucket(int tamBucket) {
        this->tamBucket = tamBucket;
    }

    int getQtdRegistros() const {
        return qtdRegistros;
    }

    void setQtdRegistros(int qtdRegistros) {
        this->qtdRegistros = qtdRegistros;
    }

    char* getDados() const {
        return dados;
    }

    void setDados(char* dados) {
        this->dados = dados;
    }

    char* getRegistro(int pos) const {
        if (pos >= 0 && pos < qtdRegistros) {
            char* registro = new char[tamRegistro];
            memcpy(registro, &dados[pos * tamRegistro], tamRegistro);
            return registro;
        }
        return nullptr;
    }

    void setRegistro(int pos, char* registro) {
        if (pos >= 0 && pos < qtdRegistros) {
            memcpy(&dados[pos * tamRegistro], registro, tamRegistro);
        }
    }
};  

class HashingExtensivel {
 private:
  std::fstream arquivoDados; // arquivo de dados
  std::fstream arquivoDiretorio; // arquivo de diretório
  int profundidadeGlobal; // profundidade global
  int tamBucket; // tamanho do bucket
  int quantidadeBlocosDiretorio; // quantidade de blocos do arquivo de diretório
  int quantidadeRegistrosInseridos; // quantidade de registros inseridos no arquivo de dados
  int quantidadeRegistrosPorBucket; // quantidade de registros que cabem em um bucket
  std::map<int, Bucket*> mapaBuckets; // mapa de buckets em memória primária

  // função para atualizar o arquivo de diretório
  void atualizarDiretorio();

  // função para criar um novo bucket e retornar o seu endereço
  Bucket* criarBucket();

  // função para buscar um bucket pelo seu endereço
  Bucket* buscarBucket(int endereco);

 public:
  HashingExtensivel(std::string arquivoDados, std::string arquivoDiretorio, int tamBucket = 2, int quantidadeRegistrosPorBucket = 1);

  ~HashingExtensivel();

  // função para inserir um registro no arquivo de dados
  bool inserir(Registro& registro);

  // função para buscar um registro no arquivo de dados
  bool buscar(int id, Registro& registro);

  // função para remover um registro do arquivo de dados
  bool remover(int id);
};

#endif // HASHING_EXTENSIVEL_H