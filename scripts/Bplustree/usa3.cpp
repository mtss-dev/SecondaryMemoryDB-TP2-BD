#include "bplustree3.h"

int main()
{
    BPTree node;
    node.insert(5, 10);
    node.insert(15, 20);
    node.insert(25, 30);
    node.insert(35, 40);
    node.insert(45, 50);
    node.insert(55, 60);
    node.insert(40, 70);
    node.insert(30, 80);
    node.insert(20, 90);
    cout << "Arvore B+ criada: \n"
         << endl;
    int tam = node.calculateSize(node.getRoot());
    cout << endl;
    cout << "Tamanho em bytes da árvore B+ criada:" << tam << endl;
    cout << endl;

    cout << "Registros na Arvore B+ em ordem crescente: \n"
         << endl;
    node.display(node.getRoot(), 0);

    //node.search(15);
}