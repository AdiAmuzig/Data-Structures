//
// Created by adi.amuzig on 11/06/2020.
//

#ifndef CODE_DYNAMICCHAINHASHING_H
#define CODE_DYNAMICCHAINHASHING_H

#include <functional>
#include <cmath>
#include "Doubly_Linked_List.h"

//using h(x)=x mod(m)

template <class T, class Key, class Compare = std::less<Key>>
class DynamicChainHashing{
private:
    typedef DoublyLinkedList<T, Key, Compare>* Node;
    typedef typename DoublyLinkedList<T, Key, Compare>::Iterator iterator;

    Node* head;
    int sizeOfArray;
    int amountOfNodes;
    Compare compare;

    int hFunction (const Key &key);
    bool changeSizeArray(int newSize);

public:
    DynamicChainHashing(int size = 5);
    ~DynamicChainHashing();
    T& find(const Key& key);
    bool insert(const T& data, const Key& key);
    bool erase(const Key& key);

    class doesntExist: public std::exception {};
    class alreadyExists: public std::exception{};
};

template<class T, class Key, class Compare>
DynamicChainHashing<T, Key, Compare>::DynamicChainHashing(int size):
    head(nullptr),
    sizeOfArray(size),
    amountOfNodes(0),
    compare(compare){
        head = new Node[sizeOfArray];
        for (int i = 0; i < sizeOfArray; i++ ){
            head[i] = nullptr;
        }
}

template<class T, class Key, class Compare>
bool DynamicChainHashing<T, Key, Compare>::insert(const T &data, const Key &key) {
    int h = hFunction(key);
    if (head[h] == nullptr){
        head[h] = new DoublyLinkedList<T, Key, Compare>;
    }
    Node node = head[h];
    try {
        node->insertWithKey(key, data);
    } catch (std::exception& e) {
        return false;
    }
    amountOfNodes++;

    if (amountOfNodes == sizeOfArray){
        int newSize = 2*sizeOfArray;
        changeSizeArray(newSize);
    }
    return true;
}

template<class T, class Key, class Compare>
bool DynamicChainHashing<T, Key, Compare>::erase(const Key &key) {
    int h = hFunction(key);
    Node node = head[h];
    if (node == nullptr){
        throw doesntExist();
    }
    bool eraseResult = node->eraseWithKey(key);
    if (eraseResult && node->returnNumOfNodes() == 0){
        delete head[h];
        head[h] = nullptr;
    }
    amountOfNodes--;

    if (amountOfNodes < sizeOfArray/4){
        int newSize = sizeOfArray*0.5;
        changeSizeArray(newSize);
    }

    return eraseResult;
}

template<class T, class Key, class Compare>
T &DynamicChainHashing<T, Key, Compare>::find(const Key &key) {
    int h = hFunction(key);
    Node node = head[h];
    if (node == nullptr){
        throw doesntExist();
    }
    return *(node->findWithKey(key));
}

template<class T, class Key, class Compare>
DynamicChainHashing<T, Key, Compare>::~DynamicChainHashing() {
    for (int i = 0; i < sizeOfArray; i++ ){
        delete head[i];
    }
    delete[] head;
}

template<class T, class Key, class Compare>
int DynamicChainHashing<T, Key, Compare>::hFunction(const Key &key) {
    double a = (std::sqrt(5.0) - 1.0)/2.0;
    double m = (double)sizeOfArray;
    double h = std::floor(m * (a * key - std::floor(a * key)));
    return (int)h;
}

template<class T, class Key, class Compare>
bool DynamicChainHashing<T, Key, Compare>::changeSizeArray(int newSize) {
    int prevSizeOfArray = sizeOfArray;
    sizeOfArray = newSize;
    iterator i;
    Key key;
    T data;
    int h;
    Node* newHead = new Node[sizeOfArray];
    for (int p = 0; p < sizeOfArray; p++ ){
        newHead[p] = nullptr;
    }

    for (int j = 0 ; j < prevSizeOfArray ; j++){
        if (head[j] != nullptr){
            Node oldNode = head[j];
            i = oldNode->returnHead();

            while (oldNode->returnNumOfNodes() > 0){
                key = i.getKey();
                data = *i;
                h = hFunction(key);
                if (newHead[h] == nullptr){
                    newHead[h] = new DoublyLinkedList<T, Key, Compare>;
                }
                Node newNode = newHead[h];
                newNode->insertWithKey(key, data);
                i = oldNode->erase(i);
            }
        }
    }

    for (int k = 0; k < prevSizeOfArray; k++ ){
        delete head[k];
    }
    delete[] head;
    head = newHead;
    return true;
}

#endif //CODE_DYNAMICCHAINHASHING_H
