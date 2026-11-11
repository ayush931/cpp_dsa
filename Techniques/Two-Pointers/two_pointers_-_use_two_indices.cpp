/**
 * Two pointers -> use two indices to scan a range
 * Same direction -> removing duplicates, moving elements
 * Opposite direction -> pair sum in a sorted array, palindrome check
 * Reduces many O(n^2) solutions to O(n)
 */

#include <iostream>
#include <vector>
using namespace std;

bool pairSumSorted(vector<int> &arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];

        if (sum == target) {
            return true;
        }
        if (sum < target) {
            left++;
        } else {
            right--;
        }
    }

    return false;
};

int removeDuplicates(vector<int> &arr) {
    if (arr.empty()) {
        return 0;
    }

    int write = 1;

    for (int read = 1; read < arr.size(); read++) {
        if (arr[read] != arr[write - 1]) {
            arr[write] = arr[read];
            write++;
        }
    }

    return write;
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 6, 8};
    cout << pairSumSorted(arr, 10) << endl;  // 1

    vector<int> duplicates = {1, 1, 2, 2, 2, 3, 4, 4};
    int length = removeDuplicates(duplicates);
    cout << length << endl;  // 4

    for (int i = 0; i < length; i++) {
        cout << duplicates[i] << " ";
    }
    cout << endl;

    return 0;
}

// Time complexity -> O(n)
// Space complexity -> O(1)
