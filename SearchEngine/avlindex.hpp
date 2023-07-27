#ifndef AVLINDEX_HPP
#define AVLINDEX_HPP

#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

/**
 * AVLIndex Class
 * Authored by Alexandra Ingrando
 * This class can store templated types
 * in a self-balancing tree;
 */

template <class T>
class AVLIndex{
private:
    struct Node{
        T data;
        Node* left;
        Node* right;
        int height, balanceFactor;
        Node(): left(nullptr), right(nullptr), height(0), balanceFactor(0) {}
        Node(T d):left(nullptr), right(nullptr), data(d), height(0), balanceFactor(0) {}
    };
    Node* head;
    Node* lastSearch;
    int numNodes;
    void recursiveAdd(T&, Node*);
    T& recursiveSearch(T&, Node*);
    bool recursiveInTree(T&, Node*);
    void deleteRecursion(Node*);
    void leftRotationRightChild(Node*&);
    void leftRotationLeftChild(Node*&);
    void leftHeadRotation();
    void leftRightRotation(Node*&);
    void rightRotationRightChild(Node*&);
    void rightRotationLeftChild(Node*&);
    void rightHeadRotation();
    void rightLeftRotation(Node*&);
    void calculateHeight(Node*&);
    void calculateFactor(Node*&);
    void displayRecursion(Node*, string);
    void writeInOrderRecursion(Node*, ostream&);
    void preorderRecursion(Node*&, vector<T>&);

public:

    //rule of three
    AVLIndex();
    AVLIndex(const AVLIndex&);
    ~AVLIndex();
    void initialize();
    void add(T&);
    T& search(T&);
    T* find(T&);
    bool inTree(T&);
    void deleteIndex();
    int size();
    vector<T> preOrder();


};

#endif // AVLINDEX_HPP


template <class T>
AVLIndex<T>::AVLIndex(){
    this->head = nullptr;
    this->lastSearch = nullptr;
    this->numNodes = 0;
}

template <class T>
AVLIndex<T>::AVLIndex(const AVLIndex& newData){
    this->head = new Node(newData.head->data);
}


/**
 * void initialize() sets Node pointers to null and counter to zero
 */
template <class T>
void AVLIndex<T>::initialize(){
    this->head = nullptr;
    this->lastSearch = nullptr;
    this->numNodes = 0;
}

/**
 * add(T&) accepts a templated object as a parameter. The function will sort and add the passed object into the tree, balancing the tree as it does
 */
template <class T>
void AVLIndex<T>::add(T& data){
    if(this->head == nullptr){
        this->head = new Node(data);
        numNodes++;
    } else {
        recursiveAdd(data, head);
        if (head->balanceFactor >= 2){
            rightHeadRotation();
        } else if (head->balanceFactor <= -2){
            leftHeadRotation();
        }
    }
}

template <class T>
void AVLIndex<T>::recursiveAdd(T& data, Node* root){
    if (root->data > data){
        if(root->left == nullptr){
            numNodes++;
            root->left = new Node(data);
        } else {
            recursiveAdd(data, root->left);
        }
    } else if (root->data < data){
        if(root->right == nullptr){
            numNodes++;
            root->right = new Node(data);
        } else {
            recursiveAdd(data, root->right);
        }
    }

    calculateHeight(root);
    if(root->right != nullptr){
        if (root->right->balanceFactor >= 2){
            rightRotationRightChild(root);
        } else if (root->right->balanceFactor <= -2){
            leftRotationRightChild(root);
        }
    }

    if(root->left != nullptr){
        if (root->left->balanceFactor >= 2){
            rightRotationLeftChild(root);
        } else if (root->left->balanceFactor <= -2){
            leftRotationLeftChild(root);
        }
    }


}


/**
 * size() will return the current number of objects stored in the AVL tree;
 */
template <class T>
int AVLIndex<T>::size(){
    return numNodes;
}

