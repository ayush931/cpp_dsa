// Counting frequencies in an array
//
// There are two common representations:
//
//   1. a map from value to count: works for any integer values and uses
//      O(k) memory for k distinct values
//   2. a counting array of size maxValue + 1: O(1) lookup, but only for a
//      small non-negative range
//
// Example:
//   values {4, 1, 4, 2, 1, 4}
//   frequencies: 1 -> 2, 2 -> 1, 4 -> 3
//   mode (most frequent value): 4
//
// A tie has no unique mode. The code below picks the smallest value among the
// tied values because std::map iterates in sorted key order.
//
// Edge cases:
//   - empty array: no frequencies and no mode
//   - all values equal: one entry with count n
//   - negative values: fine with a map, not with a counting array
//   - very large values: a map avoids allocating a huge array
//   - a value outside [0, maxValue]: must be rejected or handled separately
//
// Complexity:
//   std::map          -> O(n log k) time, O(k) space
//   unordered_map     -> O(n) average time, O(k) space
//   counting array    -> O(n + R) time, O(R) space, R = maxValue + 1

#include <iostream>
#include <map>
#include <vector>
using namespace std;

map<int, int> countFrequencies(const vector<int> &values) {
    map<int, int> frequency;

    for (int value : values) {
        frequency[value]++;
    }

    return frequency;
}

int mostFrequent(const map<int, int> &frequency) {
    int bestValue = 0;
    int bestCount = -1;

    for (const auto &entry : frequency) {
        if (entry.second > bestCount) {
            bestValue = entry.first;
            bestCount = entry.second;
        }
    }

    return bestValue;
}

vector<int> countSmallRange(const vector<int> &values, int maxValue) {
    if (maxValue < 0) {
        return {};
    }

    vector<int> counts(maxValue + 1, 0);

    for (int value : values) {
        if (value >= 0 && value <= maxValue) {
            counts[value]++;
        }
    }

    return counts;
}

void printFrequency(const map<int, int> &frequency) {
    for (const auto &entry : frequency) {
        cout << entry.first << " -> " << entry.second << endl;
    }
}

int main() {
    vector<int> values = {4, 1, 4, 2, 1, 4};
    map<int, int> frequency = countFrequencies(values);

    printFrequency(frequency);
    cout << "mode: " << mostFrequent(frequency) << endl;

    vector<int> small = {0, 1, 1, 3, 3, 3};
    vector<int> counts = countSmallRange(small, 3);

    for (int value = 0; value < static_cast<int>(counts.size()); value++) {
        cout << value << ":" << counts[value] << endl;
    }

    // A tie picks the smallest value in the map.
    cout << "tie mode: " << mostFrequent(countFrequencies({2, 2, 1, 1}))
         << endl;

    // Empty input.
    map<int, int> emptyFrequency = countFrequencies({});
    cout << "empty entries: " << emptyFrequency.size() << endl;

    return 0;
}

// Output:
// 1 -> 2
// 2 -> 1
// 4 -> 3
// mode: 4
// 0:1
// 1:2
// 2:0
// 3:3
// tie mode: 1
// empty entries: 0
//
// Prefer a map when the values are sparse or unbounded. Prefer a counting
// array when the range is small and known, because it is faster and simpler.
// If only the maximum frequency is needed, a hash map can stop early once a
// count exceeds n / 2.
