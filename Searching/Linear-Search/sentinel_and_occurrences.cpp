/**
 * Linear Search Variants: Sentinel, First/Last Occurrence, Frequency
 *
 * Sentinel linear search appends the target to the end of the array, so the
 * scan loop needs no bounds check. If the match lands on the appended slot,
 * the target was not present in the original array.
 *
 * Example:
 *   arr = {4, 7, 2, 7, 9, 7, 1}
 *   sentinelSearch(arr, 9)  -> 4
 *   sentinelSearch(arr, 5)  -> -1
 *   firstOccurrence(arr, 7) -> 1
 *   lastOccurrence(arr, 7)  -> 5
 *   frequency of 7          -> 3
 *
 * Time complexity  -> O(n) per search
 * Space complexity -> O(1) extra beyond the copy used for the sentinel
 *
 * Edge cases: empty array, target at the first or last position, target
 * absent, duplicate values, negative values.
 */

#include <iostream>
#include <map>
#include <vector>
using namespace std;

int sentinelSearch(vector<int> arr, int target) {
    int n = arr.size();
    arr.push_back(target);  // sentinel

    int i = 0;
    while (arr[i] != target) {
        i++;
    }

    return (i == n) ? -1 : i;
}

int firstOccurrence(vector<int> &arr, int target) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

int lastOccurrence(vector<int> &arr, int target) {
    for (int i = (int)arr.size() - 1; i >= 0; i--) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}

map<int, int> frequencyCount(vector<int> &arr) {
    map<int, int> frequency;

    for (int value : arr) {
        frequency[value]++;
    }

    return frequency;
}

void printFrequency(map<int, int> &frequency) {
    if (frequency.empty()) {
        cout << "(empty)" << endl;
        return;
    }

    for (auto &entry : frequency) {
        cout << entry.first << ":" << entry.second << " ";
    }
    cout << endl;
}

int main() {
    vector<int> arr = {4, 7, 2, 7, 9, 7, 1};

    cout << sentinelSearch(arr, 9) << endl;   // 4
    cout << sentinelSearch(arr, 5) << endl;   // -1
    cout << firstOccurrence(arr, 7) << endl;  // 1
    cout << lastOccurrence(arr, 7) << endl;   // 5
    cout << firstOccurrence(arr, 3) << endl;  // -1

    map<int, int> frequency = frequencyCount(arr);
    printFrequency(frequency);  // 1:1 2:1 4:1 7:3 9:1

    vector<int> empty;
    cout << sentinelSearch(empty, 3) << endl;   // -1
    cout << firstOccurrence(empty, 3) << endl;  // -1

    map<int, int> emptyFrequency = frequencyCount(empty);
    printFrequency(emptyFrequency);  // (empty)

    return 0;
}
