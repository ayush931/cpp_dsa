// Sort an array by frequency
//
// Order values by decreasing frequency. When two values have the same
// frequency, put the smaller value first so the result is deterministic.
//
// Example:
//   {1, 1, 2, 2, 2, 3} -> {2, 2, 2, 1, 1, 3}
//   {4, 4, 1, 1, 2, 2, 3} -> {1, 1, 2, 2, 4, 4, 3}
//     (1, 2, and 4 all occur twice, so they are ordered by value)
//
// Brute force: build (value, count) pairs with nested scans, then selection-
// sort the pairs. Optimized: count with a map, then sort with a comparator.

#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

std::vector<int> sortByFrequencyBrute(const std::vector<int> &values) {
    std::vector<std::pair<int, int>> entries;  // {value, count}

    for (int value : values) {
        bool found = false;

        for (int i = 0; i < (int)entries.size(); i++) {
            if (entries[i].first == value) {
                entries[i].second++;
                found = true;
                break;
            }
        }

        if (!found) {
            entries.push_back({value, 1});
        }
    }

    // Selection sort the pairs: higher count first, then smaller value.
    for (int i = 0; i < (int)entries.size(); i++) {
        for (int j = i + 1; j < (int)entries.size(); j++) {
            bool better = entries[j].second > entries[i].second ||
                          (entries[j].second == entries[i].second &&
                           entries[j].first < entries[i].first);

            if (better) {
                std::swap(entries[i], entries[j]);
            }
        }
    }

    std::vector<int> result;
    for (int i = 0; i < (int)entries.size(); i++) {
        for (int count = 0; count < entries[i].second; count++) {
            result.push_back(entries[i].first);
        }
    }

    return result;
}

std::vector<int> sortByFrequency(const std::vector<int> &values) {
    std::map<int, int> frequency;

    for (int value : values) {
        frequency[value]++;
    }

    std::vector<int> result = values;

    std::sort(result.begin(), result.end(), [&frequency](int a, int b) {
        if (frequency.at(a) != frequency.at(b)) {
            return frequency.at(a) > frequency.at(b);
        }
        return a < b;
    });

    return result;
}

void print(const std::vector<int> &values) {
    for (int value : values) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<int> first = {1, 1, 2, 2, 2, 3};
    print(sortByFrequencyBrute(first));  // 2 2 2 1 1 3
    print(sortByFrequency(first));       // 2 2 2 1 1 3

    std::vector<int> second = {4, 4, 1, 1, 2, 2, 3};
    print(sortByFrequency(second));  // 1 1 2 2 4 4 3

    std::vector<int> third = {5, 5, 5};
    print(sortByFrequency(third));  // 5 5 5

    std::vector<int> empty;
    print(sortByFrequency(empty));  // (empty line)

    return 0;
}

// Output:
// 2 2 2 1 1 3
// 2 2 2 1 1 3
// 1 1 2 2 4 4 3
// 5 5 5
//
// Time complexity:
//   brute     -> O(n^2 + n * d), where d is the number of distinct values
//   optimized -> O(n log d + n log n)
// Space complexity: O(d + n) for the map and the result.
//
// Edge cases: empty array, one distinct value, all frequencies equal, and
// duplicate values. The comparator must use frequency.at() rather than
// frequency[]; operator[] could insert a new key during the sort.