template <class T>
void AVLIndex<T>::calculateHeight(Node*& root){
    if (root == nullptr) {
        //cout << "what the fuck" << endl;
    }
    if(root->left != nullptr){
        if(root->right != nullptr){
            //both exist, make bigger hider
            if (root->right->height > root->left->height)
                root->height = root->right->height + 1;
            else
                root->height = root->left->height + 1;
        } else {
            root->height = root->left->height + 1;
        }
    } else if (root->right != nullptr){
        //means left is null ptr
        root->height = root->right->height + 1;
    } else {
        root->height = 0;
    }

    calculateFactor(root);
}

template <class T>
void AVLIndex<T>::calculateFactor(Node*& root){
    if(root->left == nullptr){
        if(root->right == nullptr){
            root->balanceFactor = 0;
        } else{
            root->balanceFactor = -1 * (root->right->height + 1);
        }
    } else if (root->right == nullptr){
        //already checked left, so won't be null
        root->balanceFactor = (root->left->height + 1);
    } else{
        root->balanceFactor = (root->left->height + 1) - (root->right->height + 1);
    }

}


/**
 * search(T&) accepts an object to match to a stored node. A reference to that stored node will be returned. Recommended to first use inTree(T&).
 */
template <class T>
T& AVLIndex<T>::search(T& data){
    if(this->head != nullptr){
        if(lastSearch != nullptr && lastSearch->data == data)
            return lastSearch->data;
        else
            return recursiveSearch(data, head);
    }
}

template <class T>
T& AVLIndex<T>::recursiveSearch(T& data, Node* root){
    if(data == root->data)
        return root->data;
    else{
        if(data > root->data){
            if(root->right != nullptr){
                return recursiveSearch(data, root->right);
            }
        } else {
            if(root->left != nullptr){
                return recursiveSearch(data, root->left);
            }
        }
    }
}

/**
 * fint(T&) will return a pointer to the object that matches the passed variable. Reccomended to first use inTree(T&)
 */
template <class T>
T* AVLIndex<T>::find(T& newData){
    if(!(lastSearch->data == newData)){
        //will have lastSearch point to newData
        search(newData);
    }
    T* ptr = &lastSearch->data;
    return ptr;
}

/**
 * inTree(T&) accepts an object, and returns a bool of whether or not the object is stored in the tree.
 */
template <class T>
bool AVLIndex<T>::inTree(T& data){
    bool final = false;
    if(this->head != nullptr)
        final = recursiveInTree(data, head);
    return final;
}

template <class T>
bool AVLIndex<T>::recursiveInTree(T& data, Node* current){
    bool final = false;
    bool currentNode = false;
    bool leftChild = false;
    bool rightChild = false;
    if(current->data == data){
        currentNode = true;
        lastSearch = current;
    }
    if(currentNode == false && data < current->data  && current->left != nullptr)
        leftChild = recursiveInTree(data, current->left);
    if(currentNode == false && data > current->data && current->right != nullptr)
        rightChild = recursiveInTree(data, current->right);
    if(currentNode == true || leftChild == true || rightChild == true){
        final = true;
    }
    return final;
}

template <class T>
void AVLIndex<T>::leftRotationRightChild(Node*& root){
    //cout << "left rotation right child" << endl;
    if(root->right->right->balanceFactor >= 1){
        rightLeftRotation(root->right);
    }
    Node* newRoot = root->right->right;
    root->right->right = root->right->right->left;
    newRoot->left = root->right;
    root->right = newRoot;
    calculateHeight(root->right->left);
    calculateHeight(root->right->right);
    calculateHeight(root->right);
    calculateHeight(root);
}

template <class T>
void AVLIndex<T>::leftRotationLeftChild(Node*& root){
    if(root->left->right->balanceFactor >= 1){
        rightLeftRotation(root->left);
    }
    Node* newRoot = root->left->right;
    root->left->right = root->left->right->left;
    newRoot->left = root->left;
    root->left = newRoot;
    calculateHeight(root->left->left);
    calculateHeight(root->left->right);
    calculateHeight(root->left);
    calculateHeight(root);
}

