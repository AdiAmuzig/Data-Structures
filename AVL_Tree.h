//
// Created by adi.amuzig on 04/05/2020.
//

#ifndef HW1_WET_SOL_AVL_TREE_H
#define HW1_WET_SOL_AVL_TREE_H

#include <cmath>
#include <functional>
#include <exception>
#include <cassert>
#include <iostream>

#define basic_height 0

template <class T, class Key>
class AVLTree{
private:
    struct Node{
        T data;
        Key key;
        int height;
        int sub_tree_size;
        Node* right_son;
        Node* left_son;
        Node* father;

        Node* next;
        Node* prev;

        Node(T data,Key key);
        ~Node();//notice that deleating gose recursively meaning deleating all sons if not nulptr
        bool operator==(const Node &Node2) const;
        int nodeHeight();
        int balanceFactor();
        int calcSubTreeSize();
    };
    Node* head;
    Node* list_head;
    Node* maxNode;
    int num_of_nodes;

    //returns new head
    Node* balanceTree(Node* node);
    // returns new head of sub tree after a roll
    Node* LL_roll(Node* node);
    Node* RR_roll(Node* node);
    Node* LR_roll(Node* node);
    Node* RL_roll(Node* node);

    Node* newCompleteTree (T data, Key key, int height);
    void reverseOrderRemoveExtraLeafs(Node* node,int* counter);
    void inOrderFillTree(Node* node,const T* data, const Key *key, int* counter, Node** prev_node = nullptr);//assuming sorted data
    void changeSonOfFatherNode(Node* father, Node* son, Node* new_son);
    Node* deleteNodeForErase(Node* node);
    void recursivePrintInOrder(Node* node);
    void inOrderExtractKeyAndData(Node* node, Key* Keys_array, T* data_array, int* counter, int *tot_num = nullptr);

//    Adding functions for list usage
    void insertListUpdate(Node* node);
    void eraseListUpdate(Node* node);

public:
    struct iterator{
        Node* current;

        explicit iterator(Node* node = nullptr);
        ~iterator();
        bool isNull();
        iterator& operator++();
        T& operator*();
    };

    explicit AVLTree(const T* data = nullptr, const Key *key= nullptr, int size = 0);//constractor
    AVLTree (const AVLTree& tree);
    ~AVLTree();//distractor
    T& find(const Key& key);//find element using key
    bool insert(const T& data, const Key& key);//insert an element given the data and the key
    bool erase(const Key& key);//delete an entrance given a key
    void printInOrder();
    AVLTree& operator = (const AVLTree& tree);
    void clearTree();
    void ExtractKeys(Key* Keys_array);
    void ExtractSomeKeysAndData(Key* Keys_array, T* data_array, int *tot_num = nullptr);
    int getNumOfNodes();
    int listExtractKeysAndData(Key* Keys_array, T* data_array, int *tot_num = nullptr, int counter = 0);
    iterator returnListHead();
    iterator valueByRank(int rank);
    T& returnMaxNodeData();


//    Exceptions
    class noMatch: public std::exception {};
    class alreadyExists: public std::exception {
    public:
        Key key;
        T data;
        alreadyExists(Key key, T data):key(key),data(data){}
    };
    class doesntExist: public std::exception {
    public:
        Key key;
        doesntExist(Key key):key(key){}
    };
};

template<class T, class Key>
AVLTree<T, Key>::iterator::iterator(AVLTree::Node *node) :
    current(node){
}

template<class T, class Key>
AVLTree<T, Key>::iterator::~iterator() {
    current = nullptr;
}

template<class T, class Key>
typename AVLTree<T, Key>::iterator &AVLTree<T, Key>::iterator::operator++() {
    if (current != nullptr){
        current=current->next;
    }
    return *this;
}

template<class T, class Key>
T &AVLTree<T, Key>::iterator::operator*() {
    return current->data;
}

template<class T, class Key>
bool AVLTree<T, Key>::iterator::isNull() {
    return (current == nullptr);
}


template<class T, class Key>
AVLTree<T, Key>::~AVLTree() {
    delete head;
}

template<class T, class Key>
AVLTree<T, Key>::AVLTree(const T *data, const Key *key, int size) {
    num_of_nodes = size;
    if (size == 0){
        head = nullptr;
        list_head = nullptr;
        maxNode = nullptr;
    } else if (size >= 1){
        double head_height = 0;
        double max_nodes = 1;
        double base = 2;
        while (max_nodes < size){
            ++head_height;
            max_nodes = max_nodes + pow(base,head_height);
        }
        int counter = int(max_nodes) - size;
        head = newCompleteTree(*data, *key, head_height);
        reverseOrderRemoveExtraLeafs(head, &counter);
        int size_counter = 0;
        Node* list_node = nullptr;
        inOrderFillTree(head, data, key, &size_counter, &list_node);
        iterator i = valueByRank(1);
        maxNode = i.current;
    }
}

