#include "bplustree2.h"
#include <fstream>
#include <iostream> 

using namespace std;

int main() {
    BPTree bptree(3);
    bptree.insert({10, 100});
    bptree.insert({20, 200});
    bptree.insert({30, 300});
    bptree.insert({41, 400});
    bptree.insert({50, 500});

    bptree.serializeBPTree("bptree.bin");

    bptree.display(bptree.getRoot(), 0);

    BPTree loadedBPTree = bptree.deserializeBPTree("bptree.bin");

    loadedBPTree.display(loadedBPTree.getRoot(), 0);

    return 0;
}


