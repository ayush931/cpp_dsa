/**
 * Prefix sum -> prefix[i] stores the sum of the first i elements
 * Range sum [l, r] = prefix[r + 1] - prefix[l]
 * Build -> O(n), query -> O(1)
 *
 * 2-D prefix sum -> sum of the rectangle (0, 0) to (i, j)
 * Submatrix sum = prefix[r2][c2] - prefix[r1 - 1][c2]
 *                 - prefix[r2][c1 - 1] + prefix[r1 - 1][c1 - 1]
 */

#include <iostream>
#include <vector>
using namespace std;

vector<long long> buildPrefix(vector<int> &arr) {
    int n = arr.size();
    vector<long long> prefix(n + 1, 0);

    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + arr[i];
    }

    return prefix;
};

long long rangeSum(vector<long long> &prefix, int left, int right) {
    return prefix[right + 1] - prefix[left];
};

vector<vector<long long>> buildPrefix2D(vector<vector<int>> &matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<vector<long long>> prefix(rows + 1, vector<long long>(cols + 1, 0));

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            prefix[i][j] = matrix[i - 1][j - 1] + prefix[i - 1][j] +
                           prefix[i][j - 1] - prefix[i - 1][j - 1];
        }
    }

    return prefix;
};

long long submatrixSum(vector<vector<long long>> &prefix, int r1, int c1,
                       int r2, int c2) {
    return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] +
           prefix[r1][c1];
};

int main() {
    vector<int> arr = {2, 4, 1, 7, 3, 5};
    vector<long long> prefix = buildPrefix(arr);

    cout << rangeSum(prefix, 1, 3) << endl;  // 4 + 1 + 7 = 12

    vector<vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    vector<vector<long long>> prefix2D = buildPrefix2D(matrix);
    cout << submatrixSum(prefix2D, 1, 1, 2, 2) << endl;  // 5 + 6 + 8 + 9 = 28

    return 0;
}

// Special cases: empty array, l > r, 0-based indices, use long long to avoid
// overflow
