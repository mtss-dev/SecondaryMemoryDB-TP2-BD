#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <fstream>
#include "../Registro/registro.h"

using namespace std;

struct RegArvore {
    int chave;
    int valor;

    RegArvore(int chave, int valor) : chave(chave), valor(valor) {}
    RegArvore() : chave(0), valor(0) {}
};

template <typename T>
struct Node {
    bool is_leaf;
    size_t degree; // maximum number of children
    size_t size; // current number of item
    RegArvore* item;
    Node<RegArvore>** children;
    Node<RegArvore>* parent;

public:
    Node(size_t _degree) {// Constructor
        this->is_leaf = false;
        this->degree = _degree;
        this->size = 0;

        RegArvore* _item = new RegArvore[degree-1];
        for(int i=0; i<degree-1; i++){
            _item[i] = RegArvore(0,0);
        }
        this->item = _item;

        Node<RegArvore>** _children = new Node<RegArvore>*[degree];
        for(int i=0; i<degree; i++){
            _children[i] = nullptr;
        }
        this->children = _children;

        this->parent = nullptr;
    }
};

class BPlusTree {
public:
    Node<RegArvore>* root;
    size_t degree;

public:
    BPlusTree(size_t _degree) {// Constructor
        this->root = nullptr;
        this->degree = _degree;
    }
    // ~BPlusTree() { // Destructor
    //     clear(this->root);
    // }

    Node<RegArvore>* getroot(){
        return this->root;
    }

    Node<RegArvore>* BPlusTreeSearch(Node<RegArvore>* node, RegArvore key){
        if(node == nullptr) { // if root is null, return nullptr
            return nullptr;
        }
        else{
            Node<RegArvore>* cursor = node; // cursor finding key

            int altura = 1;
            while(!cursor->is_leaf){ // until cusor pointer arrive leaf
                for(int i=0; i<cursor->size; i++){ //in this index node, find what we want key
                    if(key.chave < cursor->item[i].chave){ //find some range, and let find their child also.
                        cursor = cursor->children[i];
                        break;
                    }
                    if(i == (cursor->size)-1){
                        cursor = cursor->children[i+1];
                        break;
                    }
                }
                altura++;
            }
            //search for the key if it exists in leaf node.
            for(int i=0; i<cursor->size; i++){
                if(cursor->item[i].chave == key.chave){
                    cout << "Quantidade de blocos lidos para encontrar o registro no arquivo de índice: " << altura << endl;
                    return cursor;
                }
            }
            
            return nullptr;
        }
    }

    Node<RegArvore>* BPlusTreeRangeSearch(Node<RegArvore>* node, RegArvore key){
    if(node == nullptr) { // if root is null, return nullptr
        return nullptr;
    }
    else{
        Node<RegArvore>* cursor = node; // cursor finding key

        while(!cursor->is_leaf){ // until cusor pointer arrive leaf
            for(int i=0; i<cursor->size; i++){ //in this index node, find what we want key
                if(key.chave < cursor->item[i].chave){ //find some range, and let find their child also.
                    cursor = cursor->children[i];
                    break;
                }
                if(i == (cursor->size)-1){
                    cursor = cursor->children[i+1];
                    break;
                }
            }
        }
        return cursor;
    }
}

int range_search(int start, int end, RegArvore* result_data, int arr_length) {
    int index=0;

    Node<RegArvore>* start_node = BPlusTreeRangeSearch(this->root, RegArvore(start, 0));
    Node<RegArvore>* cursor = start_node;
    RegArvore temp = cursor->item[0];

    while(temp.chave<=end){
        if(cursor == nullptr){
            break;
        }
        for(int i=0; i< cursor->size;i++){
            temp = cursor->item[i];
            if((temp.chave >= start)&&(temp.chave <= end)){
                result_data[index] = temp;
                index++;
            }
        }
        cursor = cursor->children[cursor->size];
    }
    return index;
}

    Node<RegArvore>* search(int chave) {  // Return true if the item exists. Return false if it does not.
        
        return BPlusTreeSearch(this->root, RegArvore(chave, 0)) ;
    }

