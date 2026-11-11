/**
 * Sqrt decomposition -> divide the array into blocks of size sqrt(n)
 * Point update -> O(1)
 * Range query -> O(sqrt(n))
 * Good when the update and query costs need to be balanced
 */

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

class SqrtDecomposition {
    vector<int> arr;
    vector<int> blockSum;
    int blockSize;
    int blocks;

   public:
    SqrtDecomposition(vector<int> &input) {
        arr = input;
        int n = arr.size();

        blockSize = sqrt(n) + 1;
        blocks = (n + blockSize - 1) / blockSize;
        blockSum.assign(blocks, 0);

        for (int i = 0; i < n; i++) {
            blockSum[i / blockSize] += arr[i];
        }
    }

    void update(int index, int value) {
        int block = index / blockSize;
        blockSum[block] += value - arr[index];
        arr[index] = value;
    }

    int query(int left, int right) {
        int sum = 0;

        while (left <= right && left % blockSize != 0 && left != 0) {
            sum += arr[left];
            left++;
        }

        while (left + blockSize - 1 <= right) {
            sum += blockSum[left / blockSize];
            left += blockSize;
        }

        while (left <= right) {
            sum += arr[left];
            left++;
        }

        return sum;
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    SqrtDecomposition sd(arr);

    cout << sd.query(1, 5) << endl;  // 2 + 3 + 4 + 5 + 6 = 20

    sd.update(2, 10);
    cout << sd.query(1, 5) << endl;  // 2 + 10 + 4 + 5 + 6 = 27

    return 0;
}

// Special cases: left = 0, single element query, full range query,
// array size not a perfect square, empty array
