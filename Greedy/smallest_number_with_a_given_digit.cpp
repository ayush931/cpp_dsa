/**
 * Smallest Number with a Given Digit Sum
 *
 * Given the number of digits D and the required digit sum S, find the smallest
 * D-digit number whose digits add up to S. Print -1 if no such number exists.
 *
 * Input:  S = 9, D = 2
 * Output: 18
 *
 * Input:  S = 20, D = 3
 * Output: 299
 *
 * Input:  S = 1, D = 2
 * Output: 10
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void smallestNumber(int digits, int sum) {
    if (digits <= 0 || sum < 0 || sum > 9 * digits) {
        cout << -1 << endl;
        return;
    }

    // The only one-digit number with digit sum 0 is 0.
    if (sum == 0) {
        cout << (digits == 1 ? 0 : -1) << endl;
        return;
    }

    vector<int> result(digits, 0);

    // Reserve 1 for the most significant digit so the number has D digits.
    result[0] = 1;
    sum--;

    // Fill from the right with as many 9s as possible; this keeps the leading
    // digits small, which is what makes the whole number smallest.
    for (int i = digits - 1; i > 0 && sum > 0; i--) {
        int take = min(9, sum);
        result[i] = take;
        sum -= take;
    }

    // Any remainder belongs to the most significant digit.
    result[0] += sum;

    for (int digit : result) {
        cout << digit;
    }
    cout << endl;
}

int main() {
    smallestNumber(2, 9);   // 18
    smallestNumber(3, 20);  // 299
    smallestNumber(2, 1);   // 10
    smallestNumber(1, 0);   // 0
    smallestNumber(2, 0);   // -1 (10 is the smallest 2-digit number)
    smallestNumber(2, 19);  // -1 (maximum is 9 + 9 = 18)
    smallestNumber(4, 36);  // 9999

    return 0;
}

/**
 * Time complexity -> O(D) to build and print the digits
 * Space complexity -> O(D) for the result
 *
 * Special cases: S = 0 (only D = 1 works, giving 0), S > 9 * D (impossible),
 * D = 1, S = 9 * D (all nines), and S = 1 (1 followed by zeros).
 */
