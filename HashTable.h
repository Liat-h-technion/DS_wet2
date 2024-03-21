#ifndef DS_WET2_HASHTABLE_H
#define DS_WET2_HASHTABLE_H

#include "AVLTree.h"

template<typename T>
class HashTable {
private:
    int size;
    int used_size;
    AVLTree<int, T>* table;
    const double MIN_LOAD = 0.25;
    const int INIT_SIZE = 4;

    void resize();
    int hashKey(int key);

public:
    HashTable();
    ~HashTable();
    void insert(int key, T* info);
    void erase(int key);
    T* find(int key);
    bool isEmpty() const;
    void deAllocateAllInfo();
};

/* Complexity: time: O(n), space: O(1)
 */
template<typename T>
void HashTable<T>::deAllocateAllInfo() {
    for (int i=0; i<size; i++) {
        table[i].deAllocateAllInfo();
    }
}

template<typename T>
bool HashTable<T>::isEmpty() const {
    return used_size ==0;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename T>
HashTable<T>::HashTable() {
    size = INIT_SIZE;
    used_size = 0;
    table = new AVLTree<int, T>[INIT_SIZE];
}


/* Complexity: time: O(n), space: O(1)
 */
template<typename T>
HashTable<T>::~HashTable() {
    delete[] table;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename T>
int HashTable<T>::hashKey(int key) {
    return key % size;
}


/* Complexity: time: O(1) Amortized on average, space: O(1)
 * If item was not found, returns nullptr
 */
template<typename T>
T* HashTable<T>::find(int key) {
    int hashed_key = hashKey(key);
    return table[hashed_key].find(key);
}


/* Complexity: time: O(1) Amortized on average, space: O(n)
 */
template<typename T>
void HashTable<T>::insert(int key, T* info) {
    int hashed_key = hashKey(key);
    table[hashed_key].insert(key, info);
    used_size += 1;
    resize();
}


/* Complexity: time: O(1) Amortized on average, space: O(1)
 */
template<typename T>
void HashTable<T>::erase(int key) {
    int hashed_key = hashKey(key);
    table[hashed_key].erase(key);
    used_size -= 1;
    resize();
}


/* Complexity: time: O(1) Amortized on average, space: O(n)
 */
template<typename T>
void HashTable<T>::resize() {
    if ((used_size < size) && (used_size > size*MIN_LOAD || size == INIT_SIZE)) {
        return;
    }

    AVLTree<int, T>* new_table;
    int old_size = size;
    if ((used_size >= size)) {
        size *= 2;
        new_table = new AVLTree<int, T>[size];
    }
    else {
        size /= 2;
        new_table = new AVLTree<int, T>[size];
    }

    // Move all items to the new table
    for (int i = 0; i < old_size; i++) {
        while (!table[i].isEmpty()) {
            int root_key = table[i].getRootKey();
            new_table[hashKey(root_key)].insert(root_key, table[i].getRootInfo());
            table[i].erase(root_key);
        }
    }
    AVLTree<int, T>* to_delete = table;
    table = new_table;
    delete[] to_delete;
}

#endif //DS_WET2_HASHTABLE_H
