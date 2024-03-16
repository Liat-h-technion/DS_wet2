#ifndef DS_WET1_RankTree_H
#define DS_WET1_RankTree_H

#include <cmath>
#include <iostream>

#define DEFAULT (-1)


template<typename K, typename T>
class RankTree {
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
    void deAllocateAllInfoHelper(Node* node);

public:
    RankTree() : root(nullptr), size(0) {};
    ~RankTree();
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
    void clearTree(Node* node);
    int get_num_wins(const K& key);
    void add_wins(const K& key, int x);
    void add_wins_in_range(const K& min_key, const K& max_key, int x);
    int get_index_from_key(const K& key);
    int get_key_from_index(int idx);
};


template<typename K, typename T>
class RankTree<K,T>::Node {
public:
    K key;
    T* info;
    Node* left;
    Node* right;
    int height;
    int extra;
    int subtree_size;

    explicit Node(const K& default_key) : key(default_key), info(nullptr), left(nullptr), right(nullptr), height(0),
                                          extra(0), subtree_size(1) {};
    Node(const K& key, T* info) : key(key), info(info), left(nullptr), right(nullptr), height(0), extra(0), subtree_size(1) {};
    bool isLeaf() const;
    void swap(Node* other);
    Node* nextInSubtree();
    void updateHeight();
    void updateSubtreeSize();
    int BalanceFactor() const;
    T* getInfo() const;
};


template<typename K, typename T>
int RankTree<K, T>::get_num_wins(const K &key) {
    if (!find(key)) {
        return 0;
    }

    Node* curr = root;
    int wins = 0;
    while (curr != nullptr) {
        wins += curr->extra;
        if (curr->key == key) {
            return wins;
        }
        else if (curr->key > key) {
            curr = curr->left;
        }
        else {
            curr = curr->right;
        }
    }
}

template<typename K, typename T>
void RankTree<K, T>::Node::updateSubtreeSize() {
    subtree_size = 1;
    if (right) {
        subtree_size += right->subtree_size;
    }
    if (left) {
        subtree_size += left->subtree_size;
    }
}


/* Complexity: time: O(log n), space: O(1)
 * Returns reference to the max key in the tree. (log(n))
 * The function assumes the tree is not empty
 */
