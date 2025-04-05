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

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

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

    // Add helper functions here
    AVLNode<Key,Value>* rotateLeft(AVLNode<Key,Value>* current);
    AVLNode<Key,Value>* rotateRight(AVLNode<Key,Value>* current);
    AVLNode<Key,Value>* rotateLeftRight(AVLNode<Key,Value>* current);
    AVLNode<Key,Value>* rotateRightLeft(AVLNode<Key,Value>* current);
    AVLNode<Key,Value>* rebalance(AVLNode<Key,Value>* value);
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* value);
    void removeFix(AVLNode<Key,Value>* parent, int diff);
    AVLNode<Key,Value>* predecessor(AVLNode<Key,Value>* value);

    bool isBalanced() const; 
    int helperBalance(Node<Key,Value>* current) const;
    


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_==nullptr){
      this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr); //if root null, set new key to root 
      //std::cout << "This is now new root " << new_item.first << std::endl;
      return;
    }
    
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_); //make current node = root
    AVLNode<Key, Value>* parent = nullptr;

    while (current!=nullptr){ //walk through tree to find leaf
      if (new_item.first == current->getKey()){
        current->setValue(new_item.second);
        return;
      }
      else if (new_item.first > current->getKey()){
        parent = current;
        current = current->getRight();
      }
      else{
        parent = current;
        current = current->getLeft();
      }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key,Value> (new_item.first, new_item.second, parent); //make new node
    if (new_item.first>parent->getKey()){ //check if new node is bigger than parent 
      parent->setRight(newNode); //set current's parent's right to be new node
      newNode->setBalance(0); //set new node's balance to 0
      if (parent->getBalance()==-1){ //if parent -1 and added one on right, becomes 0
        parent->setBalance(0);
      }
      else if (parent->getBalance()==1){ 
        parent->setBalance(0);
      }
      else{ //parent becomes unbalanced
      //std::cout <<"inserright to insertfix" << std::endl;
        parent->setBalance(1);
        insertFix(parent, newNode); //call insert fix
      }
    }

    else{ //if key smaller than leaf
      parent->setLeft(newNode); //set it as left child 
      newNode->setBalance(0); //set balance to 0
      if (parent->getBalance()==-1){
        parent->setBalance(0); 
      }
    
      else if (parent->getBalance()==1){ //if already have node on right, just make it 0 
        parent->setBalance(0);
      }

      else{
        parent->setBalance(-1); //if unbalanced after insertion, call insertfix 
        insertFix(parent, newNode);
      }
    }

    if (!isBalanced()){ //checks if unbalanced
      //std::cout<< "Tree is unbalancd after inserting " << new_item.first << std::endl;
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key,Value>* current = static_cast<AVLNode<Key,Value>*>(this->root_); //take the current root 
    AVLNode<Key,Value>* parent = nullptr;
    int diff = 0; 

    while (current!=nullptr){ //walk the tree to find the key 
      if (current->getKey()==key){
        break;
      }
      if (key > current->getKey()){
        current = current->getRight();
      }
      else{
        current = current->getLeft();
      }
    }
    
    if (current == nullptr){ //return null if not found
      return; 
    }

    
    if (current->getLeft()!=nullptr && current->getRight()!=nullptr){ //if the node has 2 children, swap with predecessor 
      AVLNode<Key,Value>* pred = predecessor(current);
      nodeSwap(current,pred);
    }

    AVLNode<Key,Value>* child = nullptr; //set the child 
    if (current->getLeft()!=nullptr){
      child = current->getLeft(); //set the child to left or right 
    }
    else{
      child = current->getRight();
    }

    parent = current->getParent(); //set the parent
    if (parent!=nullptr){
      if (current == parent->getLeft()){ //if new node is left child, diff = 1
        diff = 1; 
        parent->setLeft(child); //set parent's left to child 
      }
      else{
        diff = -1; 
        parent->setRight(child); //set parent's right to child 
      }
    }
    else{
      this->root_ = child; //if parent null, becomes new root 
      //std::cout<< "this is now root " << child->getKey() << std::endl;
    }

    if (child!=nullptr){
      child->setParent(parent); //if child isn't null, set its new parent 
    }
    delete current; //delete current node
    if (parent!=nullptr){
      removeFix(parent, diff); //if parent not null, removefix
    }

    if (!isBalanced()){ //checks if balanced 
      //std::cout<< "Tree is unbalancd after removing " << key << std::endl;
    }
}

template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* n, int diff)
{
  if (n == nullptr){ //return if n is null
    return;
  }
  AVLNode<Key,Value>* parent = n->getParent(); //set the node to be removed's parent 
  int ndiff = 0; 

  if (parent!=nullptr){ //check if parent's null
    if (n==parent->getLeft()){
      ndiff = 1; //if left child, set ndiff to 1
    }
    else{
      ndiff = -1; //else set -1 
    }
  }

  int newBalance = n->getBalance()+diff; //calculate balance after deleting child 
  if (diff == -1){ //if left 
    if (newBalance == -2){ //left heavy 
      AVLNode<Key,Value>* child = n->getLeft(); //store the child 
      if (child->getBalance() <=0){ //rotate right 
        AVLNode<Key,Value>* newNode = rotateRight(n);
        if (child->getBalance() == 0){ //set balance 
          n->setBalance(-1);
          child->setBalance(1);
          return;
        }
        else{ //set all balance to 0 after rotation 
          n->setBalance(0); 
          child->setBalance(0);
          newNode->setBalance(0);
          removeFix(parent, ndiff);
        }
        //std::cout <<"Bal:left node and roting right " << n->getBalance() << child->getBalance() << newNode->getBalance()<< std::endl;
      }
      else{ //zigzag
        AVLNode<Key,Value>* grandchildren = child->getRight(); //get the grandchildren for zigzag 
        rotateLeftRight(n);
        if (grandchildren->getBalance()==-1){ //set balance  
          child->setBalance(0);
          n->setBalance(1);
        }
        else if (grandchildren->getBalance()==0){
          child->setBalance(0);
          n->setBalance(0);
        }
        else{
          child->setBalance(-1);
          n->setBalance(0);
        }
        grandchildren->setBalance(0);
        //std::cout <<"Balance after removing left node and leftright " << n->getBalance() << child->getBalance() << grandchildren->getBalance() << std::endl;
        removeFix(parent, ndiff);
      }
    }
    else if (newBalance == -1){ //set balance, no rotation 
      n->setBalance(-1);
      //std::cout << "Left and no rotate bal: " << n->getBalance() << std::endl;
    }
    else if (newBalance == 0){ //set balance, recurse again 
      n->setBalance(0);
      //std::cout << "Left and removeFix bal: " << n->getBalance() << std::endl;
      removeFix(parent, ndiff);
    }
  }
  else if (diff==1){ //right child

    if (newBalance == 2){ //right heavy 
      AVLNode<Key,Value>* child = n->getRight();
      if (child->getBalance() >=0){ //zigzig
        //std::cout <<"right child rotateleft" << std::endl;
        AVLNode<Key,Value>* newNode = rotateLeft(n);
        if (child->getBalance() == 0){ //set balance
          n->setBalance(1);
          child->setBalance(-1);
          return;
        }
        else{
          n->setBalance(0);
          child->setBalance(0);
          newNode->setBalance(0);
          removeFix(parent, ndiff); //recurse again 
        }
      }
      else{ //zigzag
        AVLNode<Key,Value>* grandchildren = child->getLeft();
        //std::cout <<"Left child rotaterightleft" << std::endl;
        rotateRightLeft(n);
        if (grandchildren->getBalance()==1){ //set balance 
          child->setBalance(0);
          n->setBalance(-1);
        }
        else if (grandchildren->getBalance()==0){
          child->setBalance(0);
          n->setBalance(0);
        }
        else{
          child->setBalance(1);
          n->setBalance(0);
        }
        grandchildren->setBalance(0);
        removeFix(parent, ndiff);
      }
    }
    else if (newBalance == 1){
      n->setBalance(1);
    }
    else if (newBalance == 0){
      n->setBalance(0);
      removeFix(parent, ndiff);
    }
  }
}



