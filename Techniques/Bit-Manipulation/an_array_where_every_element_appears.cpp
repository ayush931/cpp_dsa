/**
 * Given an array where every element appears twice except two elements,
 * find the two elements that appear only once.
 *
 * Input: [1, 2, 1, 3, 2, 5]
 * Output: 3 5
 *
 * XOR of all elements gives x ^ y, where x and y are the unique elements
 * A set bit in x ^ y separates x and y into two groups
 */

#include <iostream>
#include <vector>
using namespace std;

pair<int, int> twoUniqueNumbers(vector<int> &arr) {
    int xorAll = 0;

    for (int num : arr) {
        xorAll ^= num;
    }

    int rightmostSetBit = xorAll & (-xorAll);

    int x = 0;
    int y = 0;

    for (int num : arr) {
        if (num & rightmostSetBit) {
            x ^= num;
        } else {
            y ^= num;
        }
    }

    return {x, y};
};

int main() {
    vector<int> arr = {1, 2, 1, 3, 2, 5};

    pair<int, int> result = twoUniqueNumbers(arr);

    cout << result.first << " " << result.second << endl;  // 3 5

    return 0;
}

// Time complexity -> O(n)
// Space complexity -> O(1)
