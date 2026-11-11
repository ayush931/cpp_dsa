// Best case, worst case, and average case
//
// The same algorithm can take different amounts of time on different inputs of
// the same size.
//   - best case: the minimum work over all inputs of size n
//   - worst case: the maximum work over all inputs of size n
//   - average case: the expected work over a distribution of inputs
//
// Big-O usually describes the worst case unless stated otherwise. For linear
// search:
//   - best -> O(1)   (the target is the first element)
//   - worst -> O(n)  (the target is last or absent)
//   - average -> O(n) (about (n + 1) / 2 comparisons)

#include <iostream>
#include <vector>
using namespace std;

int linearSearch(vector<int> &values, int target, int &comparisons) {
    comparisons = 0;

    for (int i = 0; i < values.size(); i++) {
        comparisons++;

        if (values[i] == target) {
            return i;
        }
    }

    return -1;
}

int main() {
    vector<int> values;

    for (int i = 1; i <= 100; i++) {
        values.push_back(i);
    }

    int comparisons;

    linearSearch(values, 1, comparisons);
    cout << "best: " << comparisons << " comparison" << endl;  // 1

    linearSearch(values, 100, comparisons);
    cout << "worst: " << comparisons << " comparisons" << endl;  // 100

    linearSearch(values, 101, comparisons);
    cout << "absent: " << comparisons << " comparisons" << endl;  // 100

    // Average over every possible target position.
    long long total = 0;

    for (int target = 1; target <= 100; target++) {
        linearSearch(values, target, comparisons);
        total += comparisons;
    }

    cout << "average: " << (double)total / 100 << " comparisons" << endl;
    // 50.5 -> (n + 1) / 2

    // Quick sort is another example: average O(n log n), worst O(n^2) when the
    // pivot is always the smallest or largest element. Randomized pivots make
    // the worst case very unlikely.
    return 0;
}

// Time complexity:
//   linear search -> best O(1), worst O(n), average O(n)
//   binary search -> best O(1), worst O(log n), average O(log n)
//   quick sort -> best O(n log n), average O(n log n), worst O(n^2)
//   hash table lookup -> average O(1), worst O(n) with many collisions
//
// Space complexity is also analyzed by case: the worst case may use more
// stack or auxiliary memory than the best case.
//
// Special cases: an empty input, one element, the target at the first or last
// position, the target absent, duplicate values, adversarial input designed to
// trigger the worst case.
//
// When you state a complexity, say which case you mean. "O(n)" alone is
// usually understood as worst case, but hash tables and quick sort are
// normally described by their average case.
