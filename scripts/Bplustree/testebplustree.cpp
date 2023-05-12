#include "bplustree.h"

int main()
{
    BPTree tree(ORDER);

    // Inserir alguns valores na árvore
    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(30, 300);
    tree.insert(40, 400);
    tree.insert(50, 500);

    cout << "Arvore B+ criada: \n"
         << endl;
    int tam = tree.calculateSize(tree.getRoot());
    cout << "Tamanho em bytes da árvore B+ inicial:" << tam << endl;
    tree.display(tree.getRoot(), 0);

    cout << "-----------------------------------" << endl;

    // Serializar a árvore em um arquivo binário
    tree.serializeTree("bptree.bin");
    cout << "Arquivo bptree.bin criado." << endl;
    cout << "-----------------------------------" << endl;

    // Limpar a árvore existente
    delete tree.getRoot();
    tree.root = nullptr;

    BPTree tree2(ORDER);
    // Desserializar a árvore a partir do arquivo binário
    cout << "Desserializando a árvore B+ do arquivo bptree.bin..." << endl;
    tree2.deserializeTree("bptree.bin");
    cout << "Árvore B+ desserializada.\n" << endl;

    // Exibir a árvore desserializada
    tam = tree2.calculateSize(tree2.getRoot());
    cout << "Tamanho em bytes da árvore B+ desserializada:" << tam << endl;
    tree2.display(tree2.getRoot(), 0);


    return 0;
}

// int main()
// {
//     BPTree node(255);
//     node.insert(5, 10);
//     node.insert(15, 20);
//     node.insert(25, 30);
//     node.insert(35, 40);
//     node.insert(45, 50);
//     node.insert(55, 60);
//     node.insert(40, 70);
//     node.insert(30, 80);
//     node.insert(20, 90);
//     cout << "Arvore B+ criada: \n"
//          << endl;
//     int tam = node.calculateSize(node.getRoot());
//     cout << endl;
//     cout << "Tamanho em bytes da árvore B+ criada:" << tam << endl;
//     cout << endl;

//     cout << "Registros na Arvore B+ em ordem crescente: \n"
//          << endl;
//     node.display(node.getRoot(), 0);

//     //node.search(15);
// }