#ifndef _BST_H_
#define _BST_H_

#include <string>
#include <stack>
#include <iostream>
#include <cassert>

template<typename K, typename I>
class BST {

public:
    using KeyType = K;
    using ItemType = I;

    ItemType* lookup(KeyType);

    void insert(KeyType, ItemType);
    void remove(KeyType);

    void displayEntries();
    void displayTree();

    BST() = default;
    BST(const BST &);
    BST & operator=(const BST &);

    BST(BST &&) noexcept ;
    BST & operator=(BST &&) noexcept ;

    ~BST();

private:
    struct Node;

    Node* root = leaf();
    static Node* leaf();
    static bool isLeaf(Node*);

    ItemType* lookupRec(KeyType soughtKey, Node* currentNode);
    void insertRec(KeyType, ItemType, Node* &);
    void removeRec(Node* &, KeyType);
    void deepDelete(Node*);
    Node* deepCopy(Node*);
    void detachMinimumNode(Node* &);

    void displayRec(Node*);
    void displayTreeRec(Node*, const short);

    void rotateRight(Node* &);
    void rotateLeft(Node* &);
};


template<typename K, typename I>
struct BST<K, I>::Node {
  KeyType key;
  ItemType item;
  Node* leftChild = nullptr;
  Node* rightChild = nullptr;

  Node(KeyType newKey, ItemType newItem) : key(newKey), item(newItem) {
  }
};

template<typename K, typename I>
typename BST<K,I>::Node* BST<K,I>::leaf() {
  return nullptr ;
}

template<typename K, typename I>
bool BST<K,I>::isLeaf(Node* n) {
    if (n == nullptr) {
        return false;
    }

    return n->leftChild == nullptr && n->rightChild == nullptr;
}

template<typename K, typename I>
typename BST<K,I>::ItemType* BST<K,I>::lookup(KeyType soughtKey) {
    return lookupRec(soughtKey, root);
}

template<typename K, typename I>
typename BST<K,I>::ItemType* BST<K,I>::lookupRec(KeyType soughtKey, Node* currentNode) {

    if (currentNode == nullptr) {
        return nullptr;
    }

    if (soughtKey == currentNode->key) {
        return &currentNode->item;
    } 
    else if (soughtKey < currentNode->key) {
        return lookupRec(soughtKey, currentNode->leftChild);
    }

    return lookupRec(soughtKey, currentNode->rightChild);
}

template<typename K, typename I>
void BST<K, I>::insertRec (KeyType key, ItemType item, Node* & current) {

    if (current == nullptr) {
        current = new Node(key, item);
    }

    if (key == current->key) {
        current->item = item;
    }

    if (key < current->key) {
        insertRec(key, item, current->leftChild);
    }
    else if (key > current->key) {
        insertRec(key, item, current->rightChild);
    }
}

template <typename K, typename I>
void BST<K,I>::insert(KeyType key, ItemType item) {
    insertRec(key, item, root);
}

template<typename K, typename I>
void BST<K,I>::remove(KeyType soughtKey) {
    this->removeRec(this->root, soughtKey);
}

template<typename K, typename I>
void BST<K,I>::removeRec(BST::Node *& current, KeyType soughtKey) {

    if (current == nullptr) {
        return;
    }

    if (soughtKey > current->key) {
        this->removeRec(current->rightChild, soughtKey);
    }
    else if (soughtKey < current->key) {
        this->removeRec(current->leftChild, soughtKey);
    }
    else {
        // no children
        if (this->isLeaf(current)) {
            delete current;
            current = nullptr;
        }
        // one child
        else if (current->leftChild == nullptr) {
            Node* temp = current;
            current = current->rightChild;
            delete temp;
        }
        else if (current->rightChild == nullptr) {
            Node* temp = current;
            current = current -> leftChild;
            delete temp;
        }
        // 2 children
        else {
            Node* temp = current->rightChild;
            detachMinimumNode(temp);
            current->key = temp->key;
            current->item = temp->item;
            this->removeRec(current->rightChild, temp->key);
        }
    }
}

template<typename K, typename I>
void BST<K, I>::detachMinimumNode(BST::Node *& node) {
    while (node->leftChild != nullptr) {
        node = node->leftChild;
    }
}

template<typename K, typename I>
void BST<K, I>::displayEntries() {
    this->displayRec(this->root);
}

template<typename K, typename I>
void BST<K, I>::displayTree() {
    displayTreeRec(this->root, 0);
}

template<typename K, typename I>
void BST<K,I>::displayTreeRec(BST::Node* current, const short indentationLevel) {

    if (current == nullptr) {
        return;
    }

    displayTreeRec(current->leftChild, indentationLevel + 1);

    if (indentationLevel > 0) {
        std::cout << "|";
    }

    for (short i = 0; i < indentationLevel; ++i) {
        std::cout << "--";
    }
    std::cout << current->key << " " << current->item << "\n";

    displayTreeRec(current->rightChild, indentationLevel + 1);
}

template <typename K, typename I>
void BST<K, I>::displayRec(Node *current) {

    if (current == nullptr) {
        return;
    }

    this->displayRec(current->leftChild);
    std::cout << current->key << " " << current->item << "\n";
    this->displayRec(current->rightChild);
}


template <typename K, typename I>
void BST<K,I>::deepDelete(Node* node) {
    
    if (node == nullptr) {
        return;
    }

    deepDelete(node->leftChild);
    deepDelete(node->rightChild);
    
    delete node;
}

template <typename K, typename I>
BST<K, I>::~BST() {
    deepDelete(root);
}

template <typename K, typename I>
BST<K, I> & BST<K, I>::operator=(const BST<K, I> & original) {

    this->root = deepCopy(original.root);

    return *this;
}

template <typename K, typename I>
BST<K, I>::BST(const BST<K,I> & originalTree) {
  this->root = deepCopy(originalTree.root);
}

template <typename K, typename I>
typename BST<K, I>::Node* BST<K, I>::deepCopy(Node* originalNode) {

    if (originalNode->leftChild != nullptr) {
        originalNode->leftChild = new BST<K,I>(originalNode->leftChild);
    }

    if (originalNode->rightChild != nullptr) {
        originalNode->rightChild = new BST<K,I>(originalNode->rightChild);
    }
}

template <typename K, typename I>
BST<K,I>::BST(BST<K, I> && originalTree) noexcept {
    this = deepCopy(originalTree);
}

template <typename K, typename I>
BST<K,I> & BST<K,I>::operator=(BST<K,I> && originalTree) noexcept {
    this = deepCopy(originalTree);
    return *this;
}

template <typename K, typename I>
void BST<K,I>::rotateRight(Node* & localRoot) {
    Node* b = localRoot;
    assert( !isLeaf(b) );
    Node* a = b->leftChild;
    assert( !isLeaf(b) );
    Node* beta = a->rightChild;

    b->leftChild = beta;
    localRoot = a;
    localRoot->rightChild = b;
}

template <typename K, typename I>
void BST<K,I>::rotateLeft (Node* & localRoot) {
    Node* a = localRoot;
    assert( !isLeaf(a) );
    Node* b = a->rightChild;
    assert( !isLeaf(b) );
    Node* beta = b->leftChild;

    a->rightChild = beta;
    localRoot = b;
    localRoot->rightChild = a;
}


#endif 