#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    int leafDepth = -1; //set the beginning depth to -1 to recognize first round 
    return checkPath(root, 0, leafDepth); //call helper recursive function
}

bool checkPath(Node * root, int currentDepth, int& depth){ //keeps track of root, current depth, and if it's first turn 
  if (root == nullptr){ //check if the root is nullptr
    return true; 
  }

  if (root->left == nullptr && root->right == nullptr){
    if (depth == -1){ //if it's a bottom leaf node, set depth = current depth 
      depth = currentDepth;
    }
    if (depth == currentDepth){ //compares depth with current node's depth  
      return true; //return true if depth match 
    }
    return false; 
  }
  
  bool left = checkPath(root->left, currentDepth +1, depth); //checks left node
  bool right = checkPath(root->right, currentDepth+1, depth); //checks right node
  
  if (left && right){ //if they both valid, then return true; 
    return true; 
  }
  return false; 
 
}