template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* value)
{
  if (parent == nullptr){ //return if parent null 
    return;
  }
  AVLNode<Key,Value>* g = parent->getParent(); //get grandparent

  if (g == nullptr){ //return if null 
    return; 
  }

  if (g->getLeft()==parent){ //if parent is left child 
    g->updateBalance(-1); //balance -1
    if (g->getBalance()==0){ //return if grandp 0 
      return;
    }
    else if(g->getBalance()==-1){
      insertFix(g, parent); //move up the ancestor if -1
    }
    else{
      if (value == parent->getLeft()){ //if left child 
        rotateRight(g); //rotate right 
        g->setBalance(0); //set all to 0 
        parent->setBalance(0);
      }
      else{
        AVLNode<Key,Value>* child = parent->getRight(); //get right child before rotation 
        //AVLNode<Key,Value>* newRoot = rotateLeftRight(g);
        rotateLeft(parent);
        rotateRight(g);
        if (child->getBalance()==-1){ //if it was -1 before
          parent->setBalance(0);
          g->setBalance(1);
        }
        else if (child->getBalance()==0){ //if 0 
          parent->setBalance(0);
          g->setBalance(0);
        }
        else{
          parent->setBalance(-1);
          g->setBalance(0);
        }
        value->setBalance(0); //set added node to 0 
      }
    }
  }
  else if (g->getRight() == parent){ //if parent is right child 
    //std::cout << "parent is right of grandp" << std::endl;
    g->updateBalance(1); //add 1 to balance of grandp
    if (g->getBalance()==0){
      return;
    }
    else if(g->getBalance()==1){
      //std::cout << "parent is right of grandp and balance ==1" << std::endl;
      insertFix(g, parent); //recurse upward 
    }
    else if (g->getBalance()>1){
      if (value == parent->getRight()){
        //std::cout << "Zigzig left case detected " << std::endl;
        rotateLeft(g);
        g->setBalance(0);
        parent->setBalance(0);
      }
      else{
        //std::cout << "Zigzag rightleft case detected " << std::endl;
        AVLNode<Key,Value>* child = parent->getLeft(); 
        int balance = child->getBalance();
        rotateRight(parent);
        rotateLeft(g);
        //AVLNode<Key,Value>* newNode = rotateRightLeft(g);
        if (balance==-1){
          parent->setBalance(1);
          g->setBalance(0);
        }
        else if (child->getBalance()==0){
          parent->setBalance(0);
          g->setBalance(0);
        }
        else{
          parent->setBalance(0);
          g->setBalance(-1);
        }
        value->setBalance(0);
      }
    }
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

template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::predecessor(AVLNode<Key,Value>* value) //finds pred 
{
  if (value == nullptr || value->getLeft()==nullptr){
      return nullptr;
  }
  AVLNode<Key, Value>* pred = value->getLeft();
  while (pred->getRight()!=nullptr){ //gets the rightmost value from left tree 
      pred = pred->getRight();
  }
  return pred; 
}

template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* current)
{
  AVLNode<Key,Value>* newRoot = current->getRight(); //get right value 
  if (newRoot==nullptr){ //if null return 
    return current;
  }
  AVLNode<Key,Value>* parent = current->getParent(); //get parnet to see if root 
  AVLNode<Key,Value>* child = newRoot->getLeft(); //get the hanging child that will rotate with it 

  newRoot->setLeft(current); //set right child's left to be parent 
  current->setRight(child); //set the hanging child from newRoot->current
  current->setParent(newRoot); //set current's new parent to newRoot 
  if (child!=nullptr){ //if hanging child not null, set its parnet to current
    child->setParent(current);
  }
  newRoot->setParent(parent); //set newRoot's parent to current's parent 

  if (parent==nullptr){
    this->root_ = newRoot; //if null, set it to root 
    //std::cout<< "this is now root " << newRoot->getKey() << std::endl;
  }
  else if (parent->getLeft()==current){ //if current was left child, set newRoot to parent's left 
    parent->setLeft(newRoot);
  }
  else{
    parent->setRight(newRoot); //else set right 
  }
  //std::cout <<"this is new root after rotating left" << newRoot->getKey() << std::endl;
  return newRoot; //return new value 
}

template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* current)
{
  AVLNode<Key,Value>* newRoot = current->getLeft(); //get left value 
  if (newRoot==nullptr){ //return if null 
    return current;
  }
  AVLNode<Key,Value>* parent = current->getParent(); 
  AVLNode<Key,Value>* child = newRoot->getRight(); //get hanging child 

  newRoot->setRight(current); //same as rotateLeft but mirroed 
  current->setLeft(child);
  current->setParent(newRoot);

  if (child!=nullptr){
    child->setParent(current);
  }
  newRoot->setParent(parent);
  if (parent==nullptr){
    this->root_ = newRoot;
    //std::cout<< "this is now root " << newRoot->getKey() << std::endl;
  }
  else if (parent->getLeft()==current){
    parent->setLeft(newRoot);
  }
  else{
    parent->setRight(newRoot);
  }
  return newRoot; 
}

