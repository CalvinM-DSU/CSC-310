#include "hash.h"

// Constructor - initializing table based on chosen hash table variant
HashTable::HashTable(int size, CollisionHandling variant) 
{
    tableSize = size; 
    elementCount = 0; 
    method = variant;
    switch (method) {
        case CHAINING_VECTOR:
            tableVector.resize(size);
            break;
        case CHAINING_LIST:
            tableList.resize(size);
            break;
        case CHAINING_BST:
            tableBST.resize(size);
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            tableProbing.resize(size);
            break;
    }
}

HashTable::~HashTable() {}

int HashTable::hash1(const string& key) const {
    unsigned long hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c);
    }
    return hash % tableSize;
}

int HashTable::hash2(const string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return 1 + (hash % (tableSize - 1));
}

int HashTable::probe(int index, int i) const {
    switch (method) {
        case LINEAR_PROBING:
            return (index + i) % tableSize;

        case QUADRATIC_PROBING:
            return (index + i * i) % tableSize;

        case DOUBLE_HASHING:
            return (index + i * hash2("")) % tableSize; 

        default:
            return index;
    }
}
void HashTable::insert(const string& key, int value) {
    resizeIfNeeded();
    int index = hash1(key);

    switch (method) {
        case CHAINING_LIST:
            for (auto& p : tableList[index]) {
                if (p.first == key) {
                    p.second = value;
                    return;
                }
            }
            tableList[index].push_back({key, value});
            break;

        case CHAINING_BST:
            tableBST[index].insert(key, value);
            break;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int slot = findEmptySlot(key);
            tableProbing[slot] = {key, value};
            break;
        }

        default:
            break;
    }

    elementCount++;
}

bool HashTable::search(const string& key, int& value) {
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR:
            for (auto& p : tableVector[index]) {
                if (p.first == key) {
                    value = p.second;
                    return true;
                }
            }
            return false;

        case CHAINING_LIST:
            for (auto& p : tableList[index]) {
                if (p.first == key) {
                    value = p.second;
                    return true;
                }
            }
            return false;

        case CHAINING_BST:
            return tableBST[index].search(key, value);

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int i = 0;
            int current;

            while (i < tableSize) {
                if (method == DOUBLE_HASHING)
                    current = (index + i * hash2(key)) % tableSize;
                else
                    current = probe(index, i);

                if (tableProbing[current].first == "")
                    return false;

                if (tableProbing[current].first == key) {
                    value = tableProbing[current].second;
                    return true;
                }
                i++;
            }
            return false;
        }
    }
    return false;
}

bool HashTable::remove(const string& key) {
    int index = hash1(key);

    switch (method) {
        case CHAINING_VECTOR:
            for (auto it = tableVector[index].begin(); it != tableVector[index].end(); ++it) {
                if (it->first == key) {
                    tableVector[index].erase(it);
                    elementCount--;
                    return true;
                }
            }
            return false;

        case CHAINING_LIST:
            for (auto it = tableList[index].begin(); it != tableList[index].end(); ++it) {
                if (it->first == key) {
                    tableList[index].erase(it);
                    elementCount--;
                    return true;
                }
            }
            return false;

        case CHAINING_BST:
            if (tableBST[index].remove(key)) {
                elementCount--;
                return true;
            }
            return false;

        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING: {
            int i = 0, current;

            while (i < tableSize) {
                if (method == DOUBLE_HASHING)
                    current = (index + i * hash2(key)) % tableSize;
                else
                    current = probe(index, i);

                if (tableProbing[current].first == "")
                    return false;

                if (tableProbing[current].first == key) {
                    tableProbing[current].first = ""; // this may cause issues with bigger datasets, but it works for this
                    elementCount--;
                    return true;
                }
                i++;
            }
            return false;
        }
    }
    return false;
}

// Function to read data from file
vector<pair<string, int>> readDataFromFile(const string& filename) {
    vector<pair<string, int>> data;
    ifstream file(filename);
    string key;
    int value;
    if (!file) {
        cerr << "Error reading file: " << filename << endl;
        return data;
    }
    while (file >> key >> value) {
        data.push_back({key, value});
    }

    file.close();
    return data;
}