template<class T, class Key>
void AVLTree<T, Key>::inOrderFillTree(Node* node,const T *data, const Key *key, int* counter,Node** prev_node) {
    if (node == nullptr){
        return;
    }
    inOrderFillTree(node->left_son, data, key, counter, prev_node);
    node -> data = *(data + (*counter));
    node -> key = *(key + (*counter));
    node -> height = node->left_son == nullptr? 0: node->left_son->height + 1;

    if ((*counter) == 0){
        list_head = node;
    } else{
        (*prev_node)->next = node;
        node ->prev = (*prev_node);
    }
    (*prev_node) = node;

    ++(*counter);
    inOrderFillTree(node->right_son, data, key, counter, prev_node);
}

template<class T, class Key>
void AVLTree<T, Key>::reverseOrderRemoveExtraLeafs(AVLTree::Node *node, int* counter) {
    if (*counter == 0){
        return;
    }else if (node->left_son == nullptr && node->right_son == nullptr){
        if (node->father->right_son == node){
            node->father->right_son = nullptr;
        } else{
            node->father->left_son = nullptr;
        }
        delete node;
        --(*counter);
        return;
    }
    reverseOrderRemoveExtraLeafs(node -> right_son, counter);
    reverseOrderRemoveExtraLeafs(node -> left_son, counter);
}

template<class T, class Key>
typename AVLTree<T,Key>::Node *AVLTree<T, Key>::newCompleteTree(T data, Key key, int height) {
    if (height == 0){
        return new Node(data,key);
    } else{
        Node* rightNode = newCompleteTree(data, key, height-1);
        Node* leftNode = newCompleteTree(data, key, height-1);
        Node* current = new Node(data,key);

        current -> right_son = rightNode;
        current -> left_son = leftNode;
        current -> height = height;
        rightNode -> father = current;
        leftNode -> father = current;
        return current;
    }
}

template<class T, class Key>
T &AVLTree<T, Key>::find(const Key &key) {
    Node* current_node = head;
    while (current_node!= nullptr){
        if (current_node->key < key){
            current_node = current_node->right_son;
        } else if (key < current_node->key){
            current_node = current_node->left_son;
        } else{
            return current_node->data;
        }
    }
    throw noMatch();
}

template<class T, class Key>
typename AVLTree<T, Key>::Node *AVLTree<T, Key>::LL_roll(AVLTree::Node *node) {
    Node* node_B = node;
    Node* node_A = node_B->left_son;
    Node* node_A_right = node_A->right_son;

    if (node_A_right != nullptr) {
        node_A_right->father = node_B;
    }

    node_A->father = node_B->father;
    node_A->right_son = node_B;
    node_B->father = node_A;
    node_B->left_son = node_A_right;

    node_B->height=node_B->nodeHeight();
    node_A->height=node_A->nodeHeight();

    node_B->sub_tree_size=node_B->calcSubTreeSize();
    node_A->sub_tree_size=node_A->calcSubTreeSize();

    if (node_A->father == nullptr){
        head = node_A;
    }else{
        changeSonOfFatherNode(node_A->father, node_B, node_A);
    }
    return node_A;
}

template<class T, class Key>
typename AVLTree<T, Key>::Node *AVLTree<T, Key>::RR_roll(AVLTree::Node *node) {
    Node* node_B = node;
    Node* node_A = node_B->right_son;
    Node* node_A_left = node_A->left_son;

    if (node_A_left != nullptr){
        node_A_left->father = node_B;
    }

    node_A->father=node_B->father;
    node_A->left_son=node_B;
    node_B->father=node_A;
    node_B->right_son=node_A_left;

    node_B->height=node_B->nodeHeight();
    node_A->height=node_A->nodeHeight();

    node_B->sub_tree_size=node_B->calcSubTreeSize();
    node_A->sub_tree_size=node_A->calcSubTreeSize();

    if (node_A->father != nullptr){
        if (node_A->father->right_son == node_B){
            node_A->father->right_son = node_A;
        }else{
            node_A->father->left_son = node_A;
        }
    } else{
        head = node_A;
    }

    return node_A;
}

template<class T, class Key>
typename AVLTree<T, Key>::Node *AVLTree<T, Key>::LR_roll(AVLTree::Node *node) {
    RR_roll(node->left_son);
    return LL_roll(node);
}

