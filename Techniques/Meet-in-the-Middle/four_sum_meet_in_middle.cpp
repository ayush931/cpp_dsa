/**
 * 4Sum using meet in the middle
 *
 * Given four arrays A, B, C, D of equal length, count the number of tuples
 * (i, j, k, l) such that A[i] + B[j] + C[k] + D[l] == target.
 *
 * Brute force -> four nested loops, O(n^4)
 * Meet in the middle:
 *   - store every sum A[i] + B[j] in a hash map with its frequency, O(n^2)
 *   - for every C[k] + D[l], add frequency[target - sum], O(n^2)
 * Total -> O(n^2) time, O(n^2) space
 *
 * Edge cases: empty arrays, duplicate sums, negative values, target outside
 * the possible range, overflow when sums are large.
 */

#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

long long fourSumBrute(vector<int> &a, vector<int> &b, vector<int> &c,
                       vector<int> &d, int target) {
    int n = a.size();
    long long count = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                for (int l = 0; l < n; l++) {
                    if (a[i] + b[j] + c[k] + d[l] == target) {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

long long fourSumMeetInMiddle(vector<int> &a, vector<int> &b, vector<int> &c,
                              vector<int> &d, int target) {
    int n = a.size();
    unordered_map<int, int> pairSums;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pairSums[a[i] + b[j]]++;
        }
    }

    long long count = 0;

    for (int k = 0; k < n; k++) {
        for (int l = 0; l < n; l++) {
            int needed = target - (c[k] + d[l]);
            auto it = pairSums.find(needed);
            if (it != pairSums.end()) {
                count += it->second;
            }
        }
    }

    return count;
}

int main() {
    vector<int> a = {1, 2};
    vector<int> b = {-2, -1};
    vector<int> c = {-1, 2};
    vector<int> d = {0, 2};

    cout << fourSumBrute(a, b, c, d, 0) << endl;         // 2
    cout << fourSumMeetInMiddle(a, b, c, d, 0) << endl;  // 2

    vector<int> empty;
    cout << fourSumMeetInMiddle(empty, empty, empty, empty, 0) << endl;  // 0

    vector<int> sameA = {0, 0};
    vector<int> sameB = {0, 0};
    vector<int> sameC = {0, 0};
    vector<int> sameD = {0, 0};
    cout << fourSumMeetInMiddle(sameA, sameB, sameC, sameD, 0) << endl;  // 16

    return 0;
}

// Special cases:
// - empty arrays -> 0
// - all zeros and target 0 -> n^4 tuples, so use long long for the count
// - duplicate pair sums are counted by frequency
// - the hash map key is an int; use long long keys if sums can overflow int
