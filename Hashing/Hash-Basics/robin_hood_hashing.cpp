/**
 * Robin Hood hashing
 *
 * An open-addressing hash table with linear probing that keeps probe distances
 * balanced. When an incoming key has traveled farther than the key already in
 * a slot, the two keys swap: the "rich" key (short distance) gives its slot to
 * the "poor" key (long distance). This shortens the worst-case probe length.
 *
 * Search can stop early when the current slot's distance is smaller than the
 * distance already traveled, because no later slot can contain the key.
 *
 * Insert/search/remove -> O(1) average, O(n) worst
 * Space -> O(capacity)
 *
 * Example:
 * insert 18, 23, 33, 43, 53, 63 into a table with 8 slots
 * contains(33) -> true
 * remove(33)
 * contains(33) -> false
 */

#include <iostream>
#include <vector>
using namespace std;

class RobinHoodHashSet {
    struct Slot {
        int key;
        int distance;  // distance from the key's home slot
        bool occupied;
    };

    vector<Slot> table;
    int elementCount;

    int hash(int key) const {
        int index = key % (int)table.size();
        return index < 0 ? index + (int)table.size() : index;
    }

    void rehash() {
        vector<int> keys;

        for (const Slot &slot : table) {
            if (slot.occupied) {
                keys.push_back(slot.key);
            }
        }

        int newCapacity = (int)table.size() * 2;
        table.assign(newCapacity, Slot{0, 0, false});
        elementCount = 0;

        for (int key : keys) {
            insertNoRehash(key);
        }
    }

    bool insertNoRehash(int key) {
        int capacity = table.size();
        int index = hash(key);
        int distance = 0;

        while (table[index].occupied) {
            // Steal the slot when the incoming key is poorer than the occupant.
            if (table[index].distance < distance) {
                swap(key, table[index].key);
                swap(distance, table[index].distance);
            }

            index = (index + 1) % capacity;
            distance++;
        }

        table[index] = Slot{key, distance, true};
        elementCount++;
        return true;
    }

   public:
    RobinHoodHashSet(int requestedCapacity = 8) {
        int capacity = 8;

        while (capacity < requestedCapacity) {
            capacity *= 2;
        }

        table.assign(capacity, Slot{0, 0, false});
        elementCount = 0;
    }

    bool insert(int key) {
        if (contains(key)) {
            return false;
        }

        if ((elementCount + 1) * 10 > (int)table.size() * 7) {
            rehash();
        }

        return insertNoRehash(key);
    }

    bool contains(int key) const {
        int capacity = table.size();
        int index = hash(key);
        int distance = 0;

        while (table[index].occupied) {
            if (table[index].key == key) {
                return true;
            }

            // Robin Hood invariant: the key cannot appear farther than this.
            if (table[index].distance < distance) {
                return false;
            }

            index = (index + 1) % capacity;
            distance++;
        }

        return false;
    }

    bool remove(int key) {
        int capacity = table.size();
        int index = hash(key);
        int distance = 0;

        while (table[index].occupied) {
            if (table[index].key == key) {
                break;
            }

            if (table[index].distance < distance) {
                return false;
            }

            index = (index + 1) % capacity;
            distance++;
        }

        if (!table[index].occupied) {
            return false;
        }

        // Backward-shift deletion: pull later keys back instead of using a
        // tombstone.
        int current = index;
        int next = (current + 1) % capacity;

        while (table[next].occupied && table[next].distance > 0) {
            table[current] = table[next];
            table[current].distance--;
            current = next;
            next = (next + 1) % capacity;
        }

        table[current].occupied = false;
        table[current].distance = 0;
        elementCount--;
        return true;
    }

    int size() const {
        return elementCount;
    }

    int capacity() const {
        return table.size();
    }

    void print() const {
        cout << "[";

        for (int i = 0; i < (int)table.size(); i++) {
            if (!table[i].occupied) {
                continue;
            }

            if (i > 0) {
                cout << ", ";
            }
            cout << i << ":" << table[i].key << "(d" << table[i].distance
                 << ")";
        }

        cout << "]" << endl;
    }
};

int main() {
    RobinHoodHashSet set(8);

    for (int value : {18, 23, 33, 43, 53, 63}) {
        set.insert(value);
    }

    cout << set.size() << endl;         // 6
    cout << set.capacity() << endl;     // 16 (rehashed at load 0.7)
    cout << set.contains(33) << endl;   // 1
    cout << set.contains(100) << endl;  // 0

    set.remove(33);
    cout << set.contains(33) << endl;  // 0
    cout << set.size() << endl;        // 5

    set.insert(73);
    cout << set.contains(73) << endl;  // 1
    cout << set.size() << endl;        // 6

    set.print();  // slot:key(distance) in slot order

    return 0;
}

/**
 * Special cases: duplicate insertion (returns false), removing a missing key,
 * removing the first key in a cluster (backward shift), a full table
 * (rehash happens before the next insert), negative keys, and keys whose home
 * slots are far apart.
 *
 * The printout is deterministic: it always walks slots from index 0 upward.
 */