template <class T>
void AVLIndex<T>::leftHeadRotation(){
    if(head->right->balanceFactor >= 1){
        rightLeftRotation(head);
    }
    Node* tempNode = head;
    head = tempNode->right;
    tempNode->right = head->left;
    head->left = tempNode;
    calculateHeight(head->left);
    calculateHeight(head->right);
    calculateHeight(head);
}


template <class T>
void AVLIndex<T>::rightRotationLeftChild(Node*& root){
    if(root->left->left->balanceFactor <= -1){
        leftRightRotation(root->left);
    }
    Node* newRoot = root->left->left;
    root->left->left = root->left->left->right;
    newRoot->right = root->left;
    root->left = newRoot;
    calculateHeight(root->left->left);
    calculateHeight(root->left->right);
    calculateHeight(root->left);
    calculateHeight(root);

}

template <class T>
void AVLIndex<T>::rightRotationRightChild(Node*& root){
    if(root->right->left->balanceFactor <= -1){
        leftRightRotation(root->right);
    }
    Node* newRoot = root->right->left;
    root->right->left = root->right->left->right;
    newRoot->right = root->right;
    root->right = newRoot;
    calculateHeight(root->right->left);
    calculateHeight(root->right->right);
    calculateHeight(root->right);
    calculateHeight(root);

}

template <class T>
void AVLIndex<T>::rightHeadRotation(){
    if(head->left->balanceFactor <= -1){
        leftRightRotation(head);
    }
    Node* tempNode = head;
    head = tempNode->left;
    tempNode->left = head->right;
    head->right = tempNode;

    calculateHeight(head->left);
    calculateHeight(head->right);
    calculateHeight(head);
}

template <class T>
void AVLIndex<T>::leftRightRotation(Node*& root){
    Node* temp = root->left;
    root->left = root->left->right;
    Node* temp2 = root->left->left;
    root->left->left = temp;
    root->left->left->right = temp2;
}

template <class T>
void AVLIndex<T>::rightLeftRotation(Node*& root){
    Node* temp = root->right;
    root->right = root->right->left;
    Node* temp2 = root->right->right;
    root->right->right = temp;
    root->right->right->left = temp2;
}

/**
 * deleteIndex() clears trees of all nodes
 */
template <class T>
void AVLIndex<T>::deleteIndex(){
    if(this->head != nullptr)
        deleteRecursion(this->head);
    this->head = nullptr;
}

template <class T>
void AVLIndex<T>::deleteRecursion(Node* root){
    if(root->left != nullptr){
        deleteRecursion(root->left);
        root->left = nullptr;
    }
    if(root->right != nullptr){
        deleteRecursion(root->right);
        root->right = nullptr;
    }
    delete root;
    root=nullptr;
}

template <class T>
void AVLIndex<T>::displayRecursion(Node* root, string tabs){
    cout << root->data << ": " << root->height << "-> " << root->balanceFactor<< endl;
    tabs += "  ";
    if(root->left != nullptr){
        cout << tabs << "L: ";
        displayRecursion(root->left, tabs);
    }
    if(root->right != nullptr){
        cout << tabs << "R: ";
        displayRecursion(root->right, tabs);
    }
}

/**
 * preOrder() returns a vector of type T containing all nodes in the tree, add by root, the left and right children;
 */
template <class T>
vector<T> AVLIndex<T>::preOrder(){
    vector<T> preorder;
    if(this->head != nullptr)
       preorderRecursion(head, preorder);
    return preorder;
}

template <class T>
void AVLIndex<T>::preorderRecursion(Node*& current, vector<T>& preorder){
    preorder.push_back(current->data);
    if(current->left != nullptr){
        preorderRecursion(current->left, preorder);
    }
    if(current->right != nullptr){
        preorderRecursion(current->right, preorder);
    }
}

template <class T>
AVLIndex<T>::~AVLIndex(){
    deleteIndex();
}
