#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

// Searching on a B+ tree in C++

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
int MAX = 3;

struct registro{
    int id;
    int bloco_addr;
};

// BP node
class Node {
  bool IS_LEAF;
  registro *key;
  int size;
  Node **ptr;
  friend class BPTree;

   public:
  Node();
};

// BP tree
class BPTree {
  Node *root;
  void insertInternal(registro , Node *, Node *);
  Node *findParent(Node *, Node *);

   public:
  BPTree();
  void search(int);
  void insert(int,int);
  void display(Node *, int);
  Node *getRoot();
  size_t calculateSize(Node *);
};

Node::Node() {
  key = new registro[MAX];
  ptr = new Node *[MAX + 1];
}

BPTree::BPTree() {
  root = NULL;
}

// Search operation
void BPTree::search(int x) {
  if (root == NULL) {
    cout << "Tree is empty\n";
  } else {
    Node *cursor = root;
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i].id) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++) {
        if (cursor->key[i].id == x) {
            cout << "Found\n";
            return;
        }
    }
    cout << "Not found\n";
  }
}

// Insert Operation
void BPTree::insert(int x, int y) {
  if (root == NULL) {
    root = new Node;
    root->key[0].id = x;
    root->key[0].bloco_addr = y;
    root->IS_LEAF = true;
    root->size = 1;
  } else {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false) {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i].id) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < MAX) {
      int i = 0;
      while (x > cursor->key[i].id && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--) {
        cursor->key[j] = cursor->key[j - 1];
      }
      cursor->key[i].id = x;
      cursor->key[i].bloco_addr = y;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } else {
      Node *newLeaf = new Node;
      registro virtualNode[MAX + 1];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];
      }
      int i = 0, j;
      while (x > virtualNode[i].id && i < MAX)
        i++;
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i].id = x;
      virtualNode[i].bloco_addr = y;
      newLeaf->IS_LEAF = true;
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = virtualNode[j];
      }
      if (cursor == root) {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}

// Insert Operation
void BPTree::insertInternal(registro x, Node *cursor, Node *child) {
  if (cursor->size < MAX) {
    int i = 0;
    while (x.id > cursor->key[i].id && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--) {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i].id = x.id;
    cursor->key[i].bloco_addr = x.bloco_addr;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } else {
    Node *newInternal = new Node;
    registro virtualKey[MAX + 1];
    Node *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++) {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x.id > virtualKey[i].id && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--) {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 2; j > i + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) {
      Node *newRoot = new Node;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
    }
  }
}

// Find the parent
Node *BPTree::findParent(Node *cursor, Node *child) {
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->ptr[i] == child) {
      parent = cursor;
      return parent;
    } else {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

// Print the tree
void BPTree::display(Node *cursor, int last) {
    int lastId = last; // Variável auxiliar para armazenar o último ID impresso
  if (cursor != NULL) {
    if (cursor->IS_LEAF) {
      for (int i = 0; i < cursor->size; i++) {
        if (cursor->key[i].id != lastId) {
          cout << "ID: " << cursor->key[i].id << ", Block Addr: " << cursor->key[i].bloco_addr << endl;
          lastId = cursor->key[i].id;
        }
      }
    } else {
      int i = 0;
      while (i < cursor->size) {
        display(cursor->ptr[i], lastId);
        if (cursor->key[i].id != lastId) {
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
size_t BPTree::calculateSize(Node *cursor) {
  if (cursor == nullptr) {
    return 0;
  }

  size_t nodeSize = sizeof(Node);  // Tamanho da estrutura do nó
  size_t keySize = sizeof(registro);    // Tamanho da estrutura da chave
  
  size_t size = nodeSize + (cursor->size * keySize);

  if (!cursor->IS_LEAF) {
    for (int i = 0; i <= cursor->size; i++) {
      size += calculateSize(cursor->ptr[i]);
    }
    cout << "Node size: " << nodeSize << " | Keys size: " << cursor->size * keySize << endl; // DEBUG
  }
  cout << "Node size: " << nodeSize << " | Keys size: " << cursor->size * keySize << endl; // DEBUG
  return size;
}


// Get the root
Node *BPTree::getRoot() {
  return root;
}



#endif