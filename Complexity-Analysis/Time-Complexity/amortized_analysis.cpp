// Amortized analysis
//
// Amortized analysis averages the cost of a sequence of operations, even when
// a single operation is expensive. It is not the same as the average case:
// there is no probability involved, and the bound holds for every sequence.
//
// Dynamic array example:
//   - push_back is O(1) when capacity is available
//   - when the array is full, it allocates double the capacity and copies
//     every element -> O(n) for that one push
//   - over n pushes, the total copying is 1 + 2 + 4 + ... + n < 2n
//   - so the amortized cost per push is O(1)
//
// If the capacity grew by a constant instead of doubling, the total copy work
// would be O(n^2), so the growth factor matters.

#include <iostream>
#include <vector>
using namespace std;

class GrowingArray {
    int *data;
    int length;
    int capacity;
    long long totalCopies;

    void grow() {
        int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        int *newData = new int[newCapacity];

        for (int i = 0; i < length; i++) {
            newData[i] = data[i];
            totalCopies++;
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

   public:
    GrowingArray() {
        data = nullptr;
        length = 0;
        capacity = 0;
        totalCopies = 0;
    }

    ~GrowingArray() {
        delete[] data;
    }

    void push_back(int value) {
        if (length == capacity) {
            grow();
        }

        data[length] = value;
        length++;
    }

    int size() const {
        return length;
    }

    int currentCapacity() const {
        return capacity;
    }

    long long copies() const {
        return totalCopies;
    }
};

int main() {
    GrowingArray array;

    for (int i = 0; i < 1000; i++) {
        array.push_back(i);
    }

    cout << array.size() << endl;             // 1000
    cout << array.currentCapacity() << endl;  // 1024
    cout << array.copies() << endl;           // 1023 (< 2 * 1000)

    cout << (double)array.copies() / array.size() << endl;  // about 1

    // The same behavior is visible in std::vector::capacity().
    vector<int> dynamic;

    for (int i = 0; i < 1000; i++) {
        dynamic.push_back(i);
    }

    cout << dynamic.capacity()
         << endl;  // usually 1024 (implementation-defined)

    // A constant-growth array would copy 0 + 1 + 2 + ... + (n - 1) = O(n^2)
    // times in total, which is why real implementations multiply the capacity.
    return 0;
}

// Time complexity:
//   push_back -> O(1) amortized, O(n) worst case for one call
//   n pushes -> O(n) total
// Space complexity -> O(n) for the elements, and up to 2x that during a grow
//
// Special cases: the first push (capacity 0), a grow at exactly a power of
// two, `reserve()` to avoid reallocation, `shrink_to_fit()` (a non-binding
// request), and repeated push/pop that never triggers a grow.
//
// Amortized O(1) also applies to:
//   - hash table insert with rehashing
//   - union-find with path compression and union by rank (inverse Ackermann)
//   - a stack with a multipop operation
//   - a binary counter increment