template<class T, class Key>
typename AVLTree<T, Key>::Node *AVLTree<T, Key>::RL_roll(AVLTree::Node *node) {
    LL_roll(node->right_son);
    return RR_roll(node);
}

template<class T, class Key>
bool AVLTree<T, Key>::insert(const T &data, const Key &key) {
    Node* added_node = new Node(data,key);
    if (head == nullptr){
        head = added_node;
        list_head = added_node;
        num_of_nodes = 1;
        maxNode = added_node;
        return true;
    }
    Node* current_node = head;
    Node* father_node = current_node->father;

    while (current_node!= nullptr){
        if (current_node->key < key){
            father_node = current_node;
            current_node = current_node->right_son;
        } else if (key < current_node->key){
            father_node = current_node;
            current_node = current_node->left_son;
        } else{
            delete added_node;
            throw alreadyExists(current_node->key,current_node->data);
        }
    }

    if (father_node->key < key){
        father_node->right_son = added_node;
    } else{
        father_node->left_son = added_node;
    }

    added_node->father = father_node;
    father_node->height = father_node->nodeHeight();
    father_node->sub_tree_size=father_node->calcSubTreeSize();

    insertListUpdate(added_node);
    balanceTree(father_node);
    num_of_nodes++;

    iterator i = valueByRank(1);
    maxNode = i.current;

    return true;
}

template<class T, class Key>
bool AVLTree<T, Key>::erase(const Key &key) {
    Node* current_node = head;
    //Find the node
    while (current_node != nullptr) {
        if (current_node->key < key){
            current_node = current_node->right_son;
        } else if (key < current_node->key){
            current_node = current_node->left_son;
        } else{
            break;
        }
    }
    if (current_node == nullptr){
        throw doesntExist(key);
    }

    eraseListUpdate(current_node);
    Node* balance_node = deleteNodeForErase(current_node);

    current_node->right_son = nullptr;
    current_node->left_son = nullptr;
    delete current_node;
    num_of_nodes--;
    balanceTree(balance_node);

    if (num_of_nodes==0){
        maxNode = nullptr;
    } else{
        iterator i = valueByRank(1);
        maxNode = i.current;
    }
    return true;

}

template<class T, class Key>
typename AVLTree<T, Key>::Node *AVLTree<T, Key>::balanceTree(AVLTree::Node *node) {
    while (node != nullptr){
        node->height = node->nodeHeight();
        node->sub_tree_size=node->calcSubTreeSize();
        int temp_height = node->height;

        if (node->balanceFactor() == 2){
            if (node->left_son->balanceFactor() == -1){
                node = LR_roll(node);
            }else {
                node = LL_roll(node);
            }
        } else if (node->balanceFactor() == -2){
            if(node->right_son->balanceFactor() == 1){
                node = RL_roll(node);
            } else{
                node = RR_roll(node);
            }
        }

        if (node->height == temp_height){
            node = node->father;
        }
    }
    return head;
}

template<class T, class Key>
void AVLTree<T, Key>::changeSonOfFatherNode(AVLTree::Node *father, AVLTree::Node *son, AVLTree::Node *new_son) {
    if (father == nullptr) {
        head = new_son;
    } else{
        if (father->right_son == son){
            father->right_son = new_son;
        } else{
            father->left_son = new_son;
        }
    }
}

template<class T, class Key>
typename AVLTree<T, Key>::Node *AVLTree<T, Key>::deleteNodeForErase(AVLTree::Node *node) {
    Node* father = node ->father;
    //0 sons
    if (node->left_son == nullptr && node->right_son == nullptr){
        changeSonOfFatherNode(father, node, nullptr);
        return father;
    }

    //1 son
    else if (node->right_son == nullptr && node->left_son != nullptr){
        changeSonOfFatherNode(father, node, node -> left_son);
        node -> left_son -> father = father;
        return father;
    } else if (node->right_son != nullptr && node->left_son == nullptr){
        changeSonOfFatherNode(father, node, node -> right_son);
        node -> right_son-> father  = father;
        return father;
    }

    // 2 sons
    else{
        Node* moving_node = node->right_son;
        if (moving_node->left_son == nullptr){
            changeSonOfFatherNode(father, node, moving_node);
            moving_node->father=father;
            node->left_son->father = moving_node;
            moving_node->left_son = node->left_son;
            return moving_node;
        } else{
            while (moving_node->left_son != nullptr){
                moving_node = moving_node->left_son;
            }
            Node* moving_node_father = moving_node->father;
            if (moving_node->right_son != nullptr){
                moving_node->right_son->father = moving_node_father;
            }
            moving_node_father->left_son= moving_node->right_son;
            changeSonOfFatherNode(father, node, moving_node);
            moving_node->father=father;
            moving_node->right_son=node->right_son;
            moving_node->left_son=node->left_son;
            node->right_son->father = moving_node;
            node->left_son->father = moving_node;
            return moving_node_father;
        }
    };
}

