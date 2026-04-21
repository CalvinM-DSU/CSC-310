#include "Includes/binomial.h"
#include "Includes/customErrorClass.h"

binomialNode::binomialNode(int k)
{
    key = k;
    degree = 0;
    parent = nullptr;
    sibling = nullptr;
    child = nullptr;
}

// ---------------------- PRIVATE FUNCTIONS ------------------------------
binomialNode* BinomialHeap::unionHeap(binomialNode* heap1, binomialNode* heap2)
{
    if(!heap1) return heap2;
    if(!heap2) return heap1;

    binomialNode* newHead = nullptr;
    binomialNode** pos = &newHead;

    while(heap1 && heap2) {
        if(heap1->degree <= heap2->degree) {
            *pos = heap1;
            heap1 = heap1->sibling;
        } else {
            *pos = heap2;
            heap2 = heap2->sibling;
        }
        pos = &((*pos)->sibling);
    }
    // add remaining trees, if any
    *pos = (heap1) ? heap1 : heap2;

    return newHead;
}


binomialNode* BinomialHeap::mergeTrees(binomialNode* tree1, binomialNode* tree2)
{
    // assuming tree1 has the smallest root
    // to keep min heap property
    if(tree1->key > tree2->key) swap(tree1, tree2);

    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}

void BinomialHeap::linkTrees(binomialNode*& prev, binomialNode*& cur, binomialNode*& next)
{
    if(cur->degree != next->degree || next->sibling && next->sibling->degree == cur->degree) {
        prev = cur;
        cur = next;
    } else { // we have a violation aka duplicates
        if(cur->key <= next->key) {
            cur->sibling = next->sibling;
            mergeTrees(cur, next);
        } else {
            if(!prev) head = next;
            else prev->sibling = next;

            mergeTrees(next, cur);
            cur = next;
        }
    }
}

// ---------------- PUBLIC FUNCTIONS ------------------------------
BinomialHeap::BinomialHeap()
{
    head = nullptr;
}

void BinomialHeap::insert(int key)
{
    BinomialHeap tempHeap;
    tempHeap.head = new binomialNode(key);

    head = unionHeap(head, tempHeap.head);

    if(!head || !head->sibling) return;

    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while(next) {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }
}

void BinomialHeap::merge(BinomialHeap& other)
{
    head = unionHeap(head, other.head);

    if(!head || !head->sibling) return;

    binomialNode* prev = nullptr;
    binomialNode* cur = head;
    binomialNode* next = cur->sibling;

    while(next) {
        linkTrees(prev, cur, next);
        next = cur->sibling;
    }

}

int BinomialHeap::findMin() // log n operation but it can O(1) with a min pointer
{
    if(!head) return -1;

    binomialNode* minNode = head;
    int minKey = head->key;

    binomialNode* temp = head->sibling;
    while(temp) {
        if(temp->key < minKey) {
            minKey = temp->key;
            minNode = temp;
        }
        temp = temp->sibling;
    }

    return minKey;
}

void BinomialHeap::printHeap() {
    cout << "Binomial Heap:\n";
    binomialNode* current = head;


    while (current != nullptr) {
        cout << endl << "Tree of degree " << current->degree << endl;
        printTree(current, 0);
        current = current->sibling;
    }

}

void BinomialHeap::printTree(binomialNode* node, int space) 
{
    if (node == nullptr) 
        return;

    cout << setw(space * 2) << node->key << endl;

    binomialNode* child = node->child;
    while (child) {
        printTree(child, space + 3);
        child = child->sibling;
    }
}

binomialNode* BinomialHeap::findNode(binomialNode* root, int key) {
    if (!root) return nullptr;

    if (root->key == key) return root;

    binomialNode* res = findNode(root->child, key);
    if (res) return res;

    return findNode(root->sibling, key);
}

int BinomialHeap::deleteMin()
{
    if (!head) {
        throw MyException("deleteMin() called on empty heap");
    }

    binomialNode* minNode = head;
    binomialNode* minPrev = nullptr;
    binomialNode* prev = nullptr;
    binomialNode* curr = head;

    int minKey = head->key;

    // find min
    while (curr) {
        if (curr->key < minKey) {
            minKey = curr->key;
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }

    // remove min
    if (minPrev)
        minPrev->sibling = minNode->sibling;
    else
        head = minNode->sibling;

    binomialNode* child = minNode->child;
    binomialNode* revHead = nullptr;

    while (child) {
        binomialNode* next = child->sibling;
        child->sibling = revHead;
        child->parent = nullptr;
        revHead = child;
        child = next;
    }

    head = unionHeap(head, revHead);

    if (head) {
        binomialNode* prev2 = nullptr;
        binomialNode* cur = head;
        binomialNode* next2 = cur->sibling;

        while (next2) {
            linkTrees(prev2, cur, next2);
            next2 = cur->sibling;
        }
    }

    delete minNode;
    return minKey;
}

void BinomialHeap::decreaseKey(int oldKey, int newKey)
{
    if (newKey > oldKey) {
        throw MyException("decreaseKey(): new key is greater than old key");
    }

    binomialNode* node = findNode(head, oldKey);

    if (!node) {
        throw MyException("decreaseKey(): key not found");
    }

    node->key = newKey;

    binomialNode* parent = node->parent;

    while (parent && node->key < parent->key) {
        swap(node->key, parent->key);
        node = parent;
        parent = node->parent;
    }
}

void BinomialHeap::deleteKey(int key)
{
    binomialNode* node = findNode(head, key);

    if (!node) {
        throw MyException("deleteKey(): key not found");
    }

    decreaseKey(key, INT_MIN);

    deleteMin();
}