    int find_index(RegArvore* arr, RegArvore data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data.chave < arr[i].chave){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }
        return index;
    }

    RegArvore* item_insert(RegArvore* arr, RegArvore data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data.chave < arr[i].chave){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }

        for(int i = len; i > index; i--){
            arr[i] = arr[i-1];
        }

        arr[index] = data;

        return arr;
    }

    Node<RegArvore>** child_insert(Node<RegArvore>** child_arr, Node<RegArvore>*child,int len,int index){
        for(int i= len; i > index; i--){
            child_arr[i] = child_arr[i - 1];
        }
        child_arr[index] = child;
        return child_arr;
    }

    Node<RegArvore>* child_item_insert(Node<RegArvore>* node, RegArvore data, Node<RegArvore>* child){
        int item_index=0;
        int child_index=0;
        for(int i=0; i< node->size; i++){
            if(data.chave < node->item[i].chave){
                item_index = i;
                child_index = i+1;
                break;
            }
            if(i==node->size-1){
                item_index = node->size;
                child_index = node->size+1;
                break;
            }
        }
        for(int i = node->size;i > item_index; i--){
            node->item[i] = node->item[i-1];
        }
        for(int i=node->size+1;i>child_index;i--){
            node->children[i] = node->children[i-1];
        }

        node->item[item_index] = data;
        node->children[child_index] = child;

        return node;
    }

    void InsertPar(Node<RegArvore>* par, Node<RegArvore>* child, RegArvore data) {
        // overflow check
        Node<RegArvore>* cursor = par;
        if (cursor->size < this->degree - 1) { // not overflow, just insert in the correct position
            // insert item, child, and reallocate
            cursor = child_item_insert(cursor, data, child);
            cursor->size++;
        }
        else { // overflow
            // make new node
            auto* Newnode = new Node<RegArvore>(this->degree);
            Newnode->parent = cursor->parent;

            // copy item
            RegArvore* item_copy = new RegArvore[cursor->size + 1];
            for (int i = 0; i < cursor->size; i++) {
                item_copy[i] = cursor->item[i];
            }
            item_copy = item_insert(item_copy, data, cursor->size);

            auto** child_copy = new Node<RegArvore>*[cursor->size + 2];
            for (int i = 0; i < cursor->size + 1; i++) {
                child_copy[i] = cursor->children[i];
            }
            child_copy[cursor->size + 1] = nullptr;
            child_copy = child_insert(child_copy, child, cursor->size + 1, find_index(item_copy, data, cursor->size + 1));

            // split nodes
            cursor->size = (this->degree) / 2;
            if ((this->degree) % 2 == 0) {
                Newnode->size = (this->degree) / 2 - 1;
            }
            else {
                Newnode->size = (this->degree) / 2;
            }

            for (int i = 0; i < cursor->size; i++) {
                cursor->item[i] = item_copy[i];
                cursor->children[i] = child_copy[i];
            }
            cursor->children[cursor->size] = child_copy[cursor->size];

            for (int i = 0; i < Newnode->size; i++) {
                Newnode->item[i] = item_copy[cursor->size + i + 1];
                Newnode->children[i] = child_copy[cursor->size + i + 1];
                Newnode->children[i]->parent = Newnode;
            }
            Newnode->children[Newnode->size] = child_copy[cursor->size + Newnode->size + 1];
            Newnode->children[Newnode->size]->parent = Newnode;

            RegArvore paritem = item_copy[this->degree / 2];

            delete[] item_copy;
            delete[] child_copy;

            // parent check
            if (cursor->parent == nullptr) { // if there are no parent node (root case)
                auto* Newparent = new Node<RegArvore>(this->degree);
                cursor->parent = Newparent;
                Newnode->parent = Newparent;

                Newparent->item[0] = paritem;
                Newparent->size++;

                Newparent->children[0] = cursor;
                Newparent->children[1] = Newnode;

                this->root = Newparent;
            }
            else { // if there already have parent node
                InsertPar(cursor->parent, Newnode, paritem);
            }
        }
    }

    void insert(RegArvore* data) {
        RegArvore reg(data->chave, data->valor);

        if (this->root == nullptr) { //if the tree is empty
            this->root = new Node<RegArvore>(this->degree);
            this->root->is_leaf = true;
            this->root->item[0] = *data;
            this->root->size = 1;
        } else { //if the tree has at least one node
            Node<RegArvore>* cursor = this->root;

            //move to leaf node
            cursor = BPlusTreeRangeSearch(cursor, reg);

            //overflow check
            if (cursor->size < (this->degree - 1)) { // not overflow, just insert in the correct position
                //item insert and rearrange
                cursor->item = item_insert(cursor->item, *data, cursor->size);
                cursor->size++;
                //edit pointer(next node)
                cursor->children[cursor->size] = cursor->children[cursor->size - 1];
                cursor->children[cursor->size - 1] = nullptr;
            } else { //overflow case
                //make new node
                auto* Newnode = new Node<RegArvore>(this->degree);
                Newnode->is_leaf = true;
                Newnode->parent = cursor->parent;

                //copy item
                RegArvore* item_copy = new RegArvore[cursor->size + 1];
                for (int i = 0; i < cursor->size; i++) {
                    item_copy[i] = cursor->item[i];
                }

                //insert and rearrange
                item_copy = item_insert(item_copy, *data, cursor->size);

                //split nodes
                cursor->size = (this->degree) / 2;
                if ((this->degree) % 2 == 0) {
                    Newnode->size = (this->degree) / 2;
                } else {
                    Newnode->size = (this->degree) / 2 + 1;
                }

                for (int i = 0; i < cursor->size; i++) {
                    cursor->item[i] = item_copy[i];
                }
                for (int i = 0; i < Newnode->size; i++) {
                    Newnode->item[i] = item_copy[cursor->size + i];
                }

                cursor->children[cursor->size] = Newnode;
                Newnode->children[Newnode->size] = cursor->children[this->degree - 1];
                cursor->children[this->degree - 1] = nullptr;

                delete[] item_copy;

                //parent check
                RegArvore paritem = Newnode->item[0];

                if (cursor->parent == nullptr) { //if there are no parent node(root case)
                    auto* Newparent = new Node<RegArvore>(this->degree);
                    cursor->parent = Newparent;
                    Newnode->parent = Newparent;

                    Newparent->item[0] = paritem;
                    Newparent->size++;

                    Newparent->children[0] = cursor;
                    Newparent->children[1] = Newnode;

                    this->root = Newparent;
                } else { //if there already have parent node
                    InsertPar(cursor->parent, Newnode, paritem);
                }
            }
        }
    }
    void bpt_print(){
        print(this->root);
    }

    void print(Node<RegArvore>* cursor) {
        if (cursor != NULL) {
            for (int i = 0; i < cursor->size; ++i) {
                cout  << cursor->item[i].chave << " ";
                cout  <<cursor->item[i].valor << " ";
            }
            cout << "\n";

            if (!cursor->is_leaf) {
                for (int i = 0; i < cursor->size + 1; ++i) {
                    print(cursor->children[i]);
                }
            }
        }
    }

    // Função para serializar uma árvore B+ em um arquivo binário
    void serializeBPlusTree(const BPlusTree& tree, const string& filename) {
        ofstream file(filename, ios::binary | ios::out);
        if (!file) {
            cerr << "Error opening file for serialization: " << filename << endl;
            return;
        }

        // Escrever o grau da árvore no arquivo
        size_t degree = tree.degree;
        file.write(reinterpret_cast<char*>(&degree), sizeof(degree));

        // Serializar a árvore recursivamente, começando pelo nó raiz
        serializeNode(file, tree.root);

        file.close();
    }

    // Função recursiva para serializar um nó e seus filhos
    void serializeNode(ofstream& file, const Node<RegArvore>* node) {
        // Escrever as informações do nó no arquivo
        bool is_leaf = node->is_leaf;
        size_t size = node->size;
        file.write(reinterpret_cast<const char*>(&is_leaf), sizeof(is_leaf));
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Escrever os itens do nó no arquivo
        file.write(reinterpret_cast<const char*>(node->item), sizeof(RegArvore) * (node->degree - 1));

        if (!is_leaf) {
            // Serializar os nós filhos recursivamente
            for (size_t i = 0; i <= size; ++i) {
                serializeNode(file, node->children[i]);
            }
        }
    }

    // Função para desserializar uma árvore B+ de um arquivo binário
    BPlusTree deserializeBPlusTree(const string& filename) {
        ifstream file(filename, ios::binary | ios::in);
        if (!file) {
            cerr << "Error opening file for deserialization: " << filename << endl;
            return BPlusTree(0);  // Retornar uma árvore B+ vazia
        }

        // Ler o grau da árvore do arquivo
        size_t degree;
        file.read(reinterpret_cast<char*>(&degree), sizeof(degree));

        // Criar uma nova árvore B+ com o grau fornecido
        BPlusTree tree(degree);

        // Desserializar a árvore recursivamente, começando pelo nó raiz
        tree.root = deserializeNode(file, nullptr, degree);

        file.close();

        return tree;
    }
    
    // Função recursiva para desserializar um nó e seus filhos
    Node<RegArvore>* deserializeNode(ifstream& file, Node<RegArvore>* parent, size_t degree) {
        // Ler as informações do nó do arquivo
        bool is_leaf;
        size_t size;
        file.read(reinterpret_cast<char*>(&is_leaf), sizeof(is_leaf));
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        // Criar um novo nó
        auto* node = new Node<RegArvore>(degree);
        node->is_leaf = is_leaf;
        node->size = size;
        node->parent = parent;

        // Ler os itens do nó do arquivo
        node->item = new RegArvore[degree - 1];
        file.read(reinterpret_cast<char*>(node->item), sizeof(RegArvore) * (degree - 1));

        if (!is_leaf)
        {
        // Desserializar os nós filhos recursivamente
        node->children = new Node<RegArvore>*[degree];
        for (size_t i = 0; i <= size; ++i) {
            node->children[i] = deserializeNode(file, node, degree);
        }
        }
        return node;
    }

};

