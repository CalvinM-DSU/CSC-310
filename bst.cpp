#include <iostream>
#include <fstream>
using namespace std;

#include "bst.h"

// allocate a new node
//   not part of the class
treenode *makeatreenode( int x )
{
        treenode *node;
        node = new treenode;
        node->dat = x;
        node->lchild = NULL;
        node->rchild = NULL;

        return NULL;
}



bstree::bstree() {
        root = NULL;
}

void bstree::in(treenode *t) {
        if (t == NULL) return;
        in(t->lchild);
        cout << t->dat << " ";
        in(t->rchild);
}

void bstree::pre(treenode *t) {
        if (t == NULL) return;
        cout << t->dat << " ";
        pre(t->lchild);
        pre(t->rchild);
}

void bstree::post(treenode *t) {
        if (t == NULL) return;
        post(t->lchild);
        post(t->rchild);
        cout << t->dat << " ";
}

void bstree::in() {
        in(root);
}

void bstree::pre() {
        pre(root);
}

void bstree::post() {
        post(root);
}

void bstree::insert(int val) {
    root = insert(root, val);
}
treenode *bstree::insert(treenode *t, int val) {
    if (t == NULL) {
        treenode *node = new treenode;
        node->dat = val;
        node->lchild = NULL;
        node->rchild = NULL;
        return node;
    }

    if (val < t->dat)
        t->lchild = insert(t->lchild, val);
    else if (val > t->dat)
        t->rchild = insert(t->rchild, val);

    return t;
}


bool bstree::search( int val ) {
//      return searchI(val);
        return searchR(root, val);
}

bool bstree::searchR(treenode *t, int val) {
        if (t == NULL) return false;
        if (t->dat == val) return true;

        if (val < t->dat)
                return searchR(t->lchild, val);
        else
                return searchR(t->rchild, val);
}

bool bstree::searchI(int val) {
        treenode *t = root;

        while (t != NULL) {
                if (t->dat == val)
                        return true;

                if (val < t->dat)
                        t = t->lchild;
                else
                        t = t->rchild;
        } return false;
}

int bstree::height() {
        return height(root);
}

int bstree::height(treenode *t) {
        if (t == NULL)
                return 0;

        if (t->lchild == NULL && t->rchild == NULL)
                return 1;

        int hL, hR;
        hL = height(t->lchild);
        hR = height(t->rchild);


        if (hL > hR)
                return 1 + hL;
        else
                return 1 + hR;
}

int bstree::numnodes() {
        return numnodes(root);
}

int bstree::numnodes(treenode *t) {
        if (t == NULL) return 0;
        return numnodes(t->lchild) + numnodes(t->rchild) + 1;
}

void bstree::deleter(int val) {
        root = deleter(root, val);
}

treenode* bstree::deleter(treenode *t, int val) {
        if (t == NULL)
                return t;

        if (val < t->dat)
                t->lchild = deleter(t->lchild, val);
        else if (val > t->dat)
                t->rchild = deleter(t->rchild, val);

        else {
                if (t->lchild == NULL && t->rchild == NULL) {
                        delete t;
                        return NULL;
                }
                else if (t->lchild == NULL) {
                        treenode *temp = t->rchild;
                        delete t;
                        return temp;
                }
                else if (t->rchild == NULL) {
                        treenode *temp = t->lchild;
                        delete t;
                        return temp;
                }
                else {
                        treenode *next = t->rchild;
                        while (next != NULL && next->lchild != NULL)
                                next = next->lchild;

                        t->dat = next->dat;
                        t->rchild = deleter(t->rchild, next->dat);
                }
        }
        return t;
}

void bstree::compareLR() {
        if (root == NULL) {
                cout << "Empty tree" << endl;
                return;
        }

        int hL = height(root->lchild);
        int hR = height(root->rchild);

        if (hL > hR)
                cout << "L taller" << endl;
        else if (hR > hL)
                cout << "R taller" << endl;
        else
                cout << "E taller" << endl;
}

bool bstree::isWorstCase() {
    return isWorstCase(root);
}

bool bstree::isWorstCase(treenode *t) {
    if (t == NULL)
        return true;

    if (t->lchild != NULL && t->rchild != NULL)
        return false;

    if (t->lchild != NULL)
        return isWorstCase(t->lchild);
    else
        return isWorstCase(t->rchild);
}