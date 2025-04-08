#ifndef RECCHECK
#include <algorithm>

//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


//You may add any prototypes of helper functions here

bool hasKids(Node* root) {
    if ((root->right != nullptr) || (root->left != nullptr)) {
        return true;
    }
    return false;
}


// int find_height(Node* root) {
//     if (root == nullptr) {
//         return 0;
//     }
//     else{
//         return 1 + max(find_height(root->left), find_height(root->right));
//     }
    
// } 


bool equalPathHelper(Node* root, int currentDepth, int& leafDepth) {
    if (root == nullptr) {return true;}

    else if (!hasKids(root)) {
        if (leafDepth == -1) {
            leafDepth = currentDepth;
            return true;
        }
        return (leafDepth == currentDepth);
    }

    bool ok = true;
    if (root->left) {
        ok = ok && equalPathHelper(root->left, currentDepth + 1, leafDepth);
    }
    if (root->right) {
        ok = ok && equalPathHelper(root->right, currentDepth + 1, leafDepth);
    }
    return ok;
}



bool equalPaths(Node * root)
{
    int leaf_depth = -1;
    return equalPathHelper(root, 0, leaf_depth);

}