template<class Key, class Value> //manually rotating left and right
AVLNode<Key,Value>* AVLTree<Key, Value>::rotateLeftRight(AVLNode<Key,Value>* current)
{
  AVLNode<Key,Value>* parent = current->getLeft(); //get left and right child 
  AVLNode<Key,Value>* child = parent->getRight();

  if (child==nullptr){
    return current; 
  }

  AVLNode<Key,Value>* grandparent = current->getParent(); //get grandparent 
  
  //first rotation to the left
  parent->setRight(child->getLeft()); //set child's left child to parent's right 
  if (child->getLeft()!=nullptr){ //check if null
    child->getLeft()->setParent(parent);
  }
  child->setLeft(parent); //set parent to be child's left
  parent->setParent(child); //set parent's parent to be child, swapping the two 

  //second rotation to the right 
  current->setLeft(child->getRight()); //see if child has kids on right
  if (child->getRight()!=nullptr){
    child->getRight()->setParent(current); //set the kids to their new parent 
  }

  child->setRight(current); //current becomes right of child 
  current->setParent(child);

  child->setParent(grandparent);
  if (grandparent==nullptr){ //check to see if root 
    this->root_ = child; 
    //std::cout<< "this is now root " << child->getKey() << std::endl;
  }
  else if (current==grandparent->getLeft()){ //set grandparent to new child 
    grandparent->setLeft(child);
  }
  else if (current==grandparent->getRight()){
    grandparent->setRight(child);
  }
  return child; 
}

