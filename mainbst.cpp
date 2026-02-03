#include <iostream>
#include <fstream>
using namespace std;

#include "bst.h"

// command line argument: name of command file
// see assignment for command file guidelines
int main(int argc, char *argv[])
{
        bstree t;
        bool r;

        // process a command file

    int cmd, val;

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <commandfile>" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin) {
        cout << "Error: could not open file " << argv[1] << endl;
        return 1;
    }

    while (fin >> cmd) {

        if (cmd == 0) {
            cout << "Quit" << endl;
            break;
        }

        else if (cmd == 1) {
            fin >> val;
            t.insert(val);
            cout << "Inserted " << val << endl;
        }

        else if (cmd == -1) {
            fin >> val;
                                t.deleter(val);
            cout << "Delete " << val << endl;
        }

        else if (cmd == 2) {
            fin >> val;
            r = t.search(val);
            cout << "Search " << val << ": ";
            if (r)
                cout << "Yes" << endl;
            else
                cout << "No" << endl;
        }

        else if (cmd == 3) {
            cout << "Pre: ";
            t.pre();
            cout << endl;
        }

        else if (cmd == 4) {
            cout << "In: ";
            t.in();
            cout << endl;
        }

        else if (cmd == 5) {
            cout << "Post: ";
            t.post();
            cout << endl;
        }

        else if (cmd == 6) {
            if (t.isWorstCase())
                cout << "Tree is worst case" << endl;
            else
                cout << "Tree is not worst case" << endl;
        }

        else if (cmd == 7) {
            cout << "numnodes: " << t.numnodes() << endl;
        }

        else if (cmd == 8) {
            cout << "Height: " << t.height() << endl;
        }

        else if (cmd == 9) {
                                t.compareLR();
        }

        else {
            cout << "Unknown command: " << cmd << endl;
        }
    }

        return 0;
}