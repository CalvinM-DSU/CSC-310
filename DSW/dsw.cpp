#include "Includes/dsw.h"

// ----------------- PRIVATE ----------------------------------

// when left-heavy tree
void BST::rotateRight(Node*& node) // passing the parent
{
    if(node == nullptr || node->left == nullptr)
        return;
    
    // get the node to rotate R
    Node* leftChild = node->left; 
    // 1) leftnode's right child is going to become parent's left child
    node->left = leftChild->right;
    // 2) parent is going to be right child of node that is rotated
    leftChild->right = node;
    
    node = leftChild;
}

// when right-heavy
void BST::rotateLeft(Node*& node)
{
    if(node == nullptr || node->right == nullptr)
        return;

    // get the node to rotate L
    Node* rightChild = node->right;
    // 1) rightnode's left child is going to become parent's right child
    node->right = rightChild->left;
    // 2) parent is going to be left child of node that is rotated
    rightChild->left = node;

    node = rightChild;
}

// Phase 1 - right skewed linked list tree
void BST::createVine()
{
    Node** curr = &root;

    while (*curr != nullptr)
    {
        if ((*curr)->right != nullptr)
        {
            int size = getTreeSize((*curr)->right);

            if (size > 2)
            {
                rotateLeft(*curr);
            }
            else
            {
                curr = &((*curr)->left);
            }
        }
        else
        {
            curr = &((*curr)->left);
        }
    }
}

void BST::rebuildTree(int size)
{
    if (size <= 1)
        return;

    int targetHeight = 2 * log2(size);

    // just an estimate
    int rotations = size - (1 << (int)log2(size));

    performRotation(rotations);

    size -= rotations;

    while (size > targetHeight)
    {
        performRotation(size / 2);
        size /= 2;
    }
}

// left rotate every second node based count
void BST::performRotation(int count)
{
    Node** curr = &root;

    for (int i = 0; i < count; i++)
    {
        if (*curr == nullptr || (*curr)->left == nullptr)
            break;

        rotateRight(*curr);

        curr = &((*curr)->right);
        if (*curr == nullptr)
            break;

        curr = &((*curr)->right);
    }
}

void BST::printTree(Node* root, int space) {
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
    cout << root->data << endl;

    // Print the left child
    printTree(root->left, space);
}

int BST::getTreeSize(Node* node)
{
    int count = 0;
    Node* curr = node;

    // Morris traversal (O(1) space)
    while (curr != nullptr)
    {
        if (curr->left == nullptr)
        {
            count++;
            curr = curr->right;
        }
        else
        {
            Node* pred = curr->left;
            while (pred->right != nullptr && pred->right != curr)
                pred = pred->right;

            if (pred->right == nullptr)
            {
                pred->right = curr;
                curr = curr->left;
            }
            else
            {
                pred->right = nullptr;
                count++;
                curr = curr->right;
            }
        }
    }

    return count;
}

// --------------------- PUBLIC ------------------
BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    deleteTree(root);
}

void BST::deleteTree(Node*& node)
{
    if(node == nullptr)
        return;
    
    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

void BST::insert(int val)
{
    Node* newNode = new Node(val);
    if(root == nullptr){
        root = newNode;
        return;
    }
    
    Node* curr = root;
    Node*parent = nullptr;

    while(curr != nullptr)
    {
        parent = curr;
        if(val < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if(val < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

void BST::dswBalance()
{
    if (root == nullptr)
        return;

    // Phase 1
    createVine();

    // Count nodes in vine
    int size = 0;
    Node* curr = root;
    while (curr != nullptr)
    {
        size++;
        curr = curr->left;
    }

    // Phase 2
    rebuildTree(size);
}

void BST::display()
{
    cout << endl;
    printTree(root, 0);
    cout << endl;
}