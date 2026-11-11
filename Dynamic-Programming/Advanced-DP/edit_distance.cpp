/**
 * Edit Distance (Levenshtein distance)
 * Find the minimum number of insert, delete and replace operations to
 * convert word1 into word2.
 *
 * Input: word1 = "horse", word2 = "ros"
 * Output: 3
 *
 * Brute force -> recursion, O(3^(n + m))
 * Optimized -> DP, O(n * m)
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int editDistanceBrute(const string &a, const string &b, int i, int j) {
    if (i == a.size()) {
        return b.size() - j;
    }
    if (j == b.size()) {
        return a.size() - i;
    }

    if (a[i] == b[j]) {
        return editDistanceBrute(a, b, i + 1, j + 1);
    }

    int insertOp = editDistanceBrute(a, b, i, j + 1);
    int deleteOp = editDistanceBrute(a, b, i + 1, j);
    int replaceOp = editDistanceBrute(a, b, i + 1, j + 1);

    return 1 + min({insertOp, deleteOp, replaceOp});
};

int editDistance(const string &a, const string &b) {
    int n = a.size();
    int m = b.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] =
                    1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }

    return dp[n][m];
};

int main() {
    string a = "horse";
    string b = "ros";

    cout << editDistanceBrute(a, b, 0, 0) << endl;  // 3
    cout << editDistance(a, b) << endl;             // 3

    cout << editDistance(a, a) << endl;   // 0
    cout << editDistance(a, "") << endl;  // 5

    return 0;
}

// Special cases: empty strings, one string empty, identical strings, Unicode
// input
