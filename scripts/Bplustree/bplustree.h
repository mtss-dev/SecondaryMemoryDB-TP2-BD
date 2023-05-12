#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include "../Constantes/constantes.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

    using namespace std;

    // Definição da estrutura no_registro
    struct no_registro {
        int id; // Seria a "key"
        int block_addr; // Endereço do bloco + posição do registro no arquivo de dados

        no_registro() : id(0), block_addr(0) {}
        no_registro(int _id, int _block_addr) : id(_id), block_addr(_block_addr) {}
    };


    // Definição da estrutura do nó da B+Tree
    struct Node {
        bool is_leaf;
        vector<no_registro> registros;
        vector<Node*> children;
        Node* parent;

        // Construtor atualizado
        Node(bool leaf = false) : is_leaf(leaf), parent(nullptr) {}
    };

    // Classe da B+Tree
    class BPlusTree {
    private:
        Node* root;

    public:
        BPlusTree() {
            root = nullptr;
        }

        // Função para obter a raiz da B+Tree
        Node* getRoot() const {
            return root;
        }

        int getBlockAddr(int key) {
            Node* current = root;
            while (!current->is_leaf) {
                int child_index = findChildIndex(current->registros, key);
                current = current->children[child_index];
            }

            int index = findChildIndex(current->registros, key);
            return current->registros[index].block_addr;
        }

        // Função para inserir um registro na B+Tree

        void insert(const no_registro& registro) {
            if (root == nullptr) {
                root = new Node();
                root->is_leaf = true;
                root->registros.push_back(registro);
            } else {
                Node* current = root;
                Node* parent = nullptr;

                int child_index = 0;
                while (!current->is_leaf) {
                    parent = current;
                    child_index = findChildIndex(current->registros, registro.id);
                    current = current->children[child_index];
                }

                insertIntoLeaf(current, registro);

                if (current->registros.size() > MAX_KEYS) {
                    Node* new_leaf = splitLeaf(current, parent);
                    parent->children.insert(parent->children.begin() + child_index + 1, new_leaf);
                }
            }
        }


        // Função auxiliar para encontrar o índice do filho correspondente a uma chave
        int findChildIndex(vector<no_registro>& registros, int key) {
            int index = 0;
            while (index < registros.size() && registros[index].id <= key) {
                index++;
            }
            return index;
        }

    // Função para serializar a árvore B+ em um vetor de bytes
    vector<char> serialize() const {
        vector<char> data;

        // Serializar recursivamente a árvore
        serializeNode(root, data);

        return data;
    }

    // Função para desserializar a árvore B+ a partir de um vetor de bytes
    void deserialize(const vector<char>& data) {
        size_t pos = 0;

        // Desserializar recursivamente a árvore
        root = deserializeNode(data, pos);
    }

    // Função auxiliar para serializar um valor genérico em um vetor de bytes
    template <typename T>
    static vector<char> serializeValue(const T& value) {
        vector<char> bytes;
        const char* rawPtr = reinterpret_cast<const char*>(&value);
        bytes.assign(rawPtr, rawPtr + sizeof(T));
        return bytes;
    }

    // Função auxiliar para desserializar um valor genérico a partir de um vetor de bytes
    template <typename T>
    static T deserializeValue(const vector<char>& data, size_t& pos) {
        T value;
        memcpy(&value, &data[pos], sizeof(T));
        pos += sizeof(T);
        return value;
    }

    // Função auxiliar para serializar um nó da árvore
    void serializeNode(const Node* node, vector<char>& data) const {
        // Verificar se o nó é válido
        if (node == nullptr)
            return;

        // Serializar o flag is_leaf
        data.push_back(static_cast<char>(node->is_leaf));

        // Serializar o número de registros
        size_t num_registros = node->registros.size();
        vector<char> num_registros_bytes = serializeValue(num_registros);
        data.insert(data.end(), num_registros_bytes.begin(), num_registros_bytes.end());

        // Serializar os registros
        for (const auto& registro : node->registros) {
            // Serializar cada campo do registro
            vector<char> id_bytes = serializeValue(registro.id);
            data.insert(data.end(), id_bytes.begin(), id_bytes.end());

            vector<char> block_addr_bytes = serializeValue(registro.block_addr);
            data.insert(data.end(), block_addr_bytes.begin(), block_addr_bytes.end());
        }

        // Serializar os ponteiros para os filhos
        if (!node->is_leaf) {
            for (const auto* child : node->children) {
                // Serializar o ponteiro para o filho
                serializeNode(child, data);
            }
        }
    }

    // Função auxiliar para desserializar um nó da árvore
    Node* deserializeNode(const vector<char>& data, size_t& pos) const {
        // Desserializar o flag is_leaf
        bool is_leaf = data[pos++];

        // Criar um novo nó
        Node* node = new Node(is_leaf);

        // Desserializar o número de registros
        size_t num_registros = deserializeValue<size_t>(data, pos);

        // Desserializar os registros
        for (size_t i = 0; i < num_registros; ++i) {
            // Desserializar cada campo do registro
            int id = deserializeValue<int>(data, pos);
            int block_addr = deserializeValue<int>(data, pos);

            // Adicionar o registro ao nó
            node->registros.emplace_back(id, block_addr);
        }

        // Desserializar os ponteiros para os filhos
        if (!is_leaf) {
            for (size_t i = 0; i < num_registros + 1; ++i) {
                // Desserializar o ponteiro para o filho
                Node* child = deserializeNode(data, pos);
                node->children.push_back(child);
                child->parent = node;
            }
        }

        return node;
    }

    // Função para inserir um registro em um nó folha
    void insertIntoLeaf(Node* leaf, const no_registro& registro) {
        int index = 0;
        while (index < leaf->registros.size() && leaf->registros[index].id < registro.id) {
            index++;
        }

        leaf->registros.insert(leaf->registros.begin() + index, registro);

        if (leaf->parent) {
            leaf->parent->registros[index] = registro;
        }
    }




    // Função para dividir um nó folha cheio
    // Função para dividir um nó folha cheio
    Node* splitLeaf(Node* leaf, Node* parent) {
        Node* new_leaf = new Node();
        new_leaf->is_leaf = true;

        int split_index = leaf->registros.size() / 2;
        int split_key = leaf->registros[split_index].id;

        new_leaf->registros.assign(leaf->registros.begin() + split_index, leaf->registros.end());
        leaf->registros.erase(leaf->registros.begin() + split_index, leaf->registros.end());

        if (leaf == root) {
            // Divisão do nó raiz
            Node* newRoot = new Node(false);
            Node* rightNode = new Node(false);
            int midIndex = new_leaf->registros.size() / 2;
            int midKey = new_leaf->registros[midIndex].id;

            newRoot->registros.push_back({ midKey, -1 }); // Pode definir o valor do block_addr como desejado
            newRoot->children.push_back(leaf);
            newRoot->children.push_back(rightNode);

            for (int i = midIndex + 1; i < new_leaf->registros.size(); i++) {
                rightNode->registros.push_back(new_leaf->registros[i]);
            }

            new_leaf->registros.resize(midIndex);

            leaf->parent = newRoot;
            rightNode->parent = newRoot;

            root = newRoot;
        } else {
            // O nó não é a raiz da árvore
            int index = 0;
            while (index < parent->children.size() && parent->children[index] != leaf) {
                index++;
            }

            if (index == parent->children.size()) {
                cerr << "Erro: Nó pai não contém o nó atual!" << endl;
                return nullptr;
            }

            Node* leftSibling = nullptr;
            Node* rightSibling = nullptr;

            if (index > 0) {
                leftSibling = parent->children[index - 1];
            }

            if (index < parent->children.size() - 1) {
                rightSibling = parent->children[index + 1];
            }

            if (leftSibling && leftSibling->registros.size() < MAX_KEYS) {
                // Transferir um registro do irmão esquerdo
                new_leaf->registros.insert(new_leaf->registros.begin(), parent->registros[index - 1]);
                parent->registros[index - 1] = leftSibling->registros.back();
                leftSibling->registros.pop_back();
            } else if (rightSibling && rightSibling->registros.size() < MAX_KEYS) {
                // Transferir um registro do irmão direito
                new_leaf->registros.push_back(parent->registros[index]);
                parent->registros[index] = rightSibling->registros.front();
                rightSibling->registros.erase(rightSibling->registros.begin());
            } else if (leftSibling) {
                // Mesclar com o irmão esquerdo
            leftSibling->registros.push_back(parent->registros[index - 1]);
            leftSibling->registros.insert(leftSibling->registros.end(), new_leaf->registros.begin(), new_leaf->registros.end());
            parent->registros.erase(parent->registros.begin() + index - 1);
            parent->children.erase(parent->children.begin() + index);

            delete new_leaf;

            balance(parent);
        } else if (rightSibling) {
            // Mesclar com o irmão direito
            new_leaf->registros.push_back(parent->registros[index]);
            new_leaf->registros.insert(new_leaf->registros.end(), rightSibling->registros.begin(), rightSibling->registros.end());
            parent->registros.erase(parent->registros.begin() + index);
            parent->children.erase(parent->children.begin() + index + 1);

            delete rightSibling;

            balance(parent);
        }
    }

        return new_leaf;
    }


    // Função para realizar a busca de uma chave na B+Tree
    bool search(Node* node, int key) {
        if (node == nullptr)
            return false;

        int index = 0;
        while (index < node->registros.size() && key > node->registros[index].id)
            index++;

        if (index < node->registros.size() && key == node->registros[index].id)
            return true;
        else if (node->is_leaf)
            return false;
        else
            return search(node->children[index], key);
    }

    // Função para realizar o balanceamento de um nó na B+Tree
    void balance(Node* node) {
        cout << "Balanceando nó com " << node->registros.size() << " registros" << endl;
        cout << "Máximo de registros: " << MAX_KEYS << endl;
        if (node->registros.size() <= ORDER)
            return;

        Node* parent = node->parent;
        if (parent == nullptr) {
            // O nó é a raiz da árvore
            if (node->registros.size() > ORDER){
                // Divisão do nó raiz
                Node* newRoot = new Node(false);
                Node* rightNode = new Node(false);

                int midIndex = node->registros.size() / 2;
                int midKey = node->registros[midIndex].id;

                newRoot->registros.push_back(no_registro(midKey, -1));
                newRoot->children.push_back(node);
                newRoot->children.push_back(rightNode);

                for (int i = midIndex + 1; i < node->registros.size(); i++) {
                    rightNode->registros.push_back(node->registros[i]);
                    rightNode->children.push_back(node->children[i]);
                }

                node->registros.resize(midIndex);
                node->children.resize(midIndex + 1);

                node->parent = newRoot;
                rightNode->parent = newRoot;

                root = newRoot;
            }
        } else {
            // O nó não é a raiz da árvore
            int index = 0;
            while (index < parent->children.size() && parent->children[index] != node)
                index++;

            if (index == parent->children.size()) {
                cerr << "Erro: Nó pai não contém o nó atual!" << endl;
                return;
            }

            Node* leftSibling = nullptr;
            Node* rightSibling = nullptr;

            if (index > 0)
                leftSibling = parent->children[index - 1];

            if (index < parent->children.size() - 1)
                rightSibling = parent->children[index + 1];

            if (leftSibling && leftSibling->registros.size() < MAX_KEYS) {
                // Transferir um registro do irmão esquerdo
                node->registros.insert(node->registros.begin(), parent->registros[index - 1]);
                parent->registros[index - 1] = leftSibling->registros.back();

                            if (!node->is_leaf)
                    node->children.insert(node->children.begin(), leftSibling->children.back());

                leftSibling->registros.pop_back();
                if (!leftSibling->is_leaf)
                    leftSibling->children.pop_back();
            } else if (rightSibling && rightSibling->registros.size() < MAX_KEYS) {
                // Transferir um registro do irmão direito
                node->registros.push_back(parent->registros[index]);
                parent->registros[index] = rightSibling->registros.front();
                
                if (!node->is_leaf)
                    node->children.push_back(rightSibling->children.front());

                rightSibling->registros.erase(rightSibling->registros.begin());
                if (!rightSibling->is_leaf)
                    rightSibling->children.erase(rightSibling->children.begin());
            } else if (leftSibling) {
                // Mesclar com o irmão esquerdo
                leftSibling->registros.push_back(parent->registros[index - 1]);
                for (int i = 0; i < node->registros.size(); i++) {
                    leftSibling->registros.push_back(node->registros[i]);
                    if (!leftSibling->is_leaf)
                        leftSibling->children.push_back(node->children[i]);
                }
                if (!leftSibling->is_leaf)
                    leftSibling->children.insert(leftSibling->children.end(), node->children.begin(), node->children.end());

                parent->registros.erase(parent->registros.begin() + index - 1);
                parent->children.erase(parent->children.begin() + index);

                delete node;

                balance(parent);
            } else if (rightSibling) {
                // Mesclar com o irmão direito
                node->registros.push_back(parent->registros[index]);
                for (int i = 0; i < rightSibling->registros.size(); i++) {
                    node->registros.push_back(rightSibling->registros[i]);
                    if (!node->is_leaf)
                        node->children.push_back(rightSibling->children[i]);
                }
                if (!node->is_leaf)
                    node->children.push_back(rightSibling->children.back());

                parent->registros.erase(parent->registros.begin() + index);
                parent->children.erase(parent->children.begin() + index + 1);

                delete rightSibling;

                balance(parent);
            }
        }
    }
};

#endif