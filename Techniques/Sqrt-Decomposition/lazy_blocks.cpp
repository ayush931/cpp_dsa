/**
 * Sqrt decomposition with lazy block addition
 * Range add -> O(sqrt(n))
 * Range sum -> O(sqrt(n))
 * Each block stores a pending addition that is applied to all its elements
 */

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class SqrtDecomposition {
    vector<int> arr;
    vector<int> blockSum;
    vector<int> lazy;
    int blockSize;
    int blocks;

   public:
    SqrtDecomposition(vector<int> &input) {
        arr = input;
        int n = arr.size();

        blockSize = sqrt(n) + 1;
        blocks = (n + blockSize - 1) / blockSize;
        blockSum.assign(blocks, 0);
        lazy.assign(blocks, 0);

        for (int i = 0; i < n; i++) {
            blockSum[i / blockSize] += arr[i];
        }
    }

    void rangeAdd(int left, int right, int value) {
        int leftBlock = left / blockSize;
        int rightBlock = right / blockSize;

        if (leftBlock == rightBlock) {
            for (int i = left; i <= right; i++) {
                arr[i] += value;
                blockSum[leftBlock] += value;
            }
            return;
        }

        for (int i = left; i < (leftBlock + 1) * blockSize; i++) {
            arr[i] += value;
            blockSum[leftBlock] += value;
        }

        for (int block = leftBlock + 1; block < rightBlock; block++) {
            lazy[block] += value;
            blockSum[block] += value * blockSize;
        }

        for (int i = rightBlock * blockSize; i <= right; i++) {
            arr[i] += value;
            blockSum[rightBlock] += value;
        }
    }

    int rangeSum(int left, int right) {
        int sum = 0;
        int leftBlock = left / blockSize;
        int rightBlock = right / blockSize;

        if (leftBlock == rightBlock) {
            for (int i = left; i <= right; i++) {
                sum += arr[i] + lazy[leftBlock];
            }
            return sum;
        }

        for (int i = left; i < (leftBlock + 1) * blockSize; i++) {
            sum += arr[i] + lazy[leftBlock];
        }

        for (int block = leftBlock + 1; block < rightBlock; block++) {
            sum += blockSum[block];
        }

        for (int i = rightBlock * blockSize; i <= right; i++) {
            sum += arr[i] + lazy[rightBlock];
        }

        return sum;
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    SqrtDecomposition sd(arr);

    sd.rangeAdd(1, 5, 10);
    cout << sd.rangeSum(0, 4) << endl;  // 1 + 12 + 13 + 14 + 15 = 55

    return 0;
}

// Special cases: left and right in the same block, full range update,
// partial first and last blocks, negative values
