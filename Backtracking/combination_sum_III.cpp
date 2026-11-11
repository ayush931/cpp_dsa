/**
 * Combination Sum III
 *
 * Find every combination of exactly k distinct numbers from 1 to 9 that adds
 * up to n. Each number may be used at most once.
 *
 * Example:
 * Input:  k = 3, n = 7
 * Output: [1,2,4]
 *
 * Input:  k = 3, n = 9
 * Output: [1,2,6], [1,3,5], [2,3,4]
 *
 * Input:  k = 4, n = 1
 * Output: no combinations
 *
 * Constraints: 2 <= k <= 9, 1 <= n <= 60.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every subset of 1..9 and keep the ones with k numbers and
// the required sum. There are only 2^9 subsets, so this is O(2^9 * 9).
void generateAll(int value, int k, int target, vector<int> &current,
                 vector<vector<int>> &result) {
    if (current.size() == k) {
        int sum = 0;
        for (int number : current) {
            sum += number;
        }
        if (sum == target) {
            result.push_back(current);
        }
        return;
    }

    if (value > 9) {
        return;
    }

    generateAll(value + 1, k, target, current, result);  // skip value

    current.push_back(value);  // take value
    generateAll(value + 1, k, target, current, result);
    current.pop_back();
}

vector<vector<int>> combinationSum3Brute(int k, int n) {
    vector<vector<int>> result;
    vector<int> current;

    generateAll(1, k, n, current, result);

    return result;
}

// Optimized: choose numbers in increasing order, prune impossible branches,
// and stop early when the remaining sum cannot be reached.
void generate(int start, int k, int remaining, vector<int> &current,
              vector<vector<int>> &result) {
    if (current.size() == k) {
        if (remaining == 0) {
            result.push_back(current);
        }
        return;
    }

    // Not enough numbers left, or the smallest/largest possible sums miss.
    if (remaining < 0 || 9 - start + 1 < k - current.size()) {
        return;
    }

    for (int value = start; value <= 9; value++) {
        if (value > remaining) {
            break;
        }

        current.push_back(value);
        generate(value + 1, k, remaining - value, current, result);
        current.pop_back();
    }
}

vector<vector<int>> combinationSum3(int k, int n) {
    vector<vector<int>> result;
    vector<int> current;

    generate(1, k, n, current, result);

    return result;
}

void print(const vector<vector<int>> &combinations) {
    if (combinations.empty()) {
        cout << "(none)\n";
        return;
    }

    for (const vector<int> &combination : combinations) {
        cout << "[";
        for (int i = 0; i < combination.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << combination[i];
        }
        cout << "]\n";
    }
}

int main() {
    print(combinationSum3Brute(3, 7));
    // [1,2,4]

    cout << "---\n";
    print(combinationSum3(3, 7));
    // [1,2,4]

    cout << "---\n";
    print(combinationSum3(3, 9));
    // [1,2,6]
    // [1,3,5]
    // [2,3,4]

    cout << "---\n";
    print(combinationSum3(4, 1));
    // (none)

    cout << "---\n";
    print(combinationSum3(2, 18));
    // (none) because the largest distinct pair is 8 + 9 = 17

    return 0;
}

/**
 * Time complexity -> O(C(9, k)) combinations in the worst case, with pruning
 * Space complexity -> O(k) recursion depth plus the result
 *
 * Special cases: n too small or too large for k distinct digits, k = 9
 * (only 1..9), k = 2, and no valid combination.
 */
