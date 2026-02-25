#include "Includes/splay.h"

int main() {
    SplayTree tree;

    tree.insert(57);
    tree.insert(31);
    tree.insert(72);
    tree.insert(44);
    tree.insert(69);
    tree.insert(83);

    cout << "Tree after insertions:\n";
    tree.display();

    tree.search(44);
    cout << "Tree after splaying 40:\n";
    tree.display();

    tree.remove(31);
    cout << "Tree after deleting 30:\n";
    tree.display();

    tree.semiSplay(83, 3);
    cout << "Tree after semisplay:\n";
    tree.display();

    tree.weightedSplay(72);
    cout << "Tree after weighted splay:\n";
    tree.display();

    return 0;
}
