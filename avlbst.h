#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

    int16_t findBalance() const;
    int16_t getHeight(AVLNode<Key, Value>* node) const;

protected:
    int8_t balance_;    // effectively a signed char

};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/





template<class Key, class Value>
int16_t AVLNode<Key, Value>::findBalance() const{
    if (this == nullptr) {return 0;}
    int16_t leftH = getHeight(this->getLeft());
    int16_t rightH = getHeight(this->getRight());

    int16_t balance = leftH - rightH;
    return balance;

}



template<class Key, class Value>
int16_t AVLNode<Key, Value>::getHeight(AVLNode<Key, Value>* node) const {
    if (node == nullptr) {
        return 0; //tree is empty
    }
    
    int16_t h_left = getHeight(node->getLeft());
    int16_t h_right = getHeight(node->getRight());

    return 1 + std::max(h_left, h_right);

}




/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rebalance(AVLNode<Key, Value>* node);


    // Add helper functions here


};


template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    
    if ((node == nullptr) || (node->getLeft() == nullptr)) {
        return node;
    }
    
    
    AVLNode<Key, Value>* nodeP = node->getParent();
    AVLNode<Key, Value>* nodeL = node->getLeft();
    AVLNode<Key, Value>* nodeLR = nodeL->getRight();

    nodeL->setParent(nodeP);
    nodeL->setRight(node);
    if (nodeP == nullptr) {
        this->root_ = nodeL;
    }
    else if (nodeP->getLeft() == node) {
        nodeP->setLeft(nodeL);
    }
    else {
        nodeP->setRight(nodeL);
    }

    node->setParent(nodeL);
    node->setLeft(nodeLR);

    if (nodeLR != nullptr) {
        nodeLR->setParent(node);
    }

    nodeL->setBalance(nodeL->findBalance());
    node->setBalance(node->findBalance());
    if (nodeP != nullptr) {
        nodeP->setBalance(nodeP->findBalance());
    }


    return nodeL;
}



template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* nodeP = node->getParent();
    AVLNode<Key, Value>* nodeR = node->getRight();
    AVLNode<Key, Value>* nodeRL = nodeR->getLeft();

    if ((node == nullptr) || (node->getRight() == nullptr)) {return node; }

    nodeR->setParent(nodeP);
    if (nodeP == nullptr) {this->root_= nodeR; }
    else if (nodeP->getLeft() == node) {
        nodeP->setLeft(nodeR);
    }
    else {
        nodeP->setRight(nodeR);
    }
   
    nodeR->setLeft(node);


    node->setParent(nodeR);
    node->setRight(nodeRL);

    if (nodeRL != nullptr) {nodeRL->setParent(node);}

    nodeR->setBalance(nodeR->findBalance());
    node->setBalance(node->findBalance());
    if (nodeP != nullptr) {
        nodeP->setBalance(nodeP->findBalance());
    }

    return nodeR;

}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key,Value>::rebalance(AVLNode<Key, Value>* node) {
    if (node == nullptr) {return node;}
    int16_t balance = node->findBalance();
    node->setBalance(balance);

    if (balance > 1) {
        if (node->getLeft()->getBalance() < 0) { //LR
            node->setLeft(rotateLeft(node->getLeft()));
        }

        return rotateRight(node);
    }

    if(balance < -1) {
        if (node->getRight()->getBalance() > 0) {
            node->setRight(rotateRight(node->getRight()));
        }
        return rotateLeft(node);
    }

    return node;
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    if (this->root_ == nullptr) {
        AVLNode<Key, Value>* node_new2 = new AVLNode<Key, Value> (new_item.first, new_item.second, nullptr);
        this->root_ = node_new2;
        return;
     }

     AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
     AVLNode<Key, Value>* parent = nullptr;

     while(current != nullptr) {
        parent = current;

        if (new_item.first <current->getKey()) {
            current = current->getLeft();
        }
        else if (new_item.first > current->getKey()) {
            current = current->getRight();
        }
        else {
            current -> setValue(new_item.second);
            return;

        }
     }

    AVLNode<Key, Value>* node_new = new AVLNode<Key, Value> (new_item.first, new_item.second, nullptr);
    node_new->setParent(parent);
    parent->setBalance(parent->findBalance());
    
    if (new_item.first < parent->getKey()) {
        parent->setLeft(node_new);
    } else {
        parent->setRight(node_new);
    }


    node_new->setBalance(node_new->findBalance()) ;

    current = parent;
    while(current != nullptr) {
        current = rebalance(current);
        current = current->getParent();
    }



}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if( node == nullptr) {return;}//key not found
    
    if ((node->getLeft() != nullptr) && node->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, pred);
    }
     
    AVLNode<Key, Value>* child = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
    //IDK if this sort of if statement works on c++11 might cause error
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(node->getParent());

    if (child != nullptr) {
        child->setParent(parent);
    }

    if (parent == nullptr) {
        this->root_ = child;
    }

    else if (parent->getLeft() == node) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }

    delete node;

    AVLNode<Key, Value>* current = parent;
    while (current) {
        current = rebalance(current);
        current = current->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
