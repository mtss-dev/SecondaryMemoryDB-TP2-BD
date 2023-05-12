#include "vector"
#include "../Constantes/constantes.h"
#include "iostream"

using namespace std;

// Definição da estrutura do nó da B+Tree
struct Node {
    bool is_leaf;
    vector<int> keys;
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
    
    // Função para inserir uma chave na B+Tree
    void insert(int key) {
        if (root == nullptr) {
            root = new Node();
            root->is_leaf = true;
            root->keys.push_back(key);
        } else {
            Node* current = root;
            Node* parent = nullptr;

            while (!current->is_leaf) {
                parent = current;
                int child_index = findChildIndex(current->keys, key);
                current = current->children[child_index];
            }

            insertIntoLeaf(current, key);

            if (current->keys.size() > MAX_KEYS) {
                splitLeaf(current, parent);
            }
        }
    }

    // Função auxiliar para encontrar o índice do filho correspondente a uma chave
    int findChildIndex(vector<int>& keys, int key) {
        int index = 0;
        while (index < keys.size() && keys[index] <= key) {
            index++;
        }
        return index;
    }

    // Função para inserir uma chave em um nó folha
    void insertIntoLeaf(Node* leaf, int key) {
        auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        leaf->keys.insert(it, key);
    }

    // Função para dividir um nó folha cheio
    void splitLeaf(Node* leaf, Node* parent) {
        Node* new_leaf = new Node();
        new_leaf->is_leaf = true;

        int split_index = leaf->keys.size() / 2;
        int split_key = leaf->keys[split_index];

        new_leaf->keys.assign(leaf->keys.begin() + split_index, leaf->keys.end());
        leaf->keys.erase(leaf->keys.begin() + split_index, leaf->keys.end());

        if (leaf == root) {
            root = new Node();
            root->is_leaf = false;
            root->keys.push_back(split_key);
            root->children.push_back(leaf);
            root->children.push_back(new_leaf);
        } else {
            int child_index = findChildIndex(parent->keys, split_key);
            parent->keys.insert(parent->keys.begin() + child_index, split_key);
            parent->children.insert(parent->children.begin() + child_index + 1, new_leaf);
        }
    }

    // Função para realizar a busca de uma chave na B+Tree
    bool search(Node* node, int key) {
        if (node == nullptr)
            return false;

        int index = 0;
        while (index < node->keys.size() && key > node->keys[index])
            index++;

        if (index < node->keys.size() && key == node->keys[index])
            return true;
        else if (node->is_leaf)
            return false;
        else
            return search(node->children[index], key);
    }
    // Função para realizar o balanceamento de um nó na B+Tree
    void balance(Node* node) {
        if (node->keys.size() <= ORDER)
            return;

        Node* parent = node->parent;
        if (parent == nullptr) {
            // O nó é a raiz da árvore
            if (node->keys.size() > MAX_KEYS) {
                // Divisão do nó raiz
                Node* newRoot = new Node(false);
                Node* rightNode = new Node(false);

                int midIndex = node->keys.size() / 2;
                int midKey = node->keys[midIndex];

                newRoot->keys.push_back(midKey);
                newRoot->children.push_back(node);
                newRoot->children.push_back(rightNode);

                for (int i = midIndex + 1; i < node->keys.size(); i++) {
                    rightNode->keys.push_back(node->keys[i]);
                    rightNode->children.push_back(node->children[i]);
                }

                node->keys.resize(midIndex);
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

            if (leftSibling && leftSibling->keys.size() < MAX_KEYS) {
                // Transferir uma chave do irmão esquerdo
                node->keys.insert(node->keys.begin(), parent->keys[index - 1]);
                parent->keys[index - 1] = leftSibling->keys.back();

                if (!node->is_leaf)
                    node->children.insert(node->children.begin(), leftSibling->children.back());

                leftSibling->keys.pop_back();
                if (!leftSibling->is_leaf)
                    leftSibling->children.pop_back();
            } else if (rightSibling && rightSibling->keys.size() < MAX_KEYS) {
                // Transferir uma chave do irmão direito
                node->keys.push_back(parent->keys[index]);
                parent->keys[index] = rightSibling->keys.front();

                if (!node->is_leaf)
                    node->children.push_back(rightSibling->children.front());

                rightSibling->keys.erase(rightSibling->keys.begin());
                if (!rightSibling->is_leaf)
                    rightSibling->children.erase(rightSibling->children.begin());
            } else if (leftSibling) {
                // Mesclar com o irmão esquerdo
                leftSibling->keys.push_back(parent->keys[index - 1]);
                for (int i = 0; i < node->keys.size(); i++) {
                    leftSibling->keys.push_back(node->keys[i]);
                    if (!leftSibling->is_leaf)
                        leftSibling->children.push_back(node->children[i]);
                }
                if (!leftSibling->is_leaf)
                    leftSibling->children.insert(leftSibling->children.end(), node->children.begin(), node->children.end());

                parent->keys.erase(parent->keys.begin() + index - 1);
                parent->children.erase(parent->children.begin() + index);

                delete node;

                balance(parent);
            } else if (rightSibling) {
                // Mesclar com o irmão direito
                node->keys.push_back(parent->keys[index]);
                for (int i = 0; i < rightSibling->keys.size(); i++) {
                    node->keys.push_back(rightSibling->keys[i]);
                    if (!node->is_leaf)
                        node->children.push_back(rightSibling->children[i]);
                }
                if (!node->is_leaf)
                    node->children.push_back(rightSibling->children.back());

                parent->keys.erase(parent->keys.begin() + index);
                parent->children.erase(parent->children.begin() + index + 1);

                delete rightSibling;

                balance(parent);
            }
        }
    }
};

int main() {
    BPlusTree bptree;

    // Inserção de chaves na B+Tree
    bptree.insert(10);
    bptree.insert(20);
    bptree.insert(5);
    bptree.insert(15);
    bptree.insert(25);
    bptree.insert(3);
    bptree.insert(6);
    bptree.insert(12);
    bptree.insert(18);
    bptree.insert(22);

    // Busca de chaves na B+Tree
    cout << "Chave 5 encontrada? " << (bptree.search(bptree.getRoot(), 5) ? "Sim" : "Não") << endl;
    cout << "Chave 15 encontrada? " << (bptree.search(bptree.getRoot(), 15) ? "Sim" : "Não") << endl;
    cout << "Chave 8 encontrada? " << (bptree.search(bptree.getRoot(), 8) ? "Sim" : "Não") << endl;
    cout << "Chave 22 encontrada? " << (bptree.search(bptree.getRoot(), 22) ? "Sim" : "Não") << endl;

    return 0;
}

