#include "vector"
#include "../Constantes/constantes.h"

// Definição da estrutura do nó da B+Tree
struct Node {
    bool is_leaf;
    vector<int> keys;
    vector<Node*> children;
};

// Classe da B+Tree
class BPlusTree {
private:
    Node* root;

public:
    BPlusTree() {
        root = nullptr;
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
};
