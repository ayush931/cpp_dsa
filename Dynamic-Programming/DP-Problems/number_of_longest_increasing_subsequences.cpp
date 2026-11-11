/**
 * Number of Longest Increasing Subsequences
 *
 * Given an integer array, return how many longest strictly increasing
 * subsequences it has. Subsequences are distinguished by their indices, so
 * equal values in different positions count separately.
 *
 * Examples:
 *   [1,3,5,4,7]       -> 2   ([1,3,5,7] and [1,3,4,7])
 *   [2,2,2]           -> 3   (each single element)
 *   [1,2,4,3,5,4,7,2] -> 3
 *   [1]               -> 1
 *
 * Also included: Longest Divisible Subset, which is an LIS-style DP after
 * sorting.
 *   [1,2,4,8]     -> 4
 *   [1,2,3,4,9]   -> 3
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: enumerate every index subset and keep the best length and the
// number of subsets that reach it.
// O(2^n) time, O(n) recursion space.
void countLISBrute(const vector<int> &nums, int index, int lastValue,
                   int length, int &bestLength, long long &count) {
    if (index == (int)nums.size()) {
        if (length > bestLength) {
            bestLength = length;
            count = 1;
        } else if (length == bestLength) {
            count++;
        }
        return;
    }

    // Skip nums[index].
    countLISBrute(nums, index + 1, lastValue, length, bestLength, count);

    // Take nums[index] only if it keeps the subsequence strictly increasing.
    if (lastValue == -1 || nums[index] > lastValue) {
        countLISBrute(nums, index + 1, nums[index], length + 1, bestLength,
                      count);
    }
}

long long countLISBruteDriver(const vector<int> &nums) {
    int bestLength = 0;
    long long count = 0;
    countLISBrute(nums, 0, -1, 0, bestLength, count);
    return count;
}

// O(n^2) DP. length[i] is the LIS length ending at i, ways[i] is the number
// of such subsequences.
long long countLIS(const vector<int> &nums) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    vector<int> length(n, 1);
    vector<long long> ways(n, 1);
    int bestLength = 1;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                if (length[j] + 1 > length[i]) {
                    length[i] = length[j] + 1;
                    ways[i] = ways[j];
                } else if (length[j] + 1 == length[i]) {
                    ways[i] += ways[j];
                }
            }
        }

        bestLength = max(bestLength, length[i]);
    }

    long long result = 0;

    for (int i = 0; i < n; i++) {
        if (length[i] == bestLength) {
            result += ways[i];
        }
    }

    return result;
}

// Longest subset in which every pair (smaller, larger) divides evenly.
// Sorting first turns the divisibility relation into a forward DP.
// O(n^2) time, O(n) space.
int longestDivisibleSubset(vector<int> nums) {
    if (nums.empty()) {
        return 0;
    }

    sort(nums.begin(), nums.end());

    vector<int> dp(nums.size(), 1);
    int best = 1;

    for (int i = 1; i < (int)nums.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j] && nums[i] % nums[j] == 0) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }

        best = max(best, dp[i]);
    }

    return best;
}

int main() {
    vector<int> first = {1, 3, 5, 4, 7};
    vector<int> second = {2, 2, 2};
    vector<int> third = {1, 2, 4, 3, 5, 4, 7, 2};

    cout << countLISBruteDriver(first) << " " << countLIS(first) << endl;
    // 2 2

    cout << countLISBruteDriver(second) << " " << countLIS(second) << endl;
    // 3 3

    cout << countLISBruteDriver(third) << " " << countLIS(third) << endl;
    // 3 3

    cout << countLIS({1}) << endl;  // 1
    cout << countLIS({}) << endl;   // 0

    cout << longestDivisibleSubset({1, 2, 4, 8}) << endl;     // 4
    cout << longestDivisibleSubset({1, 2, 3, 4, 9}) << endl;  // 3
    cout << longestDivisibleSubset({3, 6, 9, 18}) << endl;    // 3

    return 0;
}

/**
 * Complexity:
 *   brute count -> O(2^n) time, O(n) recursion space
 *   DP count    -> O(n^2) time, O(n) space
 *   divisible   -> O(n log n) for sorting + O(n^2) DP
 *
 * Edge cases: empty array, one element, all equal elements, strictly
 * decreasing values, and duplicate values (they cannot extend a strictly
 * increasing subsequence).
 *
 * Counting detail: when a longer subsequence is found ending at i, the count
 * is replaced. When another predecessor reaches the same length, the counts
 * are added. The final answer sums every index that reaches the global best
 * length.
 */
