/**
 * Determine whether third interleaves first and second while preserving the
 * order within each. Prefix DP: dp[i][j] comes from first[i-1] or second[j-1].
 * In a rolling row, ways[j] is the old row and ways[j-1] the current row.
 * O(m*n) time, O(n) space; bytes are compared literally.
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isInterleave(const string &first, const string &second,
                  const string &third) {
    if (first.size() + second.size() != third.size()) {
        return false;
    }
    vector<bool> ways(second.size() + 1, false);
    ways[0] = true;
    for (size_t i = 0; i <= first.size(); ++i) {
        for (size_t j = 0; j <= second.size(); ++j) {
            if (i == 0 && j == 0) {
                continue;
            }
            bool fromFirst =
                i > 0 && ways[j] && first[i - 1] == third[i + j - 1];
            bool fromSecond =
                j > 0 && ways[j - 1] && second[j - 1] == third[i + j - 1];
            ways[j] = fromFirst || fromSecond;
        }
    }
    return ways.back();
}

int main() {
    assert(isInterleave("aabcc", "dbbca", "aadbbcbcac"));
    assert(!isInterleave("aabcc", "dbbca", "aadbbbaccc"));
    assert(isInterleave("", "", ""));
    assert(isInterleave("abc", "", "abc"));
    assert(!isInterleave("a", "b", "a"));
    cout << boolalpha << isInterleave("aabcc", "dbbca", "aadbbcbcac") << '\n';
}