int countNodes(Node<RegArvore>* node) {
    if (node == nullptr) {
        return 0;
    }

    int count = 1; // conta o próprio nó

    if (!node->is_leaf) {
        for (size_t i = 0; i <= node->size; i++) {
            count += countNodes(node->children[i]); // conta os nós filhos recursivamente
        }
    }

    return count;
}


Registro* buscar_registro_bpt(string index_filename, ifstream& dataFile, int id_busca) {
    BPlusTree bpt(MAX_KEYS);

    bpt = bpt.deserializeBPlusTree(index_filename);

    Registro* registro = new Registro();
    Node<RegArvore>* node = bpt.search(id_busca);

    RegArvore* reg = new RegArvore(-1, -1); // Inicializa com valores inválidos
    
    if(node != nullptr) {
        // Busca o registro no nó
        for (int i = 0; i < node->size; i++) {
            // Se encontrou o registro
            if (node->item[i].chave == id_busca) {
                reg->chave = node->item[i].chave;
                reg->valor = node->item[i].valor;
                break;
            }
        }

        // Se não encontrou o registro
        if(reg->chave == -1) {
            return NULL;
        }else{
            dataFile.seekg(reg->valor);
            dataFile.read(reinterpret_cast<char*>(&registro->id), sizeof(int));

            // Deserializa os demais campos do registro
            getline(dataFile, registro->title, '\0');
            dataFile.read(reinterpret_cast<char*>(&registro->year), sizeof(int));
            getline(dataFile, registro->authors, '\0');
            dataFile.read(reinterpret_cast<char*>(&registro->citations), sizeof(int));
            getline(dataFile, registro->update, '\0');
            getline(dataFile, registro->snippet, '\0');

            
            registro->tamanho = sizeof(int) + registro->title.size() + 1 +
                                sizeof(int) + registro->authors.size() + 1 +
                                sizeof(int) + registro->update.size() + 1 +
                                registro->snippet.size() + 1;

            int totalNodes = countNodes(bpt.getroot());
            cout << "Quantidade total de blocos do arquivo de índice primário: " << totalNodes << endl;
            return registro;
        }
    }else{
        return NULL;
    }
}




#endif //BPTREE_BPTREE_H
