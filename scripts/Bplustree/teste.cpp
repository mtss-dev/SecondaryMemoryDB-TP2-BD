#include "bplustree.h"

int main() {
    // // Criar uma árvore B+ para teste
    // BPlusTree tree(15); // Grau 3
    // RegArvore *reg = new RegArvore(10, 20); // adiciona o registro à árvore);

    // tree.insert(reg);

    // reg->chave = 20;
    // reg->valor = 40;

    // tree.insert(reg);

    // reg->chave = 30;    
    // reg->valor = 60;

    // tree.insert(reg);

    // reg->chave = 40;
    // reg->valor = 80;

    // tree.insert(reg);

    // reg->chave = 50;
    // reg->valor = 100;

    // tree.insert(reg);

    // reg->chave = 60;
    // reg->valor = 120;

    // tree.insert(reg);

    // // Serializar a árvore para um arquivo
    // tree.serializeBPlusTree(tree, "bplustree.bin");
    

    // Desserializar a árvore do arquivo
    BPlusTree deserializedTree = deserializedTree.deserializeBPlusTree("bplustree.bin");

    // Imprimir as chaves da árvore desserializada
    // std::cout << "Chaves da árvore desserializada: ";
    // Node<RegArvore> r = deserializedTree.search(60);

    //     for (int i = 0; i < r.size; i++) {
    //         if (r.item[i].chave == 60) {
    //             cout << r.item[i].chave << endl;
    //             cout << r.item[i].valor << endl;
    //         }
            
    //     }
    deserializedTree.bpt_print();

    return 0;
}
