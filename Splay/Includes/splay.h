#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <iostream>
using namespace std;

class SplayTree {
    private:
        struct Node {
            int key;
            int weight;
            Node* left;
            Node* right;
            Node(int k) {
                key = k;
                weight = 1;
                left = nullptr;
                right = nullptr; 
            }
        };

        Node* root;

        Node* rotateRight(Node* x);
        Node* rotateLeft(Node* x);
        Node* splay(Node* root, int key);
        Node* insertNode(Node* root, int key);
        Node* deleteNode(Node* root, int key);
        Node* searchNode(Node* root, int key);
        Node* semiSplay(Node* root, int key, int& rotations, int limit);
        Node* weightedSplay(Node* root, int key);

        void printTree(Node* root, int space);

    public:
        SplayTree();
        void insert(int key);
        void remove(int key);
        bool search(int key);
        void display();
        void semiSplay(int key, int k);
        void weightedSplay(int key);
};

#endif 
