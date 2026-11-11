/**
 * Sqrt decomposition for range minimum and range maximum
 *
 * Divide the array into blocks of size about sqrt(n).
 * Each block stores its minimum and maximum.
 *
 * Point update -> O(1) to update the array, O(blockSize) to rebuild the block
 * Range min/max -> O(blockSize) for partial blocks + O(numberOfBlocks) for full
 * blocks Total -> O(sqrt(n)) per query
 *
 * Edge cases: empty range (returns sentinels), single element, negative values,
 * update outside the array, block boundaries.
 */

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class SqrtMinMax {
    vector<int> arr;
    vector<int> blockMin;
    vector<int> blockMax;
    int blockSize;
    int blocks;

    void rebuild(int block) {
        int left = block * blockSize;
        int right = min((int)arr.size() - 1, left + blockSize - 1);

        int minimum = INT_MAX;
        int maximum = INT_MIN;

        for (int i = left; i <= right; i++) {
            minimum = min(minimum, arr[i]);
            maximum = max(maximum, arr[i]);
        }

        blockMin[block] = minimum;
        blockMax[block] = maximum;
    }

   public:
    SqrtMinMax(vector<int> &input) {
        arr = input;
        int n = arr.size();

        blockSize = max(1, (int)sqrt(n) + 1);
        blocks = (n + blockSize - 1) / blockSize;

        blockMin.assign(blocks, INT_MAX);
        blockMax.assign(blocks, INT_MIN);

        for (int block = 0; block < blocks; block++) {
            rebuild(block);
        }
    }

    void update(int index, int value) {
        if (index < 0 || index >= (int)arr.size()) {
            return;
        }

        arr[index] = value;
        rebuild(index / blockSize);
    }

    pair<int, int> query(int left, int right) {
        if (left > right) {
            return {INT_MAX, INT_MIN};
        }
        if (left < 0) {
            left = 0;
        }
        if (right >= (int)arr.size()) {
            right = arr.size() - 1;
        }

        int minimum = INT_MAX;
        int maximum = INT_MIN;

        int leftBlock = left / blockSize;
        int rightBlock = right / blockSize;

        if (leftBlock == rightBlock) {
            for (int i = left; i <= right; i++) {
                minimum = min(minimum, arr[i]);
                maximum = max(maximum, arr[i]);
            }
            return {minimum, maximum};
        }

        for (int i = left; i < (leftBlock + 1) * blockSize; i++) {
            minimum = min(minimum, arr[i]);
            maximum = max(maximum, arr[i]);
        }

        for (int block = leftBlock + 1; block < rightBlock; block++) {
            minimum = min(minimum, blockMin[block]);
            maximum = max(maximum, blockMax[block]);
        }

        for (int i = rightBlock * blockSize; i <= right; i++) {
            minimum = min(minimum, arr[i]);
            maximum = max(maximum, arr[i]);
        }

        return {minimum, maximum};
    }
};

int main() {
    vector<int> values = {5, 2, 9, 1, 7, 3, 8, 4, 6, 0};
    SqrtMinMax structure(values);

    auto first = structure.query(0, 9);
    cout << first.first << " " << first.second << endl;  // 0 9

    auto second = structure.query(2, 5);
    cout << second.first << " " << second.second << endl;  // 1 9

    structure.update(3, 100);
    auto third = structure.query(0, 9);
    cout << third.first << " " << third.second << endl;  // 0 100

    auto empty = structure.query(5, 2);
    cout << empty.first << " " << empty.second
         << endl;  // 2147483647 -2147483648

    return 0;
}

// Special cases:
// - empty range returns {INT_MAX, INT_MIN}
// - negative values work because both sentinels are wider than any int value
// - a single block is handled by the direct loop
// - updating the last partial block rebuilds only the valid part
