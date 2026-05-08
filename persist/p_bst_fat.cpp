// persistent BST using fat node strategy
// partial persistence via modification records

#include <iostream>
#include <vector>

using namespace std;

struct FatNode {
    struct Mod {
        int version;
        FatNode* left;
        FatNode* right;
    };

    int key;
    FatNode* left;
    FatNode* right;
    vector<Mod> mods;

    static const int MAX_MODS = 2;

    FatNode(int k, FatNode* l = nullptr, FatNode* r = nullptr)
        : key(k), left(l), right(r)
    {
        mods.reserve(MAX_MODS);
    }
};

class persistentBST {
private:
    vector<FatNode*> roots;

    FatNode* getLeft(FatNode* node, int version) const {
        if (!node)
            return nullptr;
        for (int i = (int)node->mods.size() - 1; i >= 0; --i) {
            if (node->mods[i].version <= version)
                return node->mods[i].left;
        }
        return node->left;
    }

    FatNode* getRight(FatNode* node, int version) const {
        if (!node)
            return nullptr;
        for (int i = (int)node->mods.size() - 1; i >= 0; --i) {
            if (node->mods[i].version <= version)
                return node->mods[i].right;
        }
        return node->right;
    }

    FatNode* addModification(FatNode* node, int version, FatNode* left, FatNode* right) {
        if (!node)
            return nullptr;

        if ((int)node->mods.size() < FatNode::MAX_MODS) {
            node->mods.push_back({version, left, right});
            return node;
        }

        FatNode* copy = new FatNode(node->key);
        copy->left = left;
        copy->right = right;
        return copy;
    }

    FatNode* insert(FatNode* node, int key, int version) {
        if (!node)
            return new FatNode(key);

        if (key < node->key) {
            FatNode* newLeft = insert(getLeft(node, version - 1), key, version);
            return addModification(node, version, newLeft, getRight(node, version - 1));
        }
        if (key > node->key) {
            FatNode* newRight = insert(getRight(node, version - 1), key, version);
            return addModification(node, version, getLeft(node, version - 1), newRight);
        }

        return node; // ignore duped keys
    }

    FatNode* remove(FatNode* node, int key, int version) {
        if (!node)
            return nullptr;

        if (key < node->key) {
            FatNode* newLeft = remove(getLeft(node, version - 1), key, version);
            return addModification(node, version, newLeft, getRight(node, version - 1));
        }
        if (key > node->key) {
            FatNode* newRight = remove(getRight(node, version - 1), key, version);
            return addModification(node, version, getLeft(node, version - 1), newRight);
        }

        FatNode* leftChild = getLeft(node, version - 1);
        FatNode* rightChild = getRight(node, version - 1);

        if (!leftChild)
            return rightChild;
        if (!rightChild)
            return leftChild;

        FatNode* successor = rightChild;
        while (getLeft(successor, version - 1))
            successor = getLeft(successor, version - 1);

        FatNode* newRight = remove(rightChild, successor->key, version);
        return addModification(node, version, leftChild, newRight);
    }

    bool search(FatNode* node, int key, int version) const {
        if (!node)
            return false;
        if (key == node->key)
            return true;
        if (key < node->key)
            return search(getLeft(node, version), key, version);
        return search(getRight(node, version), key, version);
    }

    void inorder(FatNode* node, int version) const {
        if (!node)
            return;
        inorder(getLeft(node, version), version);
        cout << node->key << " ";
        inorder(getRight(node, version), version);
    }

public:
    persistentBST() {
        roots.push_back(nullptr); // version 0 is empty tree
    }

    int currentVersion() const {
        return (int)roots.size() - 1;
    }

    int insert(int key) {
        int nextVersion = currentVersion() + 1;
        FatNode* newRoot = insert(roots.back(), key, nextVersion);
        roots.push_back(newRoot);
        return nextVersion;
    }

    int remove(int key) {
        int nextVersion = currentVersion() + 1;
        FatNode* newRoot = remove(roots.back(), key, nextVersion);
        roots.push_back(newRoot);
        return nextVersion;
    }

    bool search(int version, int key) const {
        if (version < 0 || version > currentVersion())
            return false;
        return search(roots[version], key, version);
    }

    void inorder(int version) const {
        if (version < 0 || version > currentVersion()) {
            cout << "[invalid version]" << endl;
            return;
        }
        inorder(roots[version], version);
        cout << endl;
    }
};

int main()
{
    persistentBST tree;

    cout << "Initial tree (version 0): "; tree.inorder(0);

    int v1 = tree.insert(10);
    cout << "After insert(10) -> version " << v1 << ": "; tree.inorder(v1);

    int v2 = tree.insert(20);
    cout << "After insert(20) -> version " << v2 << ": "; tree.inorder(v2);

    int v3 = tree.insert(5);
    cout << "After insert(5) -> version " << v3 << ": "; tree.inorder(v3);

    int v4 = tree.insert(15);
    cout << "After insert(15) -> version " << v4 << ": "; tree.inorder(v4);

    int v5 = tree.insert(25);
    cout << "After insert(25) -> version " << v5 << ": "; tree.inorder(v5);

    int v6 = tree.remove(20);
    cout << "After remove(20) -> version " << v6 << ": "; tree.inorder(v6);

    cout << "\nAll versions:\n";
    for (int v = 0; v <= tree.currentVersion(); ++v) {
        cout << "Version " << v << ": ";
        tree.inorder(v);
    }

    cout << "\nVersion 3 contains 20? " << (tree.search(3, 20) ? "yes" : "no") << endl;
    cout << "Version 6 contains 20? " << (tree.search(6, 20) ? "yes" : "no") << endl;
    cout << "Version 5 contains 25? " << (tree.search(5, 25) ? "yes" : "no") << endl;
    cout << "Version 6 contains 25? " << (tree.search(6, 25) ? "yes" : "no") << endl;

    cout << "\nVersion 4 (before remove): "; tree.inorder(4);
    cout << "Version 6 (after remove): "; tree.inorder(6);

    return 0;
}
