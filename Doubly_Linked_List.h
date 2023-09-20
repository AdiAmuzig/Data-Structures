//
// Created by adi.amuzig on 03/05/2020.
// Notes:
// 1. Using regular pointers, meaning if 1 iterator deletes a node then it will still appear in another iterator as garbage
// 2. The erase function returns the closest iteratoe it can give.
//      a. it can be nothing if there are not any left.
//      b. previous position if the latest node was deleated
//      c. the next position otherwise

#ifndef HW1_WET_SOL_DOUBLY_LINKED_LIST_H
#define HW1_WET_SOL_DOUBLY_LINKED_LIST_H

#include <functional>

template <class T, class Key, class Compare = std::less<Key>>
class DoublyLinkedList {

private:
    struct Node{
        Key key;
        T data;
        Node* next;
        Node* prev;
    };
    Node* head;
    Node* end;
    int numOfNodes;
    Compare compare;

public:
    struct Iterator{
        Node* current;

        explicit Iterator(Node* node = nullptr);
        ~Iterator();
        bool operator == (const Iterator &I2) const;
        Iterator& operator = (const Iterator& b);
        Iterator& operator++();
        Iterator& operator--();
        T& operator*();
        Key& getKey();
    };
    DoublyLinkedList();
    ~DoublyLinkedList();
    Iterator insert(Iterator pos, const T& value, const Key& key);
    Iterator erase(Iterator pos);
    Iterator returnHead ();
    Iterator returnEnd ();

    int returnNumOfNodes();
    Iterator findWithKey(Key key);
    Iterator insertWithKey(const Key& key, const T& value);
    bool eraseWithKey(Key key);

    //    Exceptions
    class doesntExist: public std::exception {};
    class alreadyExists: public std::exception{};
};

template <class T, class Key, class Compare>
DoublyLinkedList<T, Key, Compare>::DoublyLinkedList() :
    head(nullptr),
    end(nullptr),
    numOfNodes(0){
}

template <class T, class Key, class Compare>
DoublyLinkedList<T, Key, Compare>::~DoublyLinkedList() {
    Iterator i(head);
    while (head != nullptr) {
        i = erase(i);
    }
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator DoublyLinkedList<T, Key, Compare>::insert(Iterator pos, const T &value, const Key& key) {
    Node* new_node = new Node();
    new_node -> data = value;
    new_node -> key = key;
    Node *current_node = pos.current;

    if (head == nullptr){
        head = new_node;
        end = new_node;
        Iterator new_pos(head);
        numOfNodes++;
        return new_pos;

    } else if (current_node == end) {
        new_node -> prev = current_node;
        current_node -> next = new_node;
        end = new_node;

    } else{
        Node* next_node = current_node->next;
        next_node -> prev = new_node;
        new_node -> next = next_node;
        current_node -> next = new_node;
        new_node -> prev = current_node;
    }
    numOfNodes++;
    return ++pos;
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator DoublyLinkedList<T, Key, Compare>::erase(DoublyLinkedList::Iterator pos) {
    Node* current_node = pos.current;

    if (current_node == head && current_node == end){
        pos.current = nullptr;
        head = nullptr;
        end = nullptr;
    }else if (current_node == head) {
        ++pos;
        head = current_node->next;
        head->prev= nullptr;
    }else if (current_node == end){
        --pos;
        end = current_node->prev;
        end->next= nullptr;
    }else{
        ++pos;
        Node* next_node = current_node->next;
        Node* prev_node = current_node->prev;
        prev_node->next=next_node;
        next_node->prev=prev_node;
    }
    delete current_node;
    numOfNodes--;
    return pos;
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator DoublyLinkedList<T, Key, Compare>::returnHead() {
    Iterator i(head);
    return i;
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator DoublyLinkedList<T, Key, Compare>::returnEnd() {
    Iterator i(end) ;
    return i;
}

template<class T, class Key, class Compare>
int DoublyLinkedList<T, Key, Compare>::returnNumOfNodes() {
    return numOfNodes;
}

template<class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator DoublyLinkedList<T, Key, Compare>::findWithKey(Key key) {
    Iterator i = returnHead();
    while (i.current != nullptr){
        if (compare(i.getKey(),key) || compare(key,i.getKey())){
            if (i.current -> next == nullptr){
                break;
            }
            ++i;
        } else{
            return i;
        }
    }
    throw doesntExist();
}

template<class T, class Key, class Compare>
bool DoublyLinkedList<T, Key, Compare>::eraseWithKey(Key key) {
    Iterator i;
    try {
        i = findWithKey(key);
    } catch (std::exception& e) {
        return false;//returns false if value does not exist
    }
    erase(i);
    return true;
}

template<class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator DoublyLinkedList<T, Key, Compare>::insertWithKey(const Key& key, const T& value) {
    Iterator i;
    try {
        i = findWithKey(key);
    } catch (std::exception& e) {
        return i = insert(returnHead(),value,key);
    }
    throw alreadyExists();
}

template <class T, class Key, class Compare>
bool DoublyLinkedList<T, Key, Compare>::Iterator::operator==(const DoublyLinkedList::Iterator &I2) const {
    return current == I2.current;
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator &DoublyLinkedList<T, Key, Compare>::Iterator::operator++() {
    if (current->next != nullptr){
        current=current->next;
    }
    return *this;
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator &DoublyLinkedList<T, Key, Compare>::Iterator::operator--() {
    if (current->prev != nullptr){
        current=current->prev;
    }
    return *this;
}

template <class T, class Key, class Compare>
T &DoublyLinkedList<T, Key, Compare>::Iterator::operator*() {
    return current->data;
}

template <class T, class Key, class Compare>
DoublyLinkedList<T, Key, Compare>::Iterator::Iterator(DoublyLinkedList::Node* node) {
    this->current=node;
}

template <class T, class Key, class Compare>
typename DoublyLinkedList<T, Key, Compare>::Iterator &DoublyLinkedList<T, Key, Compare>::Iterator::operator=(const DoublyLinkedList::Iterator &b) {
    this->current=b.current;
    return *this;
}

template <class T, class Key, class Compare>
DoublyLinkedList<T, Key, Compare>::Iterator::~Iterator() {
    current = nullptr;
}

template<class T, class Key, class Compare>
Key &DoublyLinkedList<T, Key, Compare>::Iterator::getKey() {
    return current->key;
}

#endif //HW1_WET_SOL_DOUBLY_LINKED_LIST_H
