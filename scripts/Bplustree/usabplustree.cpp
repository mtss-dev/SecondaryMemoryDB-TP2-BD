#include "bplustree.h"

int main() {
    // Criar e construir a árvore B+
    BPlusTree tree;

    // Inserir registros na árvore
    tree.insert(no_registro(10, 1));
    tree.insert(no_registro(5, 2));
    tree.insert(no_registro(15, 3));
    tree.insert(no_registro(3, 4));
    tree.insert(no_registro(8, 5));
    tree.insert(no_registro(12, 6));
    tree.insert(no_registro(20, 7));
    tree.insert(no_registro(14, 8));
    tree.insert(no_registro(18, 9));
    tree.insert(no_registro(22, 10));
    tree.insert(no_registro(25, 11));

    // Serializar a árvore
    vector<char> serializedData = tree.serialize();

    // Armazenar a estrutura serializada em um arquivo binário
    ofstream outFile("bplustree.bin", ios::binary);
    if (!outFile) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return 1;
    }

    outFile.write(serializedData.data(), serializedData.size());
    outFile.close();

    // Desserializar a árvore a partir do arquivo binário
    ifstream inFile("bplustree.bin", ios::binary);
    if (!inFile) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    // Obter o tamanho do arquivo
    inFile.seekg(0, ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    // Ler os dados do arquivo
    vector<char> fileData(fileSize);
    inFile.read(fileData.data(), fileSize);
    inFile.close();

    // Desserializar a árvore
    BPlusTree deserializedTree;
    deserializedTree.deserialize(fileData);

    // Realizar operações na árvore desserializada
        // Busca de registros
    int keys[] = {10, 15, 3, 8, 14, 22, 25, 3899, 129238, 229};
    for (int key : keys) {
        bool found = deserializedTree.search(deserializedTree.getRoot(), key);
        if (found) {
            cout << "-----------------------------------" << endl;
            cout << "Registro com id: " << key << " encontrado." << endl;
            cout << "Endereço do Bloco: " << deserializedTree.getBlockAddr(key) << endl;
        
        } else {
            cout << "-----------------------------------" << endl;
            std::cout << "Registro com chave " << key << " não encontrado." << std::endl;
        }
    }
    cout << "-----------------------------------" << endl;

    return 0;
}