/**
 * Minimum Brackets to Remove
 *
 * Given a bracket sequence containing only '(' and ')', return the minimum
 * number of brackets that must be removed to make the sequence balanced.
 *
 * A sequence is balanced when every closing bracket has a matching opening
 * bracket before it and no opening bracket is left unmatched.
 *
 * Example:
 * Input:  "(()())"
 * Output: 0
 *
 * Input:  "())(()"
 * Output: 2
 *
 * Input:  "((("
 * Output: 3
 */

#include <iostream>
#include <stack>
#include <string>
using namespace std;

// Brute force: use a stack to match brackets, then count what is left.
// O(n) time, O(n) space.
int minRemovalsBrute(const string &sequence) {
    stack<char> openBrackets;
    int unmatchedClosing = 0;

    for (char ch : sequence) {
        if (ch == '(') {
            openBrackets.push(ch);
        } else if (ch == ')') {
            if (!openBrackets.empty()) {
                openBrackets.pop();
            } else {
                unmatchedClosing++;
            }
        }
    }

    // Every remaining opening bracket is also unmatched.
    return unmatchedClosing + openBrackets.size();
}

// Optimized: a single counter replaces the stack.
// O(n) time, O(1) space.
int minRemovals(const string &sequence) {
    int openCount = 0;
    int unmatchedClosing = 0;

    for (char ch : sequence) {
        if (ch == '(') {
            openCount++;
        } else if (ch == ')') {
            if (openCount > 0) {
                openCount--;
            } else {
                unmatchedClosing++;
            }
        }
    }

    return unmatchedClosing + openCount;
}

int main() {
    string first = "(()())";
    string second = "())(()";
    string third = "(((";
    string fourth = ")))";
    string fifth = "";
    string sixth = ")(";

    cout << minRemovalsBrute(first) << " " << minRemovals(first) << endl;
    // 0 0

    cout << minRemovalsBrute(second) << " " << minRemovals(second) << endl;
    // 2 2

    cout << minRemovals(third) << endl;   // 3
    cout << minRemovals(fourth) << endl;  // 3
    cout << minRemovals(fifth) << endl;   // 0
    cout << minRemovals(sixth) << endl;   // 2

    return 0;
}

/**
 * Time complexity -> O(n) for both versions
 * Space complexity -> O(n) for the stack, O(1) for the counter
 *
 * Special cases: an empty string, only opening brackets, only closing
 * brackets, already balanced input, a closing bracket before any opening one,
 * and nested brackets.
 *
 * This is the same matching idea as the balanced-parentheses validation
 * problem. The counter version works because only the number of unmatched
 * opening brackets matters, not their positions.
 */
