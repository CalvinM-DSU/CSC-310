#include "Includes/binomial.h"

int main() {
    BinomialHeap bh;
    bh.insert(10);
    bh.insert(20);
    bh.insert(5);
    bh.insert(30);
    bh.insert(1);
    bh.insert(14);
    bh.insert(27);
    bh.insert(8);
    bh.insert(3);
    bh.insert(12);
    
    cout << "Heap after insertions:" << endl;
    bh.printHeap();
    
    cout << endl << "Minimum key: " << bh.findMin() << endl;

    cout << "Heap after deleting minimum:" << endl;
    bh.deleteMin();
    bh.printHeap();

    return 0;
}