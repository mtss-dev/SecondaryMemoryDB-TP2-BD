#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

// Searching on a B+ tree in C++

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
using namespace std;
const int MAX = 3;

// Definição da estrutura no_registro
struct no_registro {
    int id; // Seria a "key"
    int block_addr; // Endereço do bloco + posição do registro no arquivo de dados

    no_registro() : id(0), block_addr(0) {}
    no_registro(int _id, int _block_addr) : id(_id), block_addr(_block_addr) {}
};

struct Node {
    no_registro key[MAX];
    Node* ptr[MAX + 1];
    bool IS_LEAF;
    int size;
    int level;
    Node* parent; // Novo membro para armazenar o nó pai
};

// BP tree
class BPTree
{
    Node *root;
    int nivel;
    Node *findParent(Node *, Node *);

public:
    BPTree(int nivel);
    Node* search(int); 
    void insert(no_registro x);
    void insertInternal(no_registro x, Node* cursor, Node* child, int level);
    void splitChild(Node* cursor, int i);
    void display(Node* cursor, int level);
    void serializeNode(Node* node, std::ofstream& outFile);
    void serializeBPTree(const std::string& filename);
    Node* deserializeNode(std::ifstream& inFile);
    BPTree deserializeBPTree(const std::string& filename);
    Node* getRoot();
};


BPTree::BPTree(int nivel)
{
    root = nullptr;
    this->nivel = nivel;
}


Node* BPTree::search(int id) {
    Node* cursor = root;
    while (cursor != nullptr) {
        for (int i = 0; i < cursor->size; i++) {
            if (id == cursor->key[i].id) {
                return cursor; // Encontrou o registro
            }
            else if (id < cursor->key[i].id) {
                if (cursor->IS_LEAF)
                    return nullptr; // Registro não encontrado
                else {
                    cursor = cursor->ptr[i]; // Desce para o nível inferior
                    break;
                }
            }
            else if (i == cursor->size - 1) {
                if (cursor->IS_LEAF)
                    return nullptr; // Registro não encontrado
                else {
                    cursor = cursor->ptr[i + 1]; // Desce para o nível inferior
                    break;
                }
            }
        }
    }
    return nullptr; // Registro não encontrado
}

