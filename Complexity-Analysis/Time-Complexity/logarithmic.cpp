// Logarithmic time -> O(log n)
//
// The input is repeatedly cut by a constant factor, usually 2. Binary search
// is the standard example: each step discards half of the remaining range.
//
// n = 1,000,000 needs only about 20 comparisons, because 2^20 > 1,000,000.
// Doubling n adds just one more step.

#include <iostream>
#include <vector>
using namespace std;

// Returns the index of target, or -1. The array must be sorted.
int binarySearch(vector<int> &values, int target, int &steps) {
    int left = 0;
    int right = values.size() - 1;
    steps = 0;

    while (left <= right) {
        steps++;

        int mid = left + (right - left) / 2;  // avoids overflow

        if (values[mid] == target) {
            return mid;
        }

        if (values[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

// A loop that halves its counter is also O(log n).
int countHalvingSteps(int n) {
    int steps = 0;

    while (n > 1) {
        n /= 2;
        steps++;
    }

    return steps;
}

// Multiplying until n is reached is O(log n) as well.
int countDoublingSteps(int n) {
    int steps = 0;

    for (int value = 1; value < n; value *= 2) {
        steps++;
    }

    return steps;
}

int main() {
    vector<int> values;

    for (int i = 0; i < 1000000; i++) {
        values.push_back(i);
    }

    int steps;
    int index = binarySearch(values, 999999, steps);

    cout << index << endl;  // 999999
    cout << steps << endl;  // 20 (log2(1000000) is about 19.93)

    cout << countHalvingSteps(1000000) << endl;   // 19
    cout << countDoublingSteps(1000000) << endl;  // 20

    // Growing n by a factor of 2 adds exactly one step.
    for (int n = 8; n <= 1024; n *= 2) {
        cout << n << " -> " << countHalvingSteps(n) << " steps" << endl;
    }

    return 0;
}

// Time complexity -> O(log n), because the problem size is divided by 2 each
// iteration. The base of the logarithm does not matter asymptotically.
// Space complexity -> O(1) for the iterative version, O(log n) for the
// recursive version because of the call stack.
//
// Special cases: an empty array, one element, the target at the first or last
// position, the target missing, duplicate values (which occurrence is found
// depends on the comparison), an overflow-safe midpoint.
//
// O(log n) is much better than O(n) for large inputs, but it requires a sorted
// array (or another monotonic property, such as binary search on the answer).
