#ifndef DS_WET2_HASHTABLE_H

#include "AVLTree.h"

template<typename K, typename T>
class HashTable {
private:
    int size;
    int used_size;
    AVLTree<K, T>[] table;
}

#define DS_WET2_HASHTABLE_H

#endif //DS_WET2_HASHTABLE_H