template<class Key, class Value> //manually rotating right and left, mirroed of leftright 
AVLNode<Key,Value>* AVLTree<Key, Value>::rotateRightLeft(AVLNode<Key,Value>* current)
{
  AVLNode<Key,Value>* parent = current->getRight();
  AVLNode<Key,Value>* child = parent->getLeft();
  if (child==nullptr){
    return current; 
  }

  AVLNode<Key,Value>* grandparent = current->getParent(); 
  
  //first rotation to the right
  parent->setLeft(child->getRight()); //set child's right child to parent's left 
  if (child->getRight()!=nullptr){ //check if null
    child->getRight()->setParent(parent);
  }
  child->setRight(parent); //set parent to be child's right
  parent->setParent(child); //set parent's parent to be child, swapping the two 

  //second rotation to the left
  current->setRight(child->getLeft()); //see if child has kids on left
  if (child->getLeft()!=nullptr){
    child->getLeft()->setParent(current); //set the kids to their new parent 
  }

  child->setLeft(current); //current becomes left of child 
  current->setParent(child);

  child->setParent(grandparent);
  if (grandparent==nullptr){ //check to see if root 
    this->root_ = child; 
    //std::cout<< "this is now root " << child->getKey() << std::endl;
  }
  else if (current==grandparent->getLeft()){
    grandparent->setLeft(child);
  }
  else if (current==grandparent->getRight()){
    grandparent->setRight(child);
  }
  return child;
}

template<class Key, class Value> //check if balanced 
bool AVLTree<Key, Value>::isBalanced() const
{
    // TODO
    int result = helperBalance(this->root_);
    //std::cout << "final result " << result << std::endl;
    if (result == -1){
      //std::cout << "isbalanced bool inside " << std::endl;
      return false;
    }
    //std::cout << "isbalanced bool " <<std::endl;
    return true; 
}

template<typename Key, typename Value>
int AVLTree<Key,Value>::helperBalance(Node<Key,Value>* current) const {
  int left = 0;
  int right = 0;
  if (current == nullptr){
    return 0; 
  }
  left = helperBalance(current->getLeft());
  if (left == -1){
    return -1; 
  }
  right = helperBalance(current->getRight());
  if (right == -1){
    return -1;
  }
  if (std::abs(right-left)>1){
    return -1;
  }

  AVLNode<Key,Value>* node = static_cast<AVLNode<Key,Value>*>(current); //get the current node 
  int actualBal = right-left; //find actual balance 
  if (actualBal!=node->getBalance()){
    //std::cout <<"Balance mismatch at node " << current->getKey() << ": expected " << actualBal << ": stored " << static_cast<int>(node->getBalance()) << std::endl;
  }

  if (right>left){
    //std::cout << "right" << right + 1 << std::endl;
    return right +1;
  }
  else{
    //std::cout << "left" << left + 1 << std::endl;
    return left +1;
  }
}



#endif
