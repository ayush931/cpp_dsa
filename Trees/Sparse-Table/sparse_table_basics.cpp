/**
 * Sparse table -> answers idempotent range queries in O(1)
 * Build -> O(n log n)
 * Query -> O(1)
 * Updates are not supported
 * Works for min, max, gcd and other idempotent operations
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class SparseTable {
    vector<vector<int>> table;
    vector<int> logs;

   public:
    SparseTable(vector<int> &arr) {
        int n = arr.size();

        logs.resize(n + 1);
        for (int i = 2; i <= n; i++) {
            logs[i] = logs[i / 2] + 1;
        }

        int k = logs[n] + 1;
        table.assign(k, vector<int>(n));

        table[0] = arr;

        for (int j = 1; j < k; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                table[j][i] =
                    min(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    int query(int left, int right) {
        int length = right - left + 1;
        int j = logs[length];

        return min(table[j][left], table[j][right - (1 << j) + 1]);
    }
};

int main() {
    vector<int> arr = {7, 2, 3, 1, 5, 8, 4};

    SparseTable st(arr);

    cout << st.query(0, 3) << endl;  // min(7, 2, 3, 1) = 1
    cout << st.query(2, 6) << endl;  // min(3, 1, 5, 8, 4) = 1
    cout << st.query(4, 6) << endl;  // min(5, 8, 4) = 4

    return 0;
}

// The two overlapping blocks cover the query range completely
// Overlap is allowed because min and max are idempotent