// Benchmark function for custom HashTable
void HashTable::benchmarkHashTable(HashTable& table, const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto& pair : data) {
        table.insert(pair.first, pair.second);
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i) {
        table.search(data[rand() % data.size()].first, value);
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i) {
        table.remove(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}

void HashTable::displayStats() {
    int totalElements = 0;
    int maxChainLength = 0;
    int emptyBuckets = 0;

    switch (method) {
        case CHAINING_VECTOR:
            for (const auto& bucket : tableVector) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;
        case CHAINING_LIST:
            for (const auto& bucket : tableList) {
                if (bucket.empty()) emptyBuckets++;
                maxChainLength = max(maxChainLength, (int)bucket.size());
                totalElements += bucket.size();
            }
            break;
        case CHAINING_BST:
            for (const auto& bucket : tableBST) {
                vector<pair<string, int>> elements = bucket.inOrderTraversal();
                if (elements.empty()) {
                    emptyBuckets++;
                } else {
                    maxChainLength = max(maxChainLength, (int)elements.size());
                    totalElements += elements.size();
                }
            }
            break;
        case LINEAR_PROBING:
        case QUADRATIC_PROBING:
        case DOUBLE_HASHING:
            for (const auto& entry : tableProbing) {
                if (!entry.first.empty()) totalElements++;
                else emptyBuckets++;
            }
            break;
    }

    cout << "Total elements: " << totalElements << endl;
    cout << "Load factor: " << (double)totalElements / tableSize << endl;
    cout << "Empty buckets: " << emptyBuckets << endl;
    if (method == CHAINING_VECTOR || method == CHAINING_LIST || method == CHAINING_BST) {
        cout << "Max chain length: " << maxChainLength << endl;
    }
}


void HashTable::rehash() {
    int oldSize = tableSize;
    tableSize *= 2;

    vector<pair<string, int>> oldData;

    // get all the old stuff
    switch (method) {
        case CHAINING_VECTOR:
            for (auto& bucket : tableVector)
                for (auto& p : bucket)
                    oldData.push_back(p);
            tableVector.clear();
            tableVector.resize(tableSize);
            break;

        case CHAINING_LIST:
            for (auto& bucket : tableList)
                for (auto& p : bucket)
                    oldData.push_back(p);
            tableList.clear();
            tableList.resize(tableSize);
            break;

        case CHAINING_BST:
            for (auto& tree : tableBST) {
                auto elems = tree.inOrderTraversal();
                oldData.insert(oldData.end(), elems.begin(), elems.end());
            }
            tableBST.clear();
            tableBST.resize(tableSize);
            break;

        default:
            for (auto& p : tableProbing)
                if (!p.first.empty())
                    oldData.push_back(p);
            tableProbing.clear();
            tableProbing.resize(tableSize);
            break;
    }

    elementCount = 0;

    // reinsert the old stuff
    for (auto& p : oldData)
        insert(p.first, p.second);
}


int HashTable::findEmptySlot(const string& key) {
    int index = hash1(key);
    int i = 0;

    while (i < tableSize) {
        int current;

        if (method == DOUBLE_HASHING)
            current = (index + i * hash2(key)) % tableSize;
        else
            current = probe(index, i);

        if (tableProbing[current].first == "" || tableProbing[current].first == key)
            return current;

        i++;
    }

    return -1; // return error code if fails
}

// try experimenting with different thresholds for each technique
void HashTable::resizeIfNeeded() {
    double loadFactor = (double)elementCount / tableSize;

    // thresholds
    // lower for probing, higher for chaining
    if ((method == LINEAR_PROBING || method == QUADRATIC_PROBING || method == DOUBLE_HASHING) && loadFactor > 0.6)
        rehash();
    else if ((method == CHAINING_VECTOR || method == CHAINING_LIST || method == CHAINING_BST) && loadFactor > 1.5)
        rehash();
}

// Benchmark function for built-in hash table in C++
void benchmarkStdUnorderedMap(const vector<pair<string, int>>& data, int numSearch, int numDelete) {
    unordered_map<string, int> stdMap;

    auto start = chrono::high_resolution_clock::now();

    // Insertions
    for (const auto& pair : data) {
        stdMap[pair.first] = pair.second;
    }

    auto insertEnd = chrono::high_resolution_clock::now();

    // Searches
    int value;
    for (int i = 0; i < numSearch; ++i) {
        auto it = stdMap.find(data[rand() % data.size()].first);
        if (it != stdMap.end()) {
            value = it->second;
        }
    }

    auto searchEnd = chrono::high_resolution_clock::now();

    // Deletions
    for (int i = 0; i < numDelete; ++i) {
        stdMap.erase(data[rand() % data.size()].first);
    }

    auto end = chrono::high_resolution_clock::now();

    auto insertTime = chrono::duration_cast<chrono::microseconds>(insertEnd - start);
    auto searchTime = chrono::duration_cast<chrono::microseconds>(searchEnd - insertEnd);
    auto deleteTime = chrono::duration_cast<chrono::microseconds>(end - searchEnd);

    cout << "std::unordered_map Benchmark Results:" << endl;
    cout << "Insertion time: " << insertTime.count() << " microseconds" << endl;
    cout << "Search time: " << searchTime.count() << " microseconds" << endl;
    cout << "Deletion time: " << deleteTime.count() << " microseconds" << endl;
}