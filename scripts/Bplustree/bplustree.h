#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include "queue"
#include "../Constantes/constantes.h"

using namespace std;

struct bp_registro
{
  int id;
  int bloco_addr;
};

// BP node
class Node
{
public:
  bool IS_LEAF;
  bp_registro *key;
  int size;
  Node **ptr;

  Node();
};

// BP tree
class BPTree
{
public:
  //atributos da árvore B+
  Node *root;
  int order;

  //função construtora
  BPTree(int order)
  {
    root = nullptr;
    this->order = order;
    MAX = 2 * order;
  }
  //funções para serialização
  void serializeNode(ofstream &file, Node *node);
  void serializeTree(const string &filename);
  void deserializeTree(const string &filename);

  void deleteTree(Node *node);
  
  //funções da árvore B+
  void search(int);
  void insert(int, int);
  void display(Node *, int);
  Node *getRoot();
  size_t calculateSize(Node *);
  void insertInternal(bp_registro, Node *, Node *);
  Node *findParent(Node *, Node *);
};

Node::Node() {
  key = new bp_registro[MAX + 1];
  ptr = new Node *[MAX + 1 + 1];  // +1 para o filho à esquerda de cada chave, +1 para o último ponteiro
}

// Search operation
void BPTree::search(int x)
{
  if (root == nullptr)
  {
    cout << "Tree is empty\n";
    return;
  }

  Node *cursor = root;
  while (!cursor->IS_LEAF)
  {
    int i = 0;
    while (i < cursor->size)
    {
      if (x < cursor->key[i].id)
        break;
      i++;
    }

    if (i == cursor->size) // Verifica se o índice está fora dos limites
      i = cursor->size - 1; // Define o índice para o último ponteiro válido

    cursor = cursor->ptr[i];
  }

  for (int i = 0; i < cursor->size; i++)
  {
    if (cursor->key[i].id == x)
    {
      cout << "Found\n";
      return;
    }
  }

  cout << "Not found\n";
}


// Insert Operation
void BPTree::insert(int x, int y)
{
  if (root == NULL)
  {
    root = new Node;
    root->key[0].id = x;
    root->key[0].bloco_addr = y;
    root->IS_LEAF = true;
    root->size = 1;
  }
  else
  {
    if(x == 136421){
      cout << "oi";
    }
    Node *cursor = root;
    Node* parent = nullptr;
    while (cursor->IS_LEAF == false)
      {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++)
      {
        if (x < cursor->key[i].id)
        {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1)
        {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
      if (cursor == nullptr)
        {
          break;  // Verificação adicionada para evitar acessar um ponteiro nulo
        }
      }

    if (cursor != nullptr){
      if (cursor->size < MAX)
      {
        int i = 0;
        while (x > cursor->key[i].id && i < cursor->size)
          i++;
        for (int j = cursor->size; j > i; j--)
        {
          cursor->key[j] = cursor->key[j - 1];
        }
        cursor->key[i].id = x;
        cursor->key[i].bloco_addr = y;
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
      }
      else
      {
        Node *newLeaf = new Node;
        bp_registro virtualNode[MAX + 1];
        for (int i = 0; i < MAX; i++)
        {
          virtualNode[i] = cursor->key[i];
        }
        int i = 0, j;
        while (x > virtualNode[i].id && i < MAX)
          i++;
        for (int j = MAX + 1; j > i; j--)
        {
          virtualNode[j] = virtualNode[j - 1];
        }
        virtualNode[i].id = x;
        virtualNode[i].bloco_addr = y;
        newLeaf->IS_LEAF = true;
        cursor->size = (MAX - 1) / 2;
        newLeaf->size = MAX - cursor->size - 1;
        cursor->ptr[cursor->size] = newLeaf;
        newLeaf->ptr[newLeaf->size] = NULL; // Adicionar esta linha para inicializar o ponteiro corretamente
        cursor->ptr[MAX] = NULL;
        for (i = 0; i < cursor->size; i++)
        {
          cursor->key[i] = virtualNode[i];
        }
        for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
        {
          newLeaf->key[i] = virtualNode[j];
        }
        if (cursor == root)
        {
          Node *newRoot = new Node;
          newRoot->key[0] = newLeaf->key[0];
          newRoot->ptr[0] = cursor;
          newRoot->ptr[1] = newLeaf;
          newRoot->IS_LEAF = false;
          newRoot->size = 1;
          root = newRoot;
        }
        else
        {
          insertInternal(newLeaf->key[0], parent, newLeaf);
        }
      }
    }
  }
}

// Insert Operation
void BPTree::insertInternal(bp_registro x, Node *cursor, Node *child)
{
  if (cursor->size < MAX - 1)
  {
    int i = 0;
    while (x.id > cursor->key[i].id && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--)
    {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--)
    {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i].id = x.id;
    cursor->key[i].bloco_addr = x.bloco_addr;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  }
  else
  {
    Node *newInternal = new Node;
    bp_registro virtualKey[MAX + 1];
    Node *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++)
    {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++)
    {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x.id > virtualKey[i].id && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--)
    {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 1; j > i + 1; j--)
    {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (MAX - 1) / 2;
    newInternal->size = MAX - cursor->size - 1;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
    {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
    {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root)
    {
      Node *newRoot = new Node;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->ptr[0]->IS_LEAF = false;
      newRoot->ptr[1]->IS_LEAF = false;

      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    }
    else
    {
      insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
    }
  }
}

// Find the parent
Node *BPTree::findParent(Node *cursor, Node *child)
{
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
  {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++)
  {
    if (cursor->ptr[i] == child)
    {
      parent = cursor;
      return parent;
    }
    else
    {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

// Print the tree
void BPTree::display(Node *cursor, int last)
{
  int lastId = last; // Variável auxiliar para armazenar o último ID impresso
  if (cursor != NULL)
  {
    if (cursor->IS_LEAF)
    {
      for (int i = 0; i < cursor->size; i++)
      {
        if (cursor->key[i].id != lastId)
        {
          cout << "ID: " << cursor->key[i].id << ", Block Addr: " << cursor->key[i].bloco_addr << endl;
          lastId = cursor->key[i].id;
        }
      }
    }
    else
    {
      int i = 0;
      while (i < cursor->size)
      {
        display(cursor->ptr[i], lastId);
        if (cursor->key[i].id != lastId)
        {
          cout << "ID: " << cursor->key[i].id << ", Block Addr: " << cursor->key[i].bloco_addr << endl;
          lastId = cursor->key[i].id;
        }
        i++;
      }
      display(cursor->ptr[i], lastId);
    }
  }
}

// Função para calcular o tamanho em bytes da árvore B+Tree
size_t BPTree::calculateSize(Node *cursor)
{
  if (cursor == nullptr)
  {
    return 0;
  }

  size_t nodeSize = sizeof(Node);    // Tamanho da estrutura do nó
  size_t keySize = sizeof(bp_registro); // Tamanho da estrutura da chave

  size_t size = nodeSize + (cursor->size * keySize);

  if (!cursor->IS_LEAF)
  {
    for (int i = 0; i <= cursor->size; i++)
    {
      size += calculateSize(cursor->ptr[i]);
    }
    cout << "Node size: " << nodeSize << " | Keys size: " << cursor->size * keySize << endl; // DEBUG
  }
  cout << "Node size: " << nodeSize << " | Keys size: " << cursor->size * keySize << endl; // DEBUG
  return size;
}

// Get the root
Node *BPTree::getRoot()
{
  return root;
}

void BPTree::serializeTree(const string &filename)
{
    ofstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return;
    }
    serializeNode(file, root);
    file.close();
}

void BPTree::serializeNode(ofstream &file, Node *node)
{
    if (node == nullptr)
        return;

    file.write(reinterpret_cast<char *>(node), sizeof(Node));
    file.write(reinterpret_cast<char *>(node->key), node->size * sizeof(bp_registro));

    if (!node->IS_LEAF)
    {
        for (int i = 0; i <= node->size; i++)
        {
            serializeNode(file, node->ptr[i]);
        }
    }
}

void BPTree::deserializeTree(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        return;
    }

    deleteTree(root); // Limpar a árvore existente antes de desserializar

    while (true)
    {
        Node *node = new Node;
        if (!file.read(reinterpret_cast<char *>(node), sizeof(Node)))
            break; // Saia do loop se a leitura falhar

        // Alocar espaço para node->key
        node->key = new bp_registro[node->size];

        if (!file.read(reinterpret_cast<char *>(node->key), node->size * sizeof(bp_registro)))
            break; // Saia do loop se a leitura falhar

        node->ptr = new Node *[node->size + 1];

        if (root == nullptr)
            root = node;

        if (!node->IS_LEAF)
        {
            for (int i = 0; i <= node->size; i++)
            {
                Node *child = new Node;
                if (!file.read(reinterpret_cast<char *>(child), sizeof(Node)))
                    break; // Saia do loop se a leitura falhar

                // Alocar espaço para child->key
                child->key = new bp_registro[child->size];

                if (!file.read(reinterpret_cast<char *>(child->key), child->size * sizeof(bp_registro)))
                    break; // Saia do loop se a leitura falhar

                child->ptr = new Node *[child->size + 1];
                node->ptr[i] = child;
            }
        }
    }

    file.close();
}


void BPTree::deleteTree(Node *node)
{
    if (node == nullptr)
        return;

    if (!node->IS_LEAF)
    {
        for (int i = 0; i <= node->size; i++)
        {
            deleteTree(node->ptr[i]);
        }
    }

    delete[] node->key;
    delete[] node->ptr;
    delete node;
    node = nullptr;
}


#endif