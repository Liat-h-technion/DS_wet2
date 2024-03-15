#ifndef DS_WET1_AVLTREE_H
#define DS_WET1_AVLTREE_H

#include <cmath>
#include <iostream>

#define DEFAULT (-1)


template<typename K, typename T>
class AVLTree {
private:
    class Node;
    Node* root;
    int size;
    void insertInner(const K& key, T* info, Node* curr, Node* parent);
    void eraseInner(const K& key, Node* curr, Node* parent);
    void leftLeftFix(Node* node, Node* parent);
    void leftRightFix(Node* node, Node* parent);
    void rightLeftFix(Node* node, Node* parent);
    void rightRightFix(Node* node, Node* parent);
    void rotateLeft(Node* node, Node* parent);
    void rotateRight(Node* node, Node* parent);
    void reBalanceSubTree(Node* node, Node* parent);
    int deleteLeavesFromRight(Node* node, Node* parent, int amount);
    Node* completeTreeInner(int height, Node* node, const K& default_key);
    void deAllocateAllInfoHelper(Node* node);
    int insertInorderToArrayHelper(Node* node, T**& array, int i);
    int insertInorderToTreeHelper(Node *node, T**& array, int i);
    template <typename keyFunction>
    int insertInorderWithConditionHelper(Node *node, T **&array, int arraySize, int i, int min_id, int max_id,
                                         keyFunction T::*get_key);
    template <typename Operation>
    void doOperationInorderHelper(Node* node, Operation T::*operation, int arg1, int arg2);

public:
    AVLTree() : root(nullptr), size(0) {};
    ~AVLTree();
    bool isEmpty() const;
    bool contains(const K& key) const;
    bool insert(const K& key, T* info);
    bool erase(const K& key);
    T* find(const K& key);
    const K& getMaxKey() const;
    const K& getMinKey() const;
    int getSize() const;
    T* getMaxKeyInfo() const;
    T* getMinKeyInfo() const;
    void deAllocateAllInfo();
    void nearlyCompleteTree(int wantedSize, const K& default_key);
    int insertInorderToArray(T**& array, int i);
    int insertInorderToTree(T**& array, int i);
    void swapTrees(AVLTree<K,T>& tree2);
    void clearTree(Node* node);
    template <typename keyFunction>
    void insertInorderWithCondition(T **&array, int arraySize, int min_id, int max_id, keyFunction T::* get_key);

    template <typename Operation>
    void doOperationInorder(Operation T::*operation, int arg1, int arg2);
};


template<typename K, typename T>
class AVLTree<K,T>::Node {
public:
    K key;
    T* info;
    Node* left;
    Node* right;
    int height;
    explicit Node(const K& default_key) : key(default_key), info(nullptr), left(nullptr), right(nullptr), height(0) {};
    Node(const K& key, T* info) : key(key), info(info), left(nullptr), right(nullptr), height(0) {};
    bool isLeaf() const;
    void swap(Node* other);
    Node* next();
    void updateHeight();
    int BalanceFactor() const;
    T* getInfo() const;
};


/* Complexity: time: O(log n), space: O(1)
 * Returns reference to the max key in the tree. (log(n))
 * The function assumes the tree is not empty
 */
template<typename K, typename T>
const K &AVLTree<K, T>::getMaxKey() const {
    Node* curr = root;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr->key;
}


/* Complexity: time: O(log n), space: O(1)
 * Returns reference to the min key in the tree. (log(n))
 * The function assumes the tree is not empty
 */
template<typename K, typename T>
const K& AVLTree<K, T>::getMinKey() const{
    Node* curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->key;
}



/* Complexity: time: O(log n), space: O(1)
 * Returns the info of the max key in the tree. (log(n))
 * The function assumes the tree is not empty
 */
template<typename K, typename T>
T *AVLTree<K, T>::getMaxKeyInfo() const {
    Node* curr = root;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr->getInfo();
}


/* Complexity: time: O(log n), space: O(1)
 * Returns the info of the min key in the tree. (log(n))
 * The function assumes the tree is not empty
 */
template<typename K, typename T>
T *AVLTree<K, T>::getMinKeyInfo() const {
    Node* curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->getInfo();
}


/* Complexity: time: O(1), space: O(1)
 * Returns the trees size. */
template<typename K, typename T>
int AVLTree<K, T>::getSize() const{
    return size;
}



/* Complexity: time: O(1), space: O(1)
 * Swaps the key and info between two nodes.
 * This action will likely defy the search property and requires re-arranging the tree.
 */
