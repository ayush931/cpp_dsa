/**
 * Fibonacci search
 *
 * Fibonacci search is a comparison-based search for sorted arrays. It uses
 * Fibonacci numbers to split the array, just as binary search uses halves and
 * ternary search uses thirds. The split points are chosen so that after each
 * comparison the remaining range has a Fibonacci-numbered length.
 *
 * The algorithm never divides, only adds and subtracts, which can be useful on
 * hardware where division is expensive. It also uses only addition and
 * subtraction to compute the next probe.
 *
 * Time        -> O(log n) comparisons
 * Space       -> O(1)
 * Requirement -> the array must be sorted
 *
 * Linear search is included as the brute-force reference. After a hit, the
 * implementation walks left to the first equal value, so duplicates return the
 * first occurrence (that walk is O(n) in the worst case).
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int linearSearch(const vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int fibonacciSearch(const vector<int> &arr, int target) {
    int n = arr.size();
    if (n == 0) {
        return -1;
    }

    // fibMMm2 = F(k - 2), fibMMm1 = F(k - 1), fibM = F(k).
    int fibMMm2 = 0;
    int fibMMm1 = 1;
    int fibM = fibMMm2 + fibMMm1;

    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    int offset = -1;
    int found = -1;

    while (fibM > 1) {
        int index = min(offset + fibMMm2, n - 1);

        if (arr[index] < target) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = index;
        } else if (arr[index] > target) {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            found = index;
            break;
        }
    }

    if (found == -1 && fibMMm1 && offset + 1 < n && arr[offset + 1] == target) {
        found = offset + 1;
    }

    if (found == -1) {
        return -1;
    }

    while (found > 0 && arr[found - 1] == target) {
        found--;
    }

    return found;
}

int main() {
    vector<int> arr = {-12, -5, -5, 0, 2, 4, 9, 13, 18, 27, 35, 44, 58};

    for (int target : {9, -12, 58, 4, -5, 6, -30, 70}) {
        int fibonacci = fibonacciSearch(arr, target);
        int linear = linearSearch(arr, target);

        cout << "target " << target << ": fibonacci = " << fibonacci
             << ", linear = " << linear
             << ", agree = " << (fibonacci == linear ? "yes" : "no") << endl;
    }

    vector<int> empty;
    cout << "empty array: " << fibonacciSearch(empty, 1) << endl;
    cout << "single hit: " << fibonacciSearch(vector<int>{7}, 7) << endl;
    cout << "single miss: " << fibonacciSearch(vector<int>{7}, 3) << endl;

    return 0;
}

// Expected output:
// target 9: fibonacci = 6, linear = 6, agree = yes
// target -12: fibonacci = 0, linear = 0, agree = yes
// target 58: fibonacci = 12, linear = 12, agree = yes
// target 4: fibonacci = 5, linear = 5, agree = yes
// target -5: fibonacci = 1, linear = 1, agree = yes
// target 6: fibonacci = -1, linear = -1, agree = yes
// target -30: fibonacci = -1, linear = -1, agree = yes
// target 70: fibonacci = -1, linear = -1, agree = yes
// empty array: -1
// single hit: 0
// single miss: -1
//
// Special cases: empty array, one element, duplicates, negative values, target
// at the first or last index, target absent, and target outside the range.
//
// Fibonacci search is often compared with binary search: both are O(log n),
// but Fibonacci search needs only addition and subtraction to move its probes.
