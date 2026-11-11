/**
 * Largest Number
 *
 * Given a list of non-negative integers nums, arrange them such that they form
 * the largest number and return it as a string.
 *
 * Example:
 * Input:  [3, 30, 34, 5, 9]
 * Output: "9534330"
 *
 * Input:  [10, 2]
 * Output: "210"
 *
 * Constraints: 1 <= nums.length <= 100, 0 <= nums[i] <= 10^9.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Brute force: try every permutation -> O(n! * n)
string largestNumberBrute(vector<int> &nums) {
    vector<string> parts;

    for (int value : nums) {
        parts.push_back(to_string(value));
    }

    sort(parts.begin(), parts.end());

    string best = "";

    do {
        string candidate = "";

        for (string &part : parts) {
            candidate += part;
        }

        best = max(best, candidate);
    } while (next_permutation(parts.begin(), parts.end()));

    return best;
}

// Optimized: sort with the comparator a + b > b + a -> O(n log n)
string largestNumber(vector<int> &nums) {
    vector<string> parts;

    for (int value : nums) {
        parts.push_back(to_string(value));
    }

    sort(parts.begin(), parts.end(),
         [](const string &a, const string &b) { return a + b > b + a; });

    // All zeros: the answer must be "0", not "000..."
    if (!parts.empty() && parts[0] == "0") {
        return "0";
    }

    string result = "";

    for (string &part : parts) {
        result += part;
    }

    return result;
}

int main() {
    vector<int> first = {3, 30, 34, 5, 9};
    vector<int> second = {10, 2};
    vector<int> third = {0, 0};
    vector<int> fourth = {1};
    vector<int> fifth = {999999998, 999999997, 999999999};

    cout << largestNumberBrute(second) << endl;  // 210
    cout << largestNumber(first) << endl;        // 9534330
    cout << largestNumber(second) << endl;       // 210
    cout << largestNumber(third) << endl;        // 0
    cout << largestNumber(fourth) << endl;       // 1
    cout << largestNumber(fifth) << endl;        // 999999999999999998999999997

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n! * n)
 *   optimized -> O(n log n * k), where k is the length of the numbers
 *
 * Special cases: all zeros ("0"), one element, numbers with different digit
 * lengths, values that share a prefix (3 and 30), very large values (compare
 * strings, not integers), duplicate values.
 */