template<class T, class Key>
void AVLTree<T, Key>::recursivePrintInOrder(Node* node) {
    if (node == nullptr){
        return;
    }
    recursivePrintInOrder(node->left_son);
    std::cout << node->data << std::endl;
    recursivePrintInOrder(node->right_son);
}

template<class T, class Key>
void AVLTree<T, Key>::printInOrder() {
    recursivePrintInOrder(head);
}

template<class T, class Key>
void AVLTree<T, Key>::clearTree() {
    delete head;
    num_of_nodes = 0;
}

template<class T, class Key>
AVLTree<T, Key> &AVLTree<T, Key>::operator = (const AVLTree<T, Key> &tree) {
    clearTree();
    num_of_nodes = tree.num_of_nodes;
    if (num_of_nodes == 0){
        head = nullptr;
        maxNode = nullptr;
    } else if (num_of_nodes >= 1){
        double head_height = 0;
        double max_nodes = 1;
        double base = 2;
        while (max_nodes < num_of_nodes){
            ++head_height;
            max_nodes = max_nodes + pow(base,head_height);
        }
        int counter = int(max_nodes) - num_of_nodes;
        head = newCompleteTree(tree.head->data, tree.head->key, head_height);
        reverseOrderRemoveExtraLeafs(head, &counter);
        int size_counter = 0;
        Key keys_array[num_of_nodes];
        T data_array[num_of_nodes];
        inOrderExtractKeyAndData(tree.head, keys_array, data_array, &size_counter);
        size_counter = 0;
        Node* list_node = nullptr;
        inOrderFillTree(head, data_array, keys_array, &size_counter, &list_node);
        iterator it = valueByRank(1);
        maxNode = it.current;
    }

    return *this;
}

template<class T, class Key>
void AVLTree<T, Key>::inOrderExtractKeyAndData(Node* node, Key* Keys_array, T* data_array, int* counter, int* tot_num){
    if (node == nullptr){
        return;
    }
    inOrderExtractKeyAndData(node->left_son, Keys_array, data_array, counter, tot_num);
    if (tot_num != nullptr && (*tot_num)==0){
        return;
    }

    *(Keys_array + *(counter)) = node ->key;
    *(data_array + *(counter)) = node ->data;
    ++(*counter);
    if (tot_num != nullptr){
        --(*tot_num);
    }

    inOrderExtractKeyAndData(node->right_son, Keys_array, data_array, counter, tot_num);
}

template<class T, class Key>
AVLTree<T, Key>::AVLTree(const AVLTree &tree) {
    num_of_nodes = tree.num_of_nodes;
    maxNode = tree.maxNode;
    if (num_of_nodes == 0){
        head = nullptr;
    } else if (num_of_nodes >= 1){
        double head_height = 0;
        double max_nodes = 1;
        double base = 2;
        while (max_nodes < num_of_nodes){
            ++head_height;
            max_nodes = max_nodes + pow(base,head_height);
        }
        int counter = int(max_nodes) - num_of_nodes;
        head = newCompleteTree(tree.head->data, tree.head->key, head_height);
        reverseOrderRemoveExtraLeafs(head, &counter);
        int size_counter = 0;
        Key keys_array[num_of_nodes];
        T data_array[num_of_nodes];
        inOrderExtractKeyAndData(tree.head, keys_array, data_array, &size_counter);
        size_counter = 0;
        Node* list_node = nullptr;
        inOrderFillTree(head, data_array, keys_array, &size_counter, &list_node);
    }

}

template<class T, class Key>
void AVLTree<T, Key>::ExtractKeys(Key* keys_array) {
    int size_counter = 0;
    T data_array[num_of_nodes];
    inOrderExtractKeyAndData(head,keys_array,data_array,&size_counter);
}

template<class T, class Key>
int AVLTree<T, Key>::getNumOfNodes() {
    return num_of_nodes;
}

template<class T, class Key>
void AVLTree<T, Key>::ExtractSomeKeysAndData(Key *Keys_array, T *data_array, int* tot_num) {
    int size_counter = 0;
    inOrderExtractKeyAndData(head,Keys_array,data_array,&size_counter, tot_num);
}