template<typename K, typename T>
void AVLTree<K,T>::Node::swap(Node *other) {
    T* temp_info = this->info;
    this->info = other->info;
    other->info = temp_info;

    K temp_key = this->key;
    this->key = other->key;
    other->key = temp_key;
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
bool AVLTree<K,T>::Node::isLeaf() const {
    return ( (right == nullptr) & (left == nullptr));
}


/* Complexity: time: O(1), space: O(1)
 * Updates the height of the node from its right and left sons heights.
 */
template<typename K, typename T>
void AVLTree<K,T>::Node::updateHeight() {
    if (isLeaf()) {
        height = 0;
    }
    else if (right == nullptr) {
        height = left->height + 1;
    }
    else if (left == nullptr) {
        height = right->height + 1;
    }
    else {
        height = std::max(right->height, left->height) + 1;
    }
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
int AVLTree<K,T>::Node::BalanceFactor() const {
    if (isLeaf()) {
        return 0;
    }
    else if (right == nullptr) {
        return (left->height + 1);
    }
    else if (left == nullptr) {
        return (- (right->height + 1));
    }
    return (left->height - right->height);
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
T* AVLTree<K,T>::Node::getInfo() const {
    return this->info;
}


/* Complexity: time: O(log n), space: O(1)
 */
template<typename K, typename T>
bool AVLTree<K,T>::contains(const K& key) const {
    Node* curr = root;
    while (curr != nullptr) {
        if (curr->key == key) {
            return true;
        }
        else if (curr->key > key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return false;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
bool AVLTree<K,T>::isEmpty() const {
    if (size == 0) {
        return true;
    }
    return false;
}

/* Complexity: time: O(log n), space: O(log n)
 */
template<typename K, typename T>
bool AVLTree<K,T>::insert(const K& key, T *info) {
    if (contains(key)) { // time: O(log n)
        return false;
    }
    if (root == nullptr) {
        Node* newNode = new Node(key, info);
        root = newNode;
        size += 1;
        return true;
    }
    insertInner(key, info, root, nullptr);
    size += 1;
    return true;
}


/* Complexity: time: O(log n), space: O(log n)
 */
template<typename K, typename T>
void AVLTree<K,T>::insertInner(const K& key, T* info, AVLTree::Node *curr, AVLTree::Node *parent) {
    if (curr->key > key) {
        if (curr->left == nullptr) {
            // Add leaf as left son
            Node* newNode = new Node(key, info);
            curr->left = newNode;
        }
        else {
            // Continue down to left subtree
            insertInner(key, info, curr->left, curr);
        }
    }
    else {
        if (curr->right == nullptr) {
            // Add leaf as right son
            Node* newNode = new Node(key, info);
            curr->right = newNode;
        }
        else {
            // Continue down to right subtree
            insertInner(key, info, curr->right, curr);
        }
    }
    curr->updateHeight();
    reBalanceSubTree(curr, parent);
}


/* Complexity: time: O(log n), space: O(log n)
 */
template<typename K, typename T>
bool AVLTree<K,T>::erase(const K &key) {
    if (!contains(key)) {
        return false;
    }
    eraseInner(key, root, nullptr);
    size -= 1;
    return true;
}


/* Complexity: time: O(log n), space: O(log n)
 */
template<typename K, typename T>
void AVLTree<K,T>::eraseInner(const K &key, AVLTree::Node *curr, AVLTree::Node *parent) {
    // Found the node to remove:
    if (key == curr->key) {

        // First case: curr is a leaf
        if (curr->isLeaf()) {
            // disconnect the node from its parent:
            if (parent != nullptr) {
                if (curr == parent->left) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            else {
                // curr is the root and has no sons
                root = nullptr;
            }
            delete curr;
            return;
        }

        // Second case: curr has one son
        else if (curr->left == nullptr || curr->right == nullptr) {
            // disconnect the node from its parent:
            if (parent != nullptr) {
                if (parent->left->key == curr->key) {
                    // curr is the left child of its parent
                    if (curr->left != nullptr) {
                        parent->left = curr->left;
                    }
                    else {
                        parent->left = curr->right;
                    }
                }
                else {
                    // curr is the right child of its parent
                    if (curr->left != nullptr) {
                        parent->right = curr->left;
                    }
                    else {
                        parent->right = curr->right;
                    }
                }
            }
            else {
                // curr is the root and has one son:
                if (curr->left != nullptr) {
                    root = curr->left;
                }
                else {
                    root = curr->right;
                }
            }
            delete curr;
            return;
        }

        // Third case: curr has two sons
        else {
            Node* nextNode = curr->next();
            curr->swap(nextNode);
            eraseInner(key, curr->right, curr);
        }
    }

    // If the node to remove not yet found, continue searching down the tree:
    else if (curr->key > key) {
        eraseInner(key, curr->left, curr);
    }
    else {
        eraseInner(key, curr->right, curr);
    }
    curr->updateHeight();
    reBalanceSubTree(curr, parent);
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::reBalanceSubTree(AVLTree::Node *node, AVLTree::Node *parent) {
    if (node->BalanceFactor() == 2) {
        if (node->left->BalanceFactor() > -1) {
            leftLeftFix(node, parent);
        }
        else {
            leftRightFix(node, parent);
        }
    }
    else if (node->BalanceFactor() == -2) {
        if (node->right->BalanceFactor() < 1) {
            rightRightFix(node, parent);
        }
        else {
            rightLeftFix(node, parent);
        }
    }
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
AVLTree<K,T>::~AVLTree() {
    clearTree(root);
    root = nullptr;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::leftLeftFix(AVLTree::Node *node, AVLTree::Node *parent) {
    rotateRight(node, parent);
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::leftRightFix(AVLTree::Node *node, AVLTree::Node *parent) {
    rotateLeft(node->left, node);
    rotateRight(node, parent);
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::rightRightFix(AVLTree::Node *node, AVLTree::Node *parent) {
    rotateLeft(node, parent);
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::rightLeftFix(AVLTree::Node *node, AVLTree::Node *parent) {
    rotateRight(node->right, node);
    rotateLeft(node, parent);
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::rotateLeft(AVLTree::Node *node, AVLTree::Node *parent) {
    if (node == root) {
        root = node->right;
    }
    Node* tmpParent = node->right;
    Node* tmpSon = node->right->left;
    node->right->left = node;
    node->right = tmpSon;
    if (parent != nullptr) {
        if (parent->key > node->key) {
            parent->left = tmpParent;
        } else {
            parent->right = tmpParent;
        }
    }
    node->updateHeight();
    tmpParent->updateHeight();
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K,T>::rotateRight(AVLTree::Node *node, AVLTree::Node *parent) {
    if (node == root) {
        root = node->left;
    }
    Node* tmpParent = node->left;
    Node* tmpSon = node->left->right;
    node->left->right = node;
    node->left = tmpSon;
    if (parent != nullptr) {
        if (parent->key > node->key) {
            parent->left = tmpParent;
        } else {
            parent->right = tmpParent;
        }
    }
    node->updateHeight();
    tmpParent->updateHeight();
}


/* Complexity: time: O(log n), space: O(1)
 * Returns pointer to the next node in-order. If the node is last in the tree, returns nullptr.
 */
template<typename K, typename T>
typename AVLTree<K,T>::Node *AVLTree<K,T>::Node::next() {
    Node* curr = this->right;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}


/* Complexity: time: O(log n), space: O(1)
 */
template<typename K, typename T>
T *AVLTree<K,T>::find(const K& key) {
    Node* curr = root;
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr->info;
        }
        else if (curr->key > key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
    return nullptr;
}

/* Complexity: time: O(n), space: O(log n)
 * Deletes the wanted amount of leaves in the subtree of "node" from right to left, using reversed-Inorder traversal.
 */
template<typename K, typename T>
int AVLTree<K,T>::deleteLeavesFromRight(AVLTree::Node* node, AVLTree::Node* parent, int amount) {
    if (amount == 0 || node == nullptr) {
        return amount;
    }
    int newAmount = deleteLeavesFromRight(node->right, node, amount);
    if ( (newAmount > 0) & node->isLeaf()) {
        if (parent != nullptr) {
            // Disconnect the node from its parent:
            if (parent->right != nullptr) {
                // Removing from right to left: If parent has a right child, the node to be removed is right child.
                parent->right = nullptr;
            }
            else {
                parent->left = nullptr;
            }
            delete node;
            this->size -= 1;
            return newAmount - 1;
        }
    }
    if (newAmount > 0) {
        return deleteLeavesFromRight(node->left, node, newAmount);
    }
    return newAmount;
}


/* Complexity: time: O(n), space: O(log n)
 * Fills the array with the elements of the tree in-order.
 * The function assumes the array size is at least the tree size.
 */
template<typename K, typename T>
int AVLTree<K, T>::insertInorderToArray(T**& array, int i) {
    return insertInorderToArrayHelper(root, array, i);
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
int AVLTree<K, T>::insertInorderToArrayHelper(AVLTree::Node *node, T**& array, int i) {
    if (node == nullptr) {
        return i;
    }
    i = insertInorderToArrayHelper(node->left, array, i);
    array[i] = node->info;
    i++;
    i = insertInorderToArrayHelper(node->right, array, i);
    return i;
}


/* Complexity: time: O(size_a+size_b), space: O(1)
 */
template<typename T, typename keyFunction>
int mergeSorted(T**& mergedArray, T**& array_a, int size_a, T**& array_b, int size_b, keyFunction get_key) {
    int ia = 0, ib = 0, i = 0;
    while ( (ia < size_a) & (ib < size_b)) {
        if ( (array_b[ib]->*get_key)() > (array_a[ia]->*get_key)()) {
            mergedArray[i] = array_a[ia];
            ia++;
        }
        else {
            mergedArray[i] = array_b[ib];
            if ((array_a[ia]->*get_key)() == (array_b[ib]->*get_key)()) {
                ia++;
            }
            ib++;
        }
        i++;
    }
    for (; ia < size_a; ia++, i++) {
        mergedArray[i] = array_a[ia];
    }
    for (; ib < size_b; ib++, i++) {
        mergedArray[i] = array_b[ib];
    }
    return i;
}


/* Complexity: time: O(n), space: O(log n) (where n is the size of array, assuming size of array ~= size of trees)
 */
template<typename K, typename T>
void fillArrayFrom3Trees(AVLTree<K,T>* trees, T**& array) {
    // traverse inorder on each tree and insert the info to the array
    int last_position = 0;
    for (int i=0; i<=2; i++) {
        last_position = trees[i].insertInorderToArray(array, last_position);
    }
}

/* Complexity: time: O(size1+size2+size3), space: O(log(size1)+log(size2)+log(size3))
 */
template<typename K, typename T, typename keyFunction>
int fillArrayFrom3TreesUnsorted(AVLTree<K,T>* trees, int size1, int size2, int size3, T**& array, keyFunction get_key) {
    // Traverse inorder on each tree and insert the info to the array
    // Each tree is sorted but the trees are not sorted between them
    T** array1 = new T*[size1];
    int mergedSize = trees[0].insertInorderToArray(array1, 0);
    if (size2 > 0) {
        T** array2 = new T*[size2];
        trees[1].insertInorderToArray(array2, 0);
        T** mergedArray1 = new T*[size1+size2];
        // merge array1 with array2 into a sorted array
        mergedSize = mergeSorted<T>(mergedArray1, array1, size1, array2, size2, get_key);

        if (size3 > 0) {
            T** array3 = new T*[size3];
            trees[2].insertInorderToArray(array3, 0);
            // merge array1+array2 with array3 into a sorted array
            mergedSize = mergeSorted<T>(array, mergedArray1, mergedSize, array3, size3, get_key);
            delete[] array3;
        }
        delete[] array2;
        delete[] mergedArray1;
    }
    delete[] array1;
    return mergedSize;
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
int AVLTree<K, T>::insertInorderToTree(T**& array, int i){
    return insertInorderToTreeHelper(root, array, i);
}


/* Complexity: time: O(n), space: O(log n) assuming the complexity of getId() is O(1).
 */
template<typename K, typename T>
int AVLTree<K, T>::insertInorderToTreeHelper(AVLTree::Node *node, T**& array, int i){
    if (node == nullptr){
        return i;
    }
    i = insertInorderToTreeHelper(node->left, array, i);
    node->key = array[i]->getId();
    node->info = array[i];
    i++;
    i = insertInorderToTreeHelper(node->right, array, i);
    return i;
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
void fill3TreesFromArray(AVLTree<K,T>* newTrees, T**& array){
    int last_position = 0;
    for(int i = 0; i < 3; i++){
        last_position = newTrees[i].insertInorderToTree(array, last_position);
    }
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T, typename keyFunction>
void fill3TreesWithCondition(AVLTree<K,T>* newTrees, T**& array, int arraySize, int (&min_ids)[3], int (&max_ids)[3],
                             keyFunction T::* get_key){
    for(int i = 0; i < 3; i++){
        newTrees[i].insertInorderWithCondition(array, arraySize, min_ids[i], max_ids[i], get_key);
    }
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
template <typename keyFunction>
void AVLTree<K, T>::insertInorderWithCondition(T**& array, int arraySize, int min_id, int max_id, keyFunction T::* get_key){
    insertInorderWithConditionHelper(root, array, arraySize, 0, min_id, max_id, get_key);
}


/* Complexity: time: O(treeSize+ArraySize), space: O(log(treeSize)+log(ArraySize)) assuming the complexity of getId() and get_key is O(1).
 */
template<typename K, typename T>
template <typename keyFunction>
int AVLTree<K, T>::insertInorderWithConditionHelper(AVLTree::Node *node, T**& array, int arraySize, int i, int min_id,
                                                    int max_id, keyFunction T::* get_key){
    if (node == nullptr){
        return i;
    }
    i = insertInorderWithConditionHelper(node->left, array, arraySize, i, min_id, max_id, get_key);
    while (i < arraySize) {
        if ( (array[i]->getId() > min_id) & (array[i]->getId() <= max_id) ) {
            node->key = (array[i]->*get_key)();
            node->info = array[i];
            i++;
            break;
        }
        i++;
    }
    i = insertInorderWithConditionHelper(node->right, array, arraySize, i, min_id, max_id, get_key);
    return i;
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
void AVLTree<K,T>::clearTree(AVLTree::Node* node) {
    if (node == nullptr) {
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
    size -= 1;
}


/* Complexity: time: O(n), space: O(log n)
 * This function should be called from an empty tree, and adds nodes to get a nearly-complete tree of empty nodes.
 * Empty nodes have "default_key" as the key, and null as the info.
 */
template<typename K, typename T>
void AVLTree<K,T>::nearlyCompleteTree(int wantedSize, const K& default_key) {
    int height = static_cast<int>(ceil(log2(wantedSize + 1)) - 1);
    root = completeTreeInner(height, root, default_key);
    size = static_cast<int>(pow(2.0, (height+1)) - 1.0);
    this->deleteLeavesFromRight(root, nullptr, size - wantedSize);
}


/* Complexity: time: O(n), space: O(log n)
 * Helper function that fill an empty tree (with no nodes) to a complete tree of empty nodes.
 */
template<typename K, typename T>
typename AVLTree<K,T>::Node* AVLTree<K,T>::completeTreeInner(int height, AVLTree::Node *node, const K& default_key) {
    Node* newNode = new Node(default_key);
    newNode->height = height;
    if (height == 0) {
        return newNode;
    }
    newNode->left = completeTreeInner(height-1, newNode->left, default_key);
    newNode->right = completeTreeInner(height-1, newNode->right, default_key);
    return newNode;
}

/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
void AVLTree<K, T>::deAllocateAllInfo() {
    deAllocateAllInfoHelper(root);
}

/* Complexity: time: O(n), space: O(log n)
 * De-allocates the info of all the nodes in the tree using postorder traversal.
 * Should only be used by the owner of the info's memory.
 */
template<typename K, typename T>
void AVLTree<K, T>::deAllocateAllInfoHelper(AVLTree::Node* node) {
    if (node == nullptr) {
        return;
    }
    deAllocateAllInfoHelper(node->left);
    deAllocateAllInfoHelper(node->right);
    T* info = node->info;
    node->info = nullptr;
    delete info;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void AVLTree<K, T>::swapTrees(AVLTree<K, T> &tree2) {
    Node* tempRoot = this->root;
    this->root = tree2.root;
    tree2.root = tempRoot;

    int tempSize = this->size;
    this->size = tree2.size;
    tree2.size = tempSize;
}


/* Complexity: time: O(n), space: O(log n) (assuming the operation complexity is O(1))
 */
template<typename K, typename T>
template<typename Operation>
void AVLTree<K, T>::doOperationInorder(Operation T::*operation, int arg1, int arg2) {
    doOperationInorderHelper(root, operation, arg1, arg2);
}


/* Complexity: time: O(n), space: O(log n) (assuming the operation complexity is O(1))
 */
template<typename K, typename T>
template<typename Operation>
void AVLTree<K, T>::doOperationInorderHelper(AVLTree::Node *node, Operation T::*operation, int arg1, int arg2) {
    if (node == nullptr) {
        return;
    }
    doOperationInorderHelper(node->left, operation, arg1, arg2);
    (node->info->*operation)(arg1, arg2);
    doOperationInorderHelper(node->right, operation, arg1, arg2);
}


#endif //DS_WET1_AVLTREE_H