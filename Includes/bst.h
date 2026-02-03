// Base Code - linked binary search tree

#include <iostream>
#include <fstream>
using namespace std;

// tree node for linked BST
struct treenode
{
    int dat;
    treenode *lchild;
    treenode *rchild;
};


// Class for binary search tree
// Private will be treenode * to the root

class bstree
{
 public:
        bstree();
        void in( );
        void pre( );
        void post( );
        void insert( int x );
        void remove( int x );
        bool search( int x );
        int numnodes(); // must compute, not a variable in class
        int height();   // must compute, not a variable in class
        void deleter(int x);
        void compareLR();
        bool isWorstCase();
 private:
        bool searchI(int x);
        bool searchR(treenode *t, int x);
        void in(treenode *t);
        void pre(treenode *t);
        void post(treenode *t);
//      void insert(treenode *t, int x);
        treenode *insert(treenode *t, int x);
        int numnodes(treenode *t);
        int height(treenode *t);
        treenode *deleter(treenode *t, int x);
        treenode *root;
        bool isWorstCase(treenode *t);
};