template<class T, class Key>
void AVLTree<T, Key>::insertListUpdate(AVLTree::Node *node) {
    Node* father = node->father;
    Node* next;
    Node* prev;

    if (father == nullptr){
        list_head = node;
        node->next = nullptr;
        node->prev = nullptr;

    }else {
        if (node == father->right_son) {
            next = father->next;
            prev = father;
        } else if (node == father->left_son) {
            next = father;
            prev = father->prev;
        }

        if (prev != nullptr){
            prev->next = node;
        } else{
            list_head = node;
        }
        if (next != nullptr){
            next->prev = node;
        }
        node->next = next;
        node->prev = prev;
    }
}

template<class T, class Key>
void AVLTree<T, Key>::eraseListUpdate(AVLTree::Node *node) {
    Node* next_node = node->next;
    Node* prev_node = node->prev;
    if (next_node != nullptr){
        next_node->prev = prev_node;
    }
    if (prev_node != nullptr){
        prev_node->next = next_node;
    } else{
        list_head = next_node;
    }
}

template<class T, class Key>
int AVLTree<T, Key>::listExtractKeysAndData(Key *Keys_array, T *data_array, int *tot_num,int counter) {
    Node* node = list_head;
    while (node != nullptr){

        *(Keys_array + counter) = node ->key;
        *(data_array + counter) = node ->data;
        ++counter;
        node = node->next;
        if (tot_num != nullptr){
            --(*tot_num);
        }

        if (tot_num != nullptr && (*tot_num) == 0){
            return counter;
        }
    }
    return counter;
}

template<class T, class Key>
typename AVLTree<T, Key>::iterator AVLTree<T, Key>::returnListHead() {
    iterator i(list_head);
    return i;
}

template<class T, class Key>
typename AVLTree<T, Key>::iterator AVLTree<T, Key>::valueByRank(int rank) {
    if (rank > num_of_nodes || rank <= 0){
        throw noMatch();
    }
    Node* node = head;
    Node* tempNode;
    int currentRank;
    int x = 0;
    if (node->right_son != nullptr){
        tempNode = node->right_son;
        currentRank = tempNode->sub_tree_size + 1;
    } else{
        currentRank = 1;
    }

    while (currentRank != rank){

        if (currentRank > rank){
            node = node->right_son;
        }else if (currentRank < rank){
            x = currentRank;
            node = node->left_son;
        }

        if (node->right_son != nullptr){
            tempNode = node->right_son;
            currentRank = tempNode->sub_tree_size + 1 + x ;
        } else{
            currentRank = 1 + x;
        }
    }
    iterator i(node);
    return i;
}

template<class T, class Key>
T &AVLTree<T, Key>::returnMaxNodeData() {
    return maxNode->data;
}

template<class T, class Key>
AVLTree<T, Key>::Node::Node(T data, Key key) :
    data(data),
    key(key),
    height(basic_height),
    sub_tree_size(1),
    right_son(nullptr),
    left_son(nullptr),
    father(nullptr),
    next(nullptr),
    prev(nullptr){
}

template<class T, class Key>
bool AVLTree<T, Key>::Node::operator==(const AVLTree::Node &Node2) const {
    return key == Node2.key;
}

template<class T, class Key>
AVLTree<T, Key>::Node::~Node() {
    delete right_son;
    delete left_son;
}

template<class T, class Key>
int AVLTree<T, Key>::Node::nodeHeight() {
    int right_height;
    int left_height;
    Node* right = right_son;
    Node* left = left_son;

    if (left == nullptr && right == nullptr){
        return 0;
    }

    if (right == nullptr){
        right_height = 0;
    } else {
        right_height = right->height;
    }
    if (left == nullptr){
        left_height = 0;
    } else{
        left_height = left->height;
    }

    if (left_height > right_height){
        return left_height+1;
    } else{
        return right_height+1;
    }
}

template<class T, class Key>
int AVLTree<T, Key>::Node::balanceFactor() {
    int left_son_height = left_son == nullptr?-1 : left_son->height;
    int right_son_height = right_son == nullptr?-1 : right_son->height;
    return left_son_height - right_son_height;
}

template<class T, class Key>
int AVLTree<T, Key>::Node::calcSubTreeSize() {
    int left_size;
    int right_size;
    Node* right = right_son;
    Node* left = left_son;

    if (right == nullptr){
        right_size = 0;
    } else {
        right_size = right->sub_tree_size;
    }
    if (left == nullptr){
        left_size = 0;
    } else{
        left_size = left->sub_tree_size;
    }

    sub_tree_size = 1 + right_size + left_size;
    return sub_tree_size;
}


#endif //HW1_WET_SOL_AVL_TREE_H
