/**
 * Happy Number
 *
 * A happy number is a number that eventually reaches 1 when it is repeatedly
 * replaced by the sum of the squares of its digits. If the process loops
 * forever without reaching 1, the number is unhappy.
 *
 * Example:
 * 19 -> 1^2 + 9^2 = 82 -> 68 -> 100 -> 1  (happy)
 * 2  -> 4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4 (cycle, unhappy)
 */

#include <iostream>
#include <unordered_set>
using namespace std;

int digitSquareSum(int n) {
    int sum = 0;

    while (n > 0) {
        int digit = n % 10;
        sum += digit * digit;
        n /= 10;
    }

    return sum;
}

// Brute force: remember every value seen -> O(log n) per step, O(cycle) space
bool isHappyBrute(int n) {
    unordered_set<int> seen;

    while (n != 1 && !seen.count(n)) {
        seen.insert(n);
        n = digitSquareSum(n);
    }

    return n == 1;
}

// Optimized: Floyd's cycle detection (slow/fast pointers) -> O(1) space
bool isHappy(int n) {
    int slow = n;
    int fast = digitSquareSum(n);

    while (fast != 1 && slow != fast) {
        slow = digitSquareSum(slow);
        fast = digitSquareSum(digitSquareSum(fast));
    }

    return fast == 1;
}

int main() {
    cout << isHappyBrute(19) << " " << isHappy(19) << endl;  // 1 1
    cout << isHappyBrute(2) << " " << isHappy(2) << endl;    // 0 0
    cout << isHappy(1) << endl;                              // 1
    cout << isHappy(7) << endl;                              // 1
    cout << isHappy(100) << endl;                            // 1
    cout << isHappy(4) << endl;                              // 0

    return 0;
}

/**
 * Time complexity -> O(log n) per digit-square step; the cycle is short in
 * practice (at most a few dozen values).
 * Space complexity -> O(1) with Floyd's algorithm, O(cycle length) with a set.
 *
 * Special cases: n = 1 (happy), single-digit unhappy numbers (2, 3, 4, 5, 6,
 * 8, 9), numbers that reach 1 after many steps, very large n (int is enough
 * for the digit-square sum), negative n (not allowed; take abs first if
 * needed).
 */
