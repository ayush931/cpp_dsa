/**
 * Sqrt decomposition with lazy addition and range minimum
 *
 * Each block stores:
 *   - lazy[block] -> value added to every element in the block
 *   - blockMin[block] -> minimum of the real values in the block
 *
 * Range add:
 *   - partial blocks -> push lazy into the elements, update them directly
 *   - full blocks -> add to lazy and blockMin in O(1)
 *
 * Range minimum:
 *   - partial blocks -> scan elements and add lazy
 *   - full blocks -> use blockMin directly
 *
 * Both operations -> O(sqrt(n))
 *
 * Edge cases: empty range, negative additions, single element, block
 * boundaries.
 */

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class LazySqrtMin {
    vector<int> arr;
    vector<int> blockMin;
    vector<int> lazy;
    int blockSize;
    int blocks;

    void rebuild(int block) {
        int left = block * blockSize;
        int right = min((int)arr.size() - 1, left + blockSize - 1);

        int minimum = INT_MAX;
        for (int i = left; i <= right; i++) {
            minimum = min(minimum, arr[i]);
        }

        blockMin[block] = minimum;
    }

    void push(int block) {
        if (lazy[block] == 0) {
            return;
        }

        int left = block * blockSize;
        int right = min((int)arr.size() - 1, left + blockSize - 1);

        for (int i = left; i <= right; i++) {
            arr[i] += lazy[block];
        }

        lazy[block] = 0;
    }

   public:
    LazySqrtMin(vector<int> &input) {
        arr = input;
        int n = arr.size();

        blockSize = max(1, (int)sqrt(n) + 1);
        blocks = (n + blockSize - 1) / blockSize;

        blockMin.assign(blocks, INT_MAX);
        lazy.assign(blocks, 0);

        for (int block = 0; block < blocks; block++) {
            rebuild(block);
        }
    }

    void rangeAdd(int left, int right, int value) {
        if (left > right) {
            return;
        }
        if (left < 0) {
            left = 0;
        }
        if (right >= (int)arr.size()) {
            right = arr.size() - 1;
        }

        int leftBlock = left / blockSize;
        int rightBlock = right / blockSize;

        if (leftBlock == rightBlock) {
            push(leftBlock);
            for (int i = left; i <= right; i++) {
                arr[i] += value;
            }
            rebuild(leftBlock);
            return;
        }

        push(leftBlock);
        for (int i = left; i < (leftBlock + 1) * blockSize; i++) {
            arr[i] += value;
        }
        rebuild(leftBlock);

        for (int block = leftBlock + 1; block < rightBlock; block++) {
            lazy[block] += value;
            blockMin[block] += value;
        }

        push(rightBlock);
        for (int i = rightBlock * blockSize; i <= right; i++) {
            arr[i] += value;
        }
        rebuild(rightBlock);
    }

    int rangeMin(int left, int right) {
        if (left > right) {
            return INT_MAX;
        }
        if (left < 0) {
            left = 0;
        }
        if (right >= (int)arr.size()) {
            right = arr.size() - 1;
        }

        int answer = INT_MAX;
        int leftBlock = left / blockSize;
        int rightBlock = right / blockSize;

        if (leftBlock == rightBlock) {
            for (int i = left; i <= right; i++) {
                answer = min(answer, arr[i] + lazy[leftBlock]);
            }
            return answer;
        }

        for (int i = left; i < (leftBlock + 1) * blockSize; i++) {
            answer = min(answer, arr[i] + lazy[leftBlock]);
        }

        for (int block = leftBlock + 1; block < rightBlock; block++) {
            answer = min(answer, blockMin[block]);
        }

        for (int i = rightBlock * blockSize; i <= right; i++) {
            answer = min(answer, arr[i] + lazy[rightBlock]);
        }

        return answer;
    }
};

int main() {
    vector<int> values = {5, 2, 9, 1, 7, 3, 8, 4, 6, 0};
    LazySqrtMin structure(values);

    cout << structure.rangeMin(0, 9) << endl;  // 0

    structure.rangeAdd(2, 7, 10);
    cout << structure.rangeMin(0, 9) << endl;  // 0 (index 9 is still 0)
    cout << structure.rangeMin(2, 7) << endl;  // 11 (index 3 was 1)

    structure.rangeAdd(0, 9, -5);
    cout << structure.rangeMin(0, 9) << endl;  // -5 (index 9 was 0)

    cout << structure.rangeMin(4, 3) << endl;  // 2147483647 (empty range)

    return 0;
}

// Special cases:
// - lazy values can be negative
// - partial blocks are pushed before changing individual elements
// - an empty range returns INT_MAX
// - the last block may be shorter than blockSize