template<typename K, typename T>
const K &RankTree<K, T>::getMaxKey() const {
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
const K& RankTree<K, T>::getMinKey() const{
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
T *RankTree<K, T>::getMaxKeyInfo() const {
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
T *RankTree<K, T>::getMinKeyInfo() const {
    Node* curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->getInfo();
}


/* Complexity: time: O(1), space: O(1)
 * Returns the trees size. */
template<typename K, typename T>
int RankTree<K, T>::getSize() const{
    return size;
}



/* Complexity: time: O(1), space: O(1)
 * Swaps the key and info between two nodes.
 * This action will likely defy the search property and requires re-arranging the tree.
 */
template<typename K, typename T>
void RankTree<K,T>::Node::swap(Node *other) {
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
bool RankTree<K,T>::Node::isLeaf() const {
    return ( (right == nullptr) & (left == nullptr));
}


/* Complexity: time: O(1), space: O(1)
 * Updates the height of the node from its right and left sons heights.
 */
template<typename K, typename T>
void RankTree<K,T>::Node::updateHeight() {
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
int RankTree<K,T>::Node::BalanceFactor() const {
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
T* RankTree<K,T>::Node::getInfo() const {
    return this->info;
}


/* Complexity: time: O(log n), space: O(1)
 */
template<typename K, typename T>
bool RankTree<K,T>::contains(const K& key) const {
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
bool RankTree<K,T>::isEmpty() const {
    if (size == 0) {
        return true;
    }
    return false;
}

/* Complexity: time: O(log n), space: O(log n)
 */
template<typename K, typename T>
bool RankTree<K,T>::insert(const K& key, T *info) {
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
void RankTree<K,T>::insertInner(const K& key, T* info, RankTree::Node *curr, RankTree::Node *parent) {
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
    curr->updateSubtreeSize();
    reBalanceSubTree(curr, parent);
}


/* Complexity: time: O(log n), space: O(log n)
 */
template<typename K, typename T>
bool RankTree<K,T>::erase(const K &key) {
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
void RankTree<K,T>::eraseInner(const K &key, RankTree::Node *curr, RankTree::Node *parent) {
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
            // Update the "extra" in curr's son
            if (curr->left) {
                curr->left->extra += curr->extra;
            }
            else {
                curr->right->extra += curr->extra;
            }

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
            Node* nextNode = curr->nextInSubtree();
            // Swap the node with the next node in the subtree.
            // Update the extra to be the correct amount of wins for the next node
            int wins = get_num_wins(nextNode->key);
            int removed_node_wins = get_num_wins(curr->key);
            curr->swap(nextNode);
            int diff = wins - removed_node_wins;
            curr->extra += diff;
            // Subtract the diff in "extra" in curr node from the sons
            if (curr->right) {
                curr->right->extra -= diff;
            }
            if (curr->left) {
                curr->left->extra -= diff;
            }

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
    curr->updateSubtreeSize();
    reBalanceSubTree(curr, parent);
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::reBalanceSubTree(RankTree::Node *node, RankTree::Node *parent) {
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
RankTree<K,T>::~RankTree() {
    clearTree(root);
    root = nullptr;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::leftLeftFix(RankTree::Node *node, RankTree::Node *parent) {
    rotateRight(node, parent);
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::leftRightFix(RankTree::Node *node, RankTree::Node *parent) {
    rotateLeft(node->left, node);
    rotateRight(node, parent);
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::rightRightFix(RankTree::Node *node, RankTree::Node *parent) {
    rotateLeft(node, parent);
}

/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::rightLeftFix(RankTree::Node *node, RankTree::Node *parent) {
    rotateRight(node->right, node);
    rotateLeft(node, parent);
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::rotateLeft(RankTree::Node *node, RankTree::Node *parent) {
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

    // Update subtree size:
    node->updateSubtreeSize();
    tmpParent->updateSubtreeSize();
    // Update the "extra":
    tmpParent->extra += node->extra;
    node->extra -= tmpParent->extra;
    tmpSon->extra -= node->extra;
}


/* Complexity: time: O(1), space: O(1)
 */
template<typename K, typename T>
void RankTree<K,T>::rotateRight(RankTree::Node *node, RankTree::Node *parent) {
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

    // Update subtree size:
    node->updateSubtreeSize();
    tmpParent->updateSubtreeSize();
    // Update the "extra":
    tmpParent->extra += node->extra;
    node->extra -= tmpParent->extra;
    tmpSon->extra -= node->extra;
}


/* Complexity: time: O(log n), space: O(1)
 * Returns pointer to the next node in-order. If the node is last in the tree, returns nullptr.
 */
template<typename K, typename T>
typename RankTree<K,T>::Node *RankTree<K,T>::Node::nextInSubtree() {
    Node* curr = this->right;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}


/* Complexity: time: O(log n), space: O(1)
 */
template<typename K, typename T>
T *RankTree<K,T>::find(const K& key) {
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
 */
template<typename K, typename T>
void RankTree<K,T>::clearTree(RankTree::Node* node) {
    if (node == nullptr) {
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
    size -= 1;
}


/* Complexity: time: O(n), space: O(log n)
 */
template<typename K, typename T>
void RankTree<K, T>::deAllocateAllInfo() {
    deAllocateAllInfoHelper(root);
}

/* Complexity: time: O(n), space: O(log n)
 * De-allocates the info of all the nodes in the tree using postorder traversal.
 * Should only be used by the owner of the info's memory.
 */
template<typename K, typename T>
void RankTree<K, T>::deAllocateAllInfoHelper(RankTree::Node* node) {
    if (node == nullptr) {
        return;
    }
    deAllocateAllInfoHelper(node->left);
    deAllocateAllInfoHelper(node->right);
    T* info = node->info;
    node->info = nullptr;
    delete info;
}


#endif //DS_WET1_RankTree_H
