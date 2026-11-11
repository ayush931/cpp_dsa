/**
 * Open addressing hash table
 *
 * A hash table that stores every key inside the table itself. Collisions are
 * resolved by probing:
 *   LINEAR       -> h(k), h(k)+1, h(k)+2, ...
 *   QUADRATIC    -> h(k), h(k)+1, h(k)+3, h(k)+6, ... (triangular steps)
 *   DOUBLE_HASH  -> h(k), h(k)+step, h(k)+2*step, ... with an odd step
 *
 * The capacity is a power of two. Triangular steps and odd double-hash steps
 * visit every slot, so a free slot is always found while the table is not
 * full. Deletions use tombstones; the table rehashes when the combined live
 * and deleted load exceeds 0.7.
 *
 * Average time -> O(1) per insert/search/remove
 * Worst time   -> O(n) when many keys collide
 * Space        -> O(capacity)
 *
 * Example:
 * insert 5, 15, 25, 35, 45 into a table with 8 slots
 * contains(25) -> true
 * remove(25)
 * contains(25) -> false
 */

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum class ProbeType { LINEAR, QUADRATIC, DOUBLE_HASH };

class OpenAddressingHashTable {
    enum SlotState { EMPTY, OCCUPIED, DELETED };

    vector<int> keys;
    vector<SlotState> states;
    int elementCount;
    int deletedCount;
    ProbeType probeType;

    int hash(int key) const {
        int index = key % (int)keys.size();
        return index < 0 ? index + (int)keys.size() : index;
    }

    int probeStep(int key, int attempt) const {
        int capacity = keys.size();

        if (probeType == ProbeType::LINEAR) {
            return 1;
        }

        if (probeType == ProbeType::QUADRATIC) {
            // Cumulative triangular numbers: 1, 3, 6, 10, ...
            return attempt + 1;
        }

        // Odd step, coprime with the power-of-two capacity.
        int half = capacity / 2;
        int odd = 1 + 2 * (int)((unsigned)key % half);
        return odd;
    }

    void rehash() {
        vector<int> oldKeys;

        for (int i = 0; i < (int)keys.size(); i++) {
            if (states[i] == OCCUPIED) {
                oldKeys.push_back(keys[i]);
            }
        }

        int newCapacity = (int)keys.size() * 2;
        keys.assign(newCapacity, 0);
        states.assign(newCapacity, EMPTY);
        elementCount = 0;
        deletedCount = 0;

        for (int key : oldKeys) {
            insertNoRehash(key);
        }
    }

    bool insertNoRehash(int key) {
        int capacity = keys.size();
        int index = hash(key);
        int firstDeleted = -1;

        for (int attempt = 0; attempt < capacity; attempt++) {
            if (states[index] == OCCUPIED && keys[index] == key) {
                return false;
            }

            if (states[index] == DELETED && firstDeleted == -1) {
                firstDeleted = index;
            }

            if (states[index] == EMPTY) {
                int target = (firstDeleted != -1) ? firstDeleted : index;

                if (states[target] == DELETED) {
                    deletedCount--;
                }

                keys[target] = key;
                states[target] = OCCUPIED;
                elementCount++;
                return true;
            }

            index = (index + probeStep(key, attempt)) % capacity;
        }

        // No empty slot was found, but a tombstone can still hold the key.
        if (firstDeleted != -1) {
            keys[firstDeleted] = key;
            states[firstDeleted] = OCCUPIED;
            elementCount++;
            deletedCount--;
            return true;
        }

        return false;
    }

   public:
    OpenAddressingHashTable(int requestedCapacity = 8,
                            ProbeType type = ProbeType::LINEAR) {
        int capacity = 8;

        while (capacity < requestedCapacity) {
            capacity *= 2;
        }

        keys.assign(capacity, 0);
        states.assign(capacity, EMPTY);
        elementCount = 0;
        deletedCount = 0;
        probeType = type;
    }

    bool insert(int key) {
        if (contains(key)) {
            return false;
        }

        // Rehash before the table becomes crowded with live entries or
        // tombstones.
        if ((elementCount + deletedCount + 1) * 10 > (int)keys.size() * 7) {
            rehash();
        }

        return insertNoRehash(key);
    }

    bool contains(int key) const {
        int capacity = keys.size();
        int index = hash(key);

        for (int attempt = 0; attempt < capacity; attempt++) {
            if (states[index] == EMPTY) {
                return false;
            }

            if (states[index] == OCCUPIED && keys[index] == key) {
                return true;
            }

            index = (index + probeStep(key, attempt)) % capacity;
        }

        return false;
    }

    bool remove(int key) {
        int capacity = keys.size();
        int index = hash(key);

        for (int attempt = 0; attempt < capacity; attempt++) {
            if (states[index] == EMPTY) {
                return false;
            }

            if (states[index] == OCCUPIED && keys[index] == key) {
                states[index] = DELETED;
                elementCount--;
                deletedCount++;
                return true;
            }

            index = (index + probeStep(key, attempt)) % capacity;
        }

        return false;
    }

    int size() const {
        return elementCount;
    }

    int capacity() const {
        return keys.size();
    }

    double loadFactor() const {
        return (double)elementCount / (double)keys.size();
    }

    void print() const {
        cout << "[";
        bool firstValue = true;

        for (int i = 0; i < (int)keys.size(); i++) {
            if (states[i] != OCCUPIED) {
                continue;
            }

            if (!firstValue) {
                cout << ", ";
            }
            cout << i << ":" << keys[i];
            firstValue = false;
        }

        cout << "]" << endl;
    }
};

void run(ProbeType type, const string &name) {
    OpenAddressingHashTable table(8, type);

    for (int value : {5, 15, 25, 35, 45}) {
        table.insert(value);
    }

    cout << name << " size=" << table.size() << endl;        // 5
    cout << "contains 25=" << table.contains(25) << endl;    // 1
    cout << "contains 100=" << table.contains(100) << endl;  // 0

    table.remove(25);
    cout << "after remove, contains 25=" << table.contains(25) << endl;  // 0

    table.insert(55);
    table.insert(65);
    table.insert(75);

    cout << "size=" << table.size() << endl;  // 7
    cout << "load=" << fixed << setprecision(2) << table.loadFactor()
         << endl;  // 0.44
    table.print();
    cout << endl;
}

int main() {
    run(ProbeType::LINEAR, "linear");
    run(ProbeType::QUADRATIC, "quadratic");
    run(ProbeType::DOUBLE_HASH, "double");

    return 0;
}

/**
 * Special cases: inserting a duplicate (returns false), removing a missing
 * key, reusing a tombstone, rehashing when the load crosses 0.7, negative
 * keys (normalized with a positive modulus), and a nearly full table.
 *
 * The printed slot order is deterministic for each probe strategy, but the
 * exact slots differ between linear, quadratic, and double hashing.
 */
