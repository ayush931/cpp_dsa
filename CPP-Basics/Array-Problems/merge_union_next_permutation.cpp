// Merge, union, intersection, and next permutation
//
// These four operations all use the same two-pointer idea on sorted data.
//
// 1. Merge two sorted arrays: keep one index per array, always take the
//    smaller front value, then append the remaining tail.
//
//    {1, 3, 5} and {2, 4, 6} -> {1, 2, 3, 4, 5, 6}
//
// 2. Union of two sorted arrays: merge, but write a value only once.
//
//    {1, 2, 2, 4} and {2, 3, 4, 5} -> {1, 2, 3, 4, 5}
//
// 3. Intersection of two sorted arrays: advance the smaller side; when the
//    values are equal, write the value once and advance both sides.
//
//    {1, 2, 2, 4} and {2, 3, 4, 5} -> {2, 4}
//
// 4. Next permutation in lexicographic order:
//    - find the rightmost position where a[i] < a[i + 1] (the pivot)
//    - find the rightmost value greater than the pivot
//    - swap them and reverse the suffix after the pivot
//    - if no pivot exists, the array is the last permutation, so reverse it
//      to wrap around to the first permutation
//
//    {1, 2, 3} -> {1, 3, 2}
//    {3, 2, 1} -> {1, 2, 3} and report that it wrapped
//
// Edge cases:
//   - empty arrays: merge/union/intersection are empty; next permutation
//     returns false
//   - one array empty: the result is the other array, with duplicates removed
//     for union and intersection
//   - duplicates within one array: the union and intersection code skips them
//   - all values equal: union has one value, intersection has that value
//   - next permutation on one element or on a decreasing array wraps around
//
// Complexity:
//   merge        -> O(n + m) time, O(n + m) space
//   union        -> O(n + m) time, O(n + m) space
//   intersection -> O(n + m) time, O(min(n, m)) space
//   next perm    -> O(n) time, O(1) extra space

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<int> mergeSorted(const vector<int> &first, const vector<int> &second) {
    vector<int> result;
    int i = 0;
    int j = 0;

    while (i < static_cast<int>(first.size()) &&
           j < static_cast<int>(second.size())) {
        if (first[i] <= second[j]) {
            result.push_back(first[i]);
            i++;
        } else {
            result.push_back(second[j]);
            j++;
        }
    }

    while (i < static_cast<int>(first.size())) {
        result.push_back(first[i]);
        i++;
    }

    while (j < static_cast<int>(second.size())) {
        result.push_back(second[j]);
        j++;
    }

    return result;
}

vector<int> sortedUnion(const vector<int> &first, const vector<int> &second) {
    vector<int> result;
    int i = 0;
    int j = 0;

    while (i < static_cast<int>(first.size()) ||
           j < static_cast<int>(second.size())) {
        int value;

        if (j == static_cast<int>(second.size()) ||
            (i < static_cast<int>(first.size()) && first[i] < second[j])) {
            value = first[i];
            i++;
        } else if (i == static_cast<int>(first.size()) ||
                   second[j] < first[i]) {
            value = second[j];
            j++;
        } else {
            value = first[i];
            i++;
            j++;
        }

        if (result.empty() || result.back() != value) {
            result.push_back(value);
        }
    }

    return result;
}

vector<int> sortedIntersection(const vector<int> &first,
                               const vector<int> &second) {
    vector<int> result;
    int i = 0;
    int j = 0;

    while (i < static_cast<int>(first.size()) &&
           j < static_cast<int>(second.size())) {
        if (first[i] < second[j]) {
            i++;
        } else if (second[j] < first[i]) {
            j++;
        } else {
            if (result.empty() || result.back() != first[i]) {
                result.push_back(first[i]);
            }

            i++;
            j++;
        }
    }

    return result;
}

bool nextPermutation(vector<int> &values) {
    int n = static_cast<int>(values.size());
    int pivot = n - 2;

    while (pivot >= 0 && values[pivot] >= values[pivot + 1]) {
        pivot--;
    }

    if (pivot < 0) {
        reverse(values.begin(), values.end());
        return false;
    }

    int successor = n - 1;

    while (values[successor] <= values[pivot]) {
        successor--;
    }

    swap(values[pivot], values[successor]);
    reverse(values.begin() + pivot + 1, values.end());

    return true;
}

void print(const vector<int> &values) {
    if (values.empty()) {
        cout << "(empty) ";
        return;
    }

    for (int value : values) {
        cout << value << " ";
    }
}

int main() {
    vector<int> first = {1, 3, 5, 7};
    vector<int> second = {2, 3, 6, 8};

    cout << "merge: ";
    print(mergeSorted(first, second));
    cout << endl;

    vector<int> third = {1, 2, 2, 4, 5};
    vector<int> fourth = {2, 3, 4, 6};

    cout << "union: ";
    print(sortedUnion(third, fourth));
    cout << endl;

    cout << "intersection: ";
    print(sortedIntersection(third, fourth));
    cout << endl;

    vector<vector<int>> permutations = {{1, 2, 3}, {3, 2, 1}, {1, 1, 5},
                                        {1, 3, 2}, {1},       {}};

    for (vector<int> values : permutations) {
        bool hasNext = nextPermutation(values);

        cout << "next: ";
        print(values);
        cout << "(" << (hasNext ? "true" : "false") << ")" << endl;
    }

    return 0;
}

// Output:
// merge: 1 2 3 3 5 6 7 8
// union: 1 2 3 4 5 6
// intersection: 2 4
// next: 1 3 2 (true)
// next: 1 2 3 (false)
// next: 1 5 1 (true)
// next: 2 1 3 (true)
// next: 1 (false)
// next: (empty) (false)
//
// std::merge, std::set_union, std::set_intersection, and
// std::next_permutation provide these operations. The hand-written versions
// show the two-pointer mechanics and the in-place suffix reversal.
