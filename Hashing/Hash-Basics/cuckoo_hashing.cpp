/**
 * Cuckoo hashing
 *
 * A key has two possible locations, one in each of two tables. Insertion puts
 * the key in its first choice; if that slot is occupied, the old key is
 * evicted and moved to its other table, and so on. A cycle of evictions means
 * the table needs a larger capacity or new hash seeds.
 *
 * Lookup checks only the two candidate slots -> O(1) worst case.
 * Insert may perform several kicks -> O(1) average, O(n) during a rebuild.
 * Space -> O(capacity)
 *
 * Example:
 * insert 1..10
 * contains(7) -> true
 * remove(7)
 * contains(7) -> false
 */

#include <iostream>
#include <vector>
using namespace std;

class CuckooHashSet {
    vector<int> table1;
    vector<int> table2;
    vector<bool> used1;
    vector<bool> used2;
    int elementCount;
    unsigned seed1;
    unsigned seed2;

    static unsigned mix(unsigned value) {
        value ^= value >> 16;
        value *= 0x7feb352dU;
        value ^= value >> 15;
        value *= 0x846ca68bU;
        value ^= value >> 16;
        return value;
    }

    int h1(int key) const {
        unsigned mask = (unsigned)table1.size() - 1;
        return (int)(mix((unsigned)key ^ seed1) & mask);
    }

    int h2(int key) const {
        unsigned mask = (unsigned)table1.size() - 1;
        int index = (int)(mix((unsigned)key ^ seed2) & mask);

        // The two choices should be different whenever possible.
        if (index == h1(key)) {
            index = (index + 1) & (int)mask;
        }

        return index;
    }

    bool place(int &key, int maxKicks) {
        for (int kick = 0; kick < maxKicks; kick++) {
            int first = h1(key);

            if (!used1[first]) {
                used1[first] = true;
                table1[first] = key;
                return true;
            }

            swap(key, table1[first]);

            int second = h2(key);

            if (!used2[second]) {
                used2[second] = true;
                table2[second] = key;
                return true;
            }

            swap(key, table2[second]);
        }

        return false;
    }

    vector<int> allKeys() const {
        vector<int> keys;

        for (int i = 0; i < (int)table1.size(); i++) {
            if (used1[i]) {
                keys.push_back(table1[i]);
            }
        }

        for (int i = 0; i < (int)table2.size(); i++) {
            if (used2[i]) {
                keys.push_back(table2[i]);
            }
        }

        return keys;
    }

    bool rebuild(int newSize, const vector<int> &keys) {
        if (newSize > (1 << 20)) {
            return false;
        }

        table1.assign(newSize, 0);
        table2.assign(newSize, 0);
        used1.assign(newSize, false);
        used2.assign(newSize, false);
        elementCount = 0;

        // New seeds make it unlikely that the same eviction cycle repeats.
        seed1 += 0x9e3779b9U;
        seed2 += 0x85ebca6bU;

        for (int key : keys) {
            int pending = key;

            if (!place(pending, 2 * newSize)) {
                return false;
            }

            elementCount++;
        }

        return true;
    }

   public:
    CuckooHashSet(int requestedCapacity = 8) {
        int capacity = 8;

        while (capacity < requestedCapacity) {
            capacity *= 2;
        }

        table1.assign(capacity, 0);
        table2.assign(capacity, 0);
        used1.assign(capacity, false);
        used2.assign(capacity, false);
        elementCount = 0;
        seed1 = 0x12345678U;
        seed2 = 0x9abcdef0U;
    }

    bool insert(int key) {
        if (contains(key)) {
            return false;
        }

        // Keep the load low enough for cuckoo eviction to succeed quickly.
        if ((elementCount + 1) * 10 > (int)table1.size() * 4) {
            vector<int> keys = allKeys();
            int newSize = (int)table1.size() * 2;
            bool rebuilt = false;

            while (newSize <= (1 << 20)) {
                if (rebuild(newSize, keys)) {
                    rebuilt = true;
                    break;
                }

                newSize *= 2;
            }

            if (!rebuilt) {
                return false;
            }
        }

        int pending = key;

        if (place(pending, 2 * (int)table1.size())) {
            elementCount++;
            return true;
        }

        // A cycle occurred. Rebuild with every key, including the evicted one.
        vector<int> keys = allKeys();
        keys.push_back(pending);

        int newSize = (int)table1.size() * 2;

        while (newSize <= (1 << 20)) {
            if (rebuild(newSize, keys)) {
                return true;
            }
            newSize *= 2;
        }

        return false;
    }

    bool contains(int key) const {
        int first = h1(key);
        int second = h2(key);

        return (used1[first] && table1[first] == key) ||
               (used2[second] && table2[second] == key);
    }

    bool remove(int key) {
        int first = h1(key);

        if (used1[first] && table1[first] == key) {
            used1[first] = false;
            elementCount--;
            return true;
        }

        int second = h2(key);

        if (used2[second] && table2[second] == key) {
            used2[second] = false;
            elementCount--;
            return true;
        }

        return false;
    }

    int size() const {
        return elementCount;
    }

    int capacity() const {
        return table1.size();
    }

    double loadFactor() const {
        return (double)elementCount / (double)(2 * table1.size());
    }

    void print() const {
        cout << "table1: [";

        for (int i = 0; i < (int)table1.size(); i++) {
            if (!used1[i]) {
                continue;
            }

            if (i > 0) {
                cout << ", ";
            }
            cout << i << ":" << table1[i];
        }

        cout << "]" << endl;

        cout << "table2: [";

        for (int i = 0; i < (int)table2.size(); i++) {
            if (!used2[i]) {
                continue;
            }

            if (i > 0) {
                cout << ", ";
            }
            cout << i << ":" << table2[i];
        }

        cout << "]" << endl;
    }
};

int main() {
    CuckooHashSet set(8);

    for (int value = 1; value <= 10; value++) {
        set.insert(value);
    }

    cout << set.size() << endl;         // 10
    cout << set.capacity() << endl;     // 32 (two rebuilds)
    cout << set.contains(7) << endl;    // 1
    cout << set.contains(100) << endl;  // 0

    set.remove(7);
    cout << set.contains(7) << endl;  // 0
    cout << set.size() << endl;       // 9

    set.insert(11);
    cout << set.contains(11) << endl;  // 1
    cout << set.size() << endl;        // 10

    set.print();  // deterministic slot layout

    return 0;
}

/**
 * Special cases: duplicate insertion (returns false), a missing key, removing
 * from either table, an eviction cycle that forces a rebuild, and negative
 * keys (mixed through unsigned arithmetic).
 *
 * Cuckoo hashing gives O(1) worst-case lookup because a key has only two
 * possible slots; insertion is amortized O(1) but can be expensive when a
 * rebuild is required.
 */