void BPTree::insert(no_registro x) {
    if (root == nullptr) {
        root = new Node;
        root->key[0] = x;
        root->IS_LEAF = true;
        root->size = 1;
        root->level = 0; // Definindo o nível da raiz como 0
        root->parent = nullptr; // Definindo o pai da raiz como nulo
    } else {
        Node* cursor = root;
        Node* parent = nullptr;
        int level = 0; // Nível atual do cursor
        while (cursor->IS_LEAF == false) {
            parent = cursor;
            int i;
            for (i = 0; i < cursor->size; i++) {
                if (x.id < cursor->key[i].id) {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1) {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
            level++; // Incrementa o nível ao descer para o próximo nível
        }

        // Verifica se a chave já existe na folha
        for (int i = 0; i < cursor->size; i++) {
            if (x.id == cursor->key[i].id) {
                return; // Chave já existe, retorna sem fazer nada
            }
        }

        if (cursor->size < MAX) {
            int i = 0;
            while (x.id > cursor->key[i].id && i < cursor->size)
                i++;
            for (int j = cursor->size; j > i; j--) {
                cursor->key[j] = cursor->key[j - 1];
            }
            cursor->key[i] = x;
            cursor->size++;
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = nullptr;
            cursor->parent = parent; // Define o pai do cursor
        } else {
            Node* newLeaf = new Node;
            no_registro virtualNode[MAX + 1];
            for (int i = 0; i < MAX; i++) {
                virtualNode[i] = cursor->key[i];
            }
            int i = 0, j;
            while (x.id > virtualNode[i].id && i < MAX)
                i++;
            for (int j = MAX; j > i; j--) {
                virtualNode[j] = virtualNode[j - 1];
            }
            virtualNode[i] = x;
            newLeaf->IS_LEAF = true;
            cursor->size = MAX / 2;
            newLeaf->size = MAX - MAX / 2;
            newLeaf->level = level; // Definindo o nível do novo nó folha
            cursor->ptr[cursor->size] = newLeaf;
            newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
            cursor->ptr[MAX] = nullptr;
            for (i = 0; i < cursor->size; i++) {
                cursor->key[i] = virtualNode[i];
            }
            for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
                               newLeaf->key[i] = virtualNode[j];
            }
            if (cursor == root) {
                Node* newRoot = new Node;
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                newRoot->level = level + 1; // Definindo o nível do novo nó raiz
                newRoot->parent = nullptr; // O novo nó raiz não possui um pai
                cursor->parent = newRoot; // Define o pai do cursor
                newLeaf->parent = newRoot; // Define o pai do novo nó folha

                root = newRoot;
            } else {
                insertInternal(newLeaf->key[0], parent, newLeaf, level);
            }
        }
    }
}




void BPTree::insertInternal(no_registro x, Node* cursor, Node* child, int level) {
    int i;
    if (level == 0) {
        // Inserção em um nível folha
        for (i = 0; i < cursor->size; i++) {
            if (x.id < cursor->key[i].id)
                break;
        }
        for (int j = cursor->size; j > i; j--) {
            cursor->key[j] = cursor->key[j - 1];
            cursor->ptr[j + 1] = cursor->ptr[j];
        }
        cursor->key[i] = x;
        cursor->ptr[i + 1] = child;
        cursor->size++;
    }
    else {
        // Inserção em um nível interno
        for (i = 0; i < cursor->size; i++) {
            if (x.id < cursor->key[i].id)
                break;
        }
        if (cursor->ptr[i]->size == MAX) {
            // Nó cheio, é necessário fazer o split
            splitChild(cursor, i);
            if (x.id > cursor->key[i].id)
                i++;
        }
        insertInternal(x, cursor->ptr[i], child, level - 1);
    }
}

void BPTree::splitChild(Node* parentNode, int childIndex) {
    Node* childNode = parentNode->ptr[childIndex];
    Node* newNode = new Node;

    newNode->IS_LEAF = childNode->IS_LEAF;
    newNode->size = MAX / 2;
    parentNode->ptr[childIndex + 1] = newNode;

    for (int i = 0; i < newNode->size; i++) {
        newNode->key[i] = childNode->key[i + MAX / 2];
    }

    if (!childNode->IS_LEAF) {
        for (int i = 0; i <= newNode->size; i++) {
            newNode->ptr[i] = childNode->ptr[i + MAX / 2];
        }
    }

    childNode->size = MAX / 2;

    for (int i = parentNode->size; i > childIndex; i--) {
        parentNode->ptr[i + 1] = parentNode->ptr[i];
    }

    parentNode->ptr[childIndex + 1] = newNode;

    for (int i = parentNode->size - 1; i >= childIndex; i--) {
        parentNode->key[i + 1] = parentNode->key[i];
    }

    parentNode->key[childIndex] = childNode->key[MAX / 2];
    parentNode->size++;
}


// Find the parent
Node* BPTree::findParent(Node* currentNode, Node* childNode) {
    if (currentNode == nullptr || currentNode->IS_LEAF || currentNode->ptr[0] == childNode) {
        return nullptr;
    }

    for (int i = 0; i < currentNode->size; i++) {
        if (currentNode->ptr[i + 1] == childNode) {
            return currentNode;
        }

        if (currentNode->ptr[i + 1] == nullptr || childNode->key[0].id < currentNode->key[i].id) {
            return findParent(currentNode->ptr[i], childNode);
        }
    }

    return nullptr;
}


// Print the tree
void BPTree::display(Node* currentNode, int level) {
    if (currentNode == nullptr) {
        return;
    }

    for (int i = 0; i < currentNode->size; i++) {
        if (currentNode->IS_LEAF) {
            std::cout << "Level " << level << ": Key = " << currentNode->key[i].id << ", Block = " << currentNode->key[i].block_addr << std::endl;
        } else {
            display(currentNode->ptr[i], level + 1);
            std::cout << "Level " << level << ": Key = " << currentNode->key[i].id << std::endl;
        }
    }

    if (!currentNode->IS_LEAF) {
        display(currentNode->ptr[currentNode->size], level + 1);
    }
}



// Get the root
Node *BPTree::getRoot()
{
    return root;
}

void BPTree::serializeNode(Node* node, std::ofstream& outFile) {
    outFile.write(reinterpret_cast<char*>(node), sizeof(Node));
    outFile.write(reinterpret_cast<char*>(&node->level), sizeof(int)); // Serializa o campo nível

    if (!node->IS_LEAF) {
        for (int i = 0; i < node->size + 1; i++) {
            serializeNode(node->ptr[i], outFile);
        }
    }
}


void BPTree::serializeBPTree(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
        return;
    }

    serializeNode(root, outFile);

    outFile.close();
}


Node* BPTree::deserializeNode(std::ifstream& inFile) {
    Node* node = new Node;
    inFile.read(reinterpret_cast<char*>(node), sizeof(node->IS_LEAF) + sizeof(node->size) + sizeof(node->level));
    for (int i = 0; i < node->size; i++) {
        inFile.read(reinterpret_cast<char*>(&node->key[i]), sizeof(no_registro));
    }
    if (!node->IS_LEAF) {
        for (int i = 0; i < node->size + 1; i++) {
            node->ptr[i] = deserializeNode(inFile);
            node->ptr[i]->parent = node;
        }
    } else {
        for (int i = 0; i < node->size + 1; i++) {
            node->ptr[i] = nullptr;
        }
    }
    return node;
}



BPTree BPTree::deserializeBPTree(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Erro ao abrir o arquivo de entrada." << std::endl;
        return BPTree(0);
    }

    Node* root = BPTree::deserializeNode(inFile);

    inFile.close();

    return BPTree(root->level);
}



#endif