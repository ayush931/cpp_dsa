/**
 * Remove k digits
 *
 * Given a non-negative integer as a string and an integer k, remove k digits
 * so the remaining number is as small as possible. Leading zeros are removed;
 * an empty result becomes "0".
 *
 * Example:
 * Input:  num = "1432219", k = 3
 * Output: "1219"
 *
 * Input:  num = "10200", k = 1
 * Output: "200"
 *
 * Brute force: repeat k times, erase the first digit that is greater than its
 * successor (or the last digit if the number is non-decreasing) -> O(n * k)
 * Optimized: monotonic increasing stack -> O(n) time, O(n) space
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

string stripLeadingZeros(const string &digits) {
    int start = 0;

    while (start < (int)digits.size() && digits[start] == '0') {
        start++;
    }

    if (start == (int)digits.size()) {
        return "0";
    }
    return digits.substr(start);
}

string removeKDigitsBrute(string num, int k) {
    if (k >= (int)num.size()) {
        return "0";
    }

    for (int removed = 0; removed < k; removed++) {
        int position = 0;

        // Find the first descent: removing this digit makes the number smaller.
        while (position + 1 < (int)num.size() &&
               num[position] <= num[position + 1]) {
            position++;
        }

        num.erase(num.begin() + position);
    }

    return stripLeadingZeros(num);
}

string removeKDigits(string num, int k) {
    if (k >= (int)num.size()) {
        return "0";
    }

    string kept;

    for (char digit : num) {
        // A larger previous digit should be removed while we still have
        // removals.
        while (k > 0 && !kept.empty() && kept.back() > digit) {
            kept.pop_back();
            k--;
        }

        kept.push_back(digit);
    }

    // The number was non-decreasing, so remove the remaining digits from the
    // end.
    while (k > 0) {
        kept.pop_back();
        k--;
    }

    return stripLeadingZeros(kept);
}

int main() {
    vector<pair<string, int>> tests = {{"1432219", 3}, {"10200", 1},
                                       {"10", 2},      {"12345", 2},
                                       {"9", 0},       {"100200", 1}};

    for (auto test : tests) {
        cout << test.first << ", k=" << test.second << " -> "
             << removeKDigits(test.first, test.second) << " (brute "
             << removeKDigitsBrute(test.first, test.second) << ")" << endl;
    }

    // 1432219, k=3 -> 1219 (brute 1219)
    // 10200, k=1 -> 200 (brute 200)
    // 10, k=2 -> 0 (brute 0)
    // 12345, k=2 -> 123 (brute 123)
    // 9, k=0 -> 9 (brute 9)
    // 100200, k=1 -> 200 (brute 200)

    return 0;
}

/**
 * Special cases: k == 0, k >= number of digits, all digits equal, leading
 * zeros after removal, a strictly increasing number (remove from the end),
 * and a single digit.
 */
