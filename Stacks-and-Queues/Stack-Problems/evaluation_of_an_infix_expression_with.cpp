/**
 * Evaluation of an infix expression with +, -, *, /, ^ and parentheses.
 *
 * Example:
 * Input:  "1+(2*(3-1))+2"
 * Output: 7
 *
 * '^' is right-associative, so "2^3^2" is 2^(3^2) = 512.
 * Operands are single digits. Time and space complexity -> O(n)
 */

#include <bits/stdc++.h>
using namespace std;

int calc(int v1, int v2, char op) {
    if (op == '^') {
        int result = 1;
        for (int i = 0; i < v2; i++) {
            result *= v1;
        }
        return result;
    }

    if (op == '*') {
        return v1 * v2;
    }

    if (op == '/') {
        return v1 / v2;
    }

    if (op == '+') {
        return v1 + v2;
    }

    return v1 - v2;
};

int precedence(char ch) {
    if (ch == '^') {
        return 3;
    } else if (ch == '*' or ch == '/') {
        return 2;
    } else if (ch == '+' or ch == '-') {
        return 1;
    } else {
        return -1;
    }
};

int eval(string &str) {
    stack<int> nums;
    stack<char> ops;

    for (int i = 0; i < str.size(); i++) {
        if (isdigit(str[i])) {
            nums.push(str[i] - '0');
        } else if (str[i] == '(') {
            ops.push('(');
        } else if (str[i] == ')') {
            while (not ops.empty() and ops.top() != '(') {
                char op = ops.top();
                ops.pop();

                int v2 = nums.top();
                nums.pop();
                int v1 = nums.top();
                nums.pop();
                nums.push(calc(v1, v2, op));
            }
            if (not ops.empty()) {
                ops.pop();
            }
        } else {
            // '^' is right-associative; the other operators are
            // left-associative.
            while (not ops.empty() and ops.top() != '(' and
                   (precedence(ops.top()) > precedence(str[i]) or
                    (precedence(ops.top()) == precedence(str[i]) and
                     str[i] != '^'))) {
                char op = ops.top();
                ops.pop();

                int v2 = nums.top();
                nums.pop();
                int v1 = nums.top();
                nums.pop();
                nums.push(calc(v1, v2, op));
            }
            ops.push(str[i]);
        }
    }
    while (not ops.empty()) {
        char op = ops.top();
        ops.pop();

        int v2 = nums.top();
        nums.pop();
        int v1 = nums.top();
        nums.pop();
        nums.push(calc(v1, v2, op));
    }

    return nums.top();
};

int main() {
    string str = "1+(2*(3-1))+2";
    cout << eval(str) << endl;  // 7

    string power = "2^3^2";
    cout << eval(power) << endl;  // 512

    return 0;
}