#include "Includes/splay.h"

SplayTree::SplayTree() 
{
    root = nullptr; 
}

// aka Zig
SplayTree::Node* SplayTree::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// aka Zag
SplayTree::Node* SplayTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Splaying :)
SplayTree::Node* SplayTree::splay(Node* root, int key) {
    if (root == nullptr || root->key == key)
        return root;
    
    // key in left subtree
    if (key < root->key) {
        if (root->left == nullptr)
            return root;
        
        // Zig Zig
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rotateRight(root);
        }
        // Zig Zag
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != nullptr)
                root->left = rotateLeft(root->left);
        }

        if (root->left == nullptr)
            return root;
        else
            return rotateRight(root);
    }

    // key in right subtree
    if (key > root->key) {
        if (root->right == nullptr)
            return root;
        
        // Zag Zag
        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = rotateLeft(root);
        }
        // Zag Zig
        else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != nullptr)
                root->right = rotateRight(root->right);
        }

        if (root->right == nullptr)
            return root;
        else
            return rotateLeft(root);
    }
}


SplayTree::Node* SplayTree::insertNode(Node* root, int key) {
    if (root == nullptr)
        return new Node(key);
    
    root = splay(root, key); // splay the closest value to be the new root
    
    if (root->key == key)
        return root;
    
    Node* newNode = new Node(key);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr;
    }

    return newNode;
}


SplayTree::Node* SplayTree::deleteNode(Node* root, int key) {
    if (root == nullptr)
        return nullptr;

    // Bring key to root
    root = splay(root, key);

    // Key not found
    if (root->key != key)
        return root;

    Node* temp;

    // No left subtree
    if (root->left == nullptr) {
        temp = root->right;
        delete root;
        return temp;
    }

    // Has left subtree
    temp = root;
    root = splay(root->left, key);  // splay max of left subtree
    root->right = temp->right;
    delete temp;

    return root;
}

SplayTree::Node* SplayTree::searchNode(Node* root, int key) {
    return splay(root, key);
}

void SplayTree::insert(int key) {
    root = insertNode(root, key);
}


void SplayTree::remove(int key) {
    root = deleteNode(root, key);
}


bool SplayTree::search(int key) {
    root = searchNode(root, key);
    return (root && root->key == key);
}


void SplayTree::printTree(Node* root, int space) {
    const int COUNT = 10; 

    if (root == nullptr) {
        return;
    }

    // Increase the distance between levels
    space += COUNT;

    // Print the right child first (to appear on top)
    printTree(root->right, space);

    // Print the current node after right child

    for (int i = COUNT; i < space; i++) {
        cout << " "; // Indentation for tree depth
    }
    cout << root->key << endl;

    // Print the left child
    printTree(root->left, space);
}

void SplayTree::display() {
    printTree(root, 0);
    cout << endl;
}

SplayTree::Node* SplayTree::semiSplay(Node* root, int key, int& rotations, int limit) {
    if (root == nullptr || root->key == key || rotations >= limit)
        return root;

    if (key < root->key) {
        if (root->left == nullptr)
            return root;

        if (key < root->left->key) {
            root->left->left = semiSplay(root->left->left, key, rotations, limit);
            if (rotations < limit) {
                root = rotateRight(root);
                rotations++;
            }
        }
        else if (key > root->left->key) {
            root->left->right = semiSplay(root->left->right, key, rotations, limit);
            if (root->left->right != nullptr && rotations < limit) {
                root->left = rotateLeft(root->left);
                rotations++;
            }
        }

        if (rotations < limit && root->left != nullptr) {
            rotations++;
            return rotateRight(root);
        }
    }
    else {
        if (root->right == nullptr)
            return root;

        if (key > root->right->key) {
            root->right->right = semiSplay(root->right->right, key, rotations, limit);
            if (rotations < limit) {
                root = rotateLeft(root);
                rotations++;
            }
        }
        else if (key < root->right->key) {
            root->right->left = semiSplay(root->right->left, key, rotations, limit);
            if (root->right->left != nullptr && rotations < limit) {
                root->right = rotateRight(root->right);
                rotations++;
            }
        }

        if (rotations < limit && root->right != nullptr) {
            rotations++;
            return rotateLeft(root);
        }
    }

    return root;
}

void SplayTree::semiSplay(int key, int k) {
    int rotations = 0;
    root = semiSplay(root, key, rotations, k);
}

SplayTree::Node* SplayTree::weightedSplay(Node* root, int key) {
    if (root == nullptr)
        return nullptr;

    root = splay(root, key);

    if (root && root->key == key)
        root->weight++;   // increase access frequency

    return root;
}

void SplayTree::weightedSplay(int key) {
    if (root->left && root->left->weight >= root->weight)
    root = rotateRight(root);
}