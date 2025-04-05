#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    /*BinarySearchTree<int,int> bt;
    bt.insert(std::make_pair(25,25));
    bt.insert(std::make_pair(18,18));
    bt.insert(std::make_pair(47,47));
    bt.insert(std::make_pair(7,7));
    bt.insert(std::make_pair(20,20));
    bt.insert(std::make_pair(32,32));
    bt.insert(std::make_pair(56,56));
    bt.insert(std::make_pair(26,26));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<int,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    
    if(bt.find(7) != bt.end()) {
        cout << "Found 7" << endl;
    }
    else {
        cout << "Did not find 7" << endl;
    }
    cout << "Erasing 47" << endl;
    bt.remove(47);
    

    BinarySearchTree<int, int> testTree;

	  testTree.insert(std::make_pair(2, 8));
	  testTree.insert(std::make_pair(1, -50));
	  testTree.insert(std::make_pair(3, 0));
	  testTree.insert(std::make_pair(10, 0));
	  testTree.insert(std::make_pair(15, 0));

    testTree.remove(-1);
    testTree.remove(2);
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<int,int>::iterator it = testTree.begin(); it != testTree.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }*/


    // AVL Tree Tests
    /*AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');*/


    /*AVLTree<uint16_t, uint16_t> testTree;

	  testTree.insert(std::make_pair(48, 9));
	  testTree.insert(std::make_pair(35, 8));
	  testTree.insert(std::make_pair(20, 159));
    testTree.insert(std::make_pair(11, 89));
    testTree.insert(std::make_pair(60, 109));
    testTree.insert(std::make_pair(30, 109));
    testTree.insert(std::make_pair(25, 109));
    testTree.insert(std::make_pair(28, 109));
    
    */
    AVLTree<int, double> bst;
	  bst.insert(std::make_pair(2, 1.0));
	  bst.insert(std::make_pair(1, 1.0));
	  bst.insert(std::make_pair(3, 1.0));
	  bst.insert(std::make_pair(4, 1.0));
	  bst.remove(1);

    for(AVLTree<int, double>::iterator it = bst.begin(); it != bst.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }

    return 0;
}
