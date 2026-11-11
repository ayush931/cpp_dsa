// Generate all binary strings of length n with no two consecutive 1s
//
// Example:
//   n = 1 -> "0", "1"
//   n = 2 -> "00", "01", "10"
//   n = 3 -> "000", "001", "010", "100", "101"
//
// Brute force: enumerate all 2^n strings and filter them.
// Backtracking: append 0 freely, but append 1 only when the previous bit is 0.
//
// The count is the Fibonacci number F(n + 2), so the output size itself is
// exponential and cannot be avoided.

#include <iostream>
#include <string>
#include <vector>

bool hasConsecutiveOnes(const std::string &bits) {
    for (int i = 1; i < (int)bits.size(); i++) {
        if (bits[i] == '1' && bits[i - 1] == '1') {
            return true;
        }
    }
    return false;
}

std::vector<std::string> generateBrute(int n) {
    std::vector<std::string> result;

    for (int mask = 0; mask < (1 << n); mask++) {
        std::string bits(n, '0');

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                bits[n - 1 - i] = '1';
            }
        }

        if (!hasConsecutiveOnes(bits)) {
            result.push_back(bits);
        }
    }

    return result;
}

void generateBacktracking(int n, std::string &current, char previous,
                          std::vector<std::string> &result) {
    if ((int)current.size() == n) {
        result.push_back(current);
        return;
    }

    current.push_back('0');
    generateBacktracking(n, current, '0', result);
    current.pop_back();

    if (previous != '1') {
        current.push_back('1');
        generateBacktracking(n, current, '1', result);
        current.pop_back();
    }
}

std::vector<std::string> generateOptimized(int n) {
    std::vector<std::string> result;
    std::string current;
    generateBacktracking(n, current, '0', result);
    return result;
}

void print(const std::vector<std::string> &values) {
    for (int i = 0; i < (int)values.size(); i++) {
        std::cout << values[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    std::vector<std::string> bruteThree = generateBrute(3);
    std::vector<std::string> fastThree = generateOptimized(3);

    print(bruteThree);  // 000 001 010 100 101
    print(fastThree);   // 000 001 010 100 101

    std::cout << bruteThree.size() << " " << fastThree.size() << "\n";  // 5 5

    print(generateOptimized(1));                       // 0 1
    print(generateOptimized(2));                       // 00 01 10
    std::cout << generateOptimized(0).size() << "\n";  // 1 (the empty string)

    return 0;
}

// Output:
// 000 001 010 100 101
// 000 001 010 100 101
// 5 5
// 0 1
// 00 01 10
// 1
//
// Time complexity:
//   brute       -> O(n * 2^n)
//   backtracking -> O(n * F(n + 2)), proportional to the output size
// Space complexity: O(n) recursion stack, plus O(n * output size) for storage.
//
// Edge cases: n = 0 produces the empty string; n = 1 produces both bits; the
// previous-bit check prevents the first character from being a forbidden 1.
