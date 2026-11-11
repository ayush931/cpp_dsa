/**
 * Amortized analysis -> average cost per operation over a sequence of
 * operations It is not the same as average case analysis
 *
 * Aggregate method -> total cost of n operations divided by n
 * Accounting method -> each operation is charged a credit for future expensive
 * operations Potential method -> store credit as a potential function
 *
 * Example: dynamic array push_back
 * Most insertions cost O(1), but when the array is full it doubles and copies
 * O(n) Total cost of n insertions -> O(n), so amortized cost per insertion ->
 * O(1)
 */

#include <iostream>
using namespace std;

class DynamicArray {
    int *arr;
    int size;
    int capacity;
    long long totalCost;

   public:
    DynamicArray() {
        capacity = 1;
        size = 0;
        arr = new int[capacity];
        totalCost = 0;
    }

    ~DynamicArray() {
        delete[] arr;
    }

    void push_back(int value) {
        if (size == capacity) {
            int newCapacity = capacity * 2;
            int *newArr = new int[newCapacity];

            for (int i = 0; i < size; i++) {
                newArr[i] = arr[i];
                totalCost++;  // cost of copying
            }

            delete[] arr;
            arr = newArr;
            capacity = newCapacity;
        }

        arr[size] = value;
        size++;
        totalCost++;  // cost of writing the new element
    }

    long long getTotalCost() {
        return totalCost;
    }

    int getSize() {
        return size;
    }
};

int main() {
    DynamicArray arr;

    for (int i = 0; i < 16; i++) {
        arr.push_back(i);
    }

    cout << "Total cost: " << arr.getTotalCost() << endl;  // 31
    cout << "Amortized cost: " << (double)arr.getTotalCost() / arr.getSize()
         << endl;

    return 0;
}

// Amortized O(1) does not mean every operation is O(1)
// Worst case for a single push_back is still O(n)
// DSU with path compression and union by rank -> O(alpha(n)) amortized per
// operation
