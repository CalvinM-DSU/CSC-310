// persistent BST
// path copying
// shared pointers - keep track of refcount  for delete()
// thread-safe

#include <iostream>
#include <memory>

using namespace std;

struct Node {
    int key;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
    // int refcount;  it is not thread-safe 

    Node(int k, shared_ptr<Node> l = nullptr, shared_ptr<Node> r = nullptr)
    {
        key = k;
        left = l;
        right = r;
    }
};

class persistentBST {
    private:
        shared_ptr<Node> root;

        shared_ptr<Node> insert(shared_ptr<Node> node, int key)
        {
            if(!node)
                return make_shared<Node>(key);
            
            if(key < node->key)
            {
                return make_shared<Node>(node->key, insert(node->left, key), node->right);
            } else if (key > node->key)
            {
                return make_shared<Node>(node->key, node->left, insert(node->right, key));
            } else {
                return node;
            }
        }

        bool search(shared_ptr<Node> node, int key){
            if (!node)
                return false;
            if(key == node->key)
                return true;
            if (key < node->key)
                return search (node->left, key);
            return search(node->right, key);
        }

        void inorder(shared_ptr<Node> node)
        {
            if(!node)
                return;
            inorder(node->left);
            cout << node->key << " ";
            inorder(node->right);
        }

    public:
        persistentBST(){
            root = nullptr;
        }
        persistentBST(shared_ptr<Node> r){
            root = r;
        }

        persistentBST insert(int key)
        {
            return persistentBST(insert(root, key));
        }

        shared_ptr<Node> remove(shared_ptr<Node> node, int key)
        {
            if (!node)
                return nullptr;
            if (key < node->key)
                return make_shared<Node>(node->key, remove(node->left, key), node->right);
            if (key > node->key)
                return make_shared<Node>(node->key, node->left, remove(node->right, key));

            if (!node->left)
                return node->right;
            if (!node->right)
                return node->left;

            shared_ptr<Node> succ = node->right;
            while (succ->left)
                succ = succ->left;
            return make_shared<Node>(succ->key, node->left, remove(node->right, succ->key));
        }

        persistentBST remove(int key)
        {
            return persistentBST(remove(root, key));
        }

        bool search(int key)
        {
            return search(root, key);
        }

        void inorder()
        {
            inorder(root);
            cout<<endl;
        }
};

int main()
{
    persistentBST t1;

    auto t2 = t1.insert(10);
    cout << "t1 (empty): "; t1.inorder();
    cout << "t2 (after insert 10): "; t2.inorder();

    auto t3 = t2.insert(20);
    cout << "t2 (unchanged): "; t2.inorder();
    cout << "t3 (t2 + 20): "; t3.inorder();

    auto t4 = t3.insert(30);
    cout << "t3 (unchanged): "; t3.inorder();
    cout << "t4 (t3 + 30): "; t4.inorder();

    auto t5 = t4.insert(40);
    cout << "t4 (unchanged): "; t4.inorder();
    cout << "t5 (t4 + 40): "; t5.inorder();

    cout << "\nAll versions:\n";
    cout << "t1: "; t1.inorder();
    cout << "t2: "; t2.inorder();
    cout << "t3: "; t3.inorder();
    cout << "t4: "; t4.inorder();
    cout << "t5: "; t5.inorder();

    cout << "\nt3 contains 20? " << (t3.search(20) ? "yes" : "no") << endl;
    cout << "t2 contains 20? " << (t2.search(20) ? "yes" : "no") << endl;
    cout << "t5 contains 30? " << (t5.search(30) ? "yes" : "no") << endl;

    return 0;
}