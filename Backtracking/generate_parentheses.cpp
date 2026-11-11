/**
 * Generate All Balanced Parentheses
 *
 * Given n pairs of parentheses, generate every well-formed combination.
 *
 * Example:
 * Input:  n = 3
 * Output: ((())), (()()), (())(), ()(()), ()()()
 *
 * Input:  n = 1
 * Output: ()
 *
 * Input:  n = 0
 * Output: the empty string
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isBalanced(const string &candidate) {
    int balance = 0;

    for (char c : candidate) {
        balance += (c == '(') ? 1 : -1;
        if (balance < 0) {
            return false;
        }
    }

    return balance == 0;
}

// Brute force: build all 2^(2n) strings and validate each one.
void generateAll(int position, int length, string &current,
                 vector<string> &result) {
    if (position == length) {
        if (isBalanced(current)) {
            result.push_back(current);
        }
        return;
    }

    current.push_back('(');
    generateAll(position + 1, length, current, result);
    current.pop_back();

    current.push_back(')');
    generateAll(position + 1, length, current, result);
    current.pop_back();
}

vector<string> generateParenthesisBrute(int n) {
    vector<string> result;
    string current;

    generateAll(0, 2 * n, current, result);

    return result;
}

// Optimized: add '(' while it is still available, and add ')' only when it
// would not make the prefix invalid. This visits only valid prefixes.
void generate(int openUsed, int closeUsed, int n, string &current,
              vector<string> &result) {
    if (current.size() == 2 * n) {
        result.push_back(current);
        return;
    }

    if (openUsed < n) {
        current.push_back('(');
        generate(openUsed + 1, closeUsed, n, current, result);
        current.pop_back();
    }

    if (closeUsed < openUsed) {
        current.push_back(')');
        generate(openUsed, closeUsed + 1, n, current, result);
        current.pop_back();
    }
}

vector<string> generateParenthesis(int n) {
    vector<string> result;
    string current;

    generate(0, 0, n, current, result);

    return result;
}

void print(const vector<string> &combinations) {
    if (combinations.empty()) {
        cout << "(none)\n";
        return;
    }

    for (const string &combination : combinations) {
        if (combination.empty()) {
            cout << "(empty)\n";
        } else {
            cout << combination << "\n";
        }
    }
}

int main() {
    print(generateParenthesisBrute(3));
    // ((()))
    // (()())
    // (())()
    // ()(())
    // ()()()

    cout << "---\n";
    print(generateParenthesis(3));
    // ((()))
    // (()())
    // (())()
    // ()(())
    // ()()()

    cout << "---\n";
    print(generateParenthesis(1));
    // ()

    cout << "---\n";
    print(generateParenthesis(0));
    // (empty)

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^(2n) * n) to build and validate every string
 *   optimized -> O(Catalan(n)) results, each built in O(n)
 * Space complexity -> O(n) recursion depth plus the result
 *
 * Special cases: n = 0 (one empty string), n = 1, and larger n where the
 * Catalan number grows quickly.
 */
