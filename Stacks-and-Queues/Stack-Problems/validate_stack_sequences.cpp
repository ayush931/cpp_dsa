/**
 * Validate stack sequences
 *
 * Given two arrays pushed and popped containing distinct values, return true
 * if popped can be produced by pushing the values of pushed in order and
 * popping at arbitrary times.
 *
 * Example:
 * pushed = [1, 2, 3, 4, 5], popped = [4, 5, 3, 2, 1] -> true
 * pushed = [1, 2, 3, 4, 5], popped = [4, 3, 5, 1, 2] -> false
 *
 * Brute force: explore every push/pop choice recursively -> exponential
 * Optimized: simulate the process greedily -> O(n) time, O(n) space
 *
 * The greedy simulation is optimal: whenever the stack top equals the next
 * value in popped, popping it immediately can never hurt a valid sequence.
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

bool validateBruteRec(const vector<int> &pushed, const vector<int> &popped,
                      int pushIndex, int popIndex, stack<int> &st) {
    if (popIndex == (int)popped.size()) {
        return st.empty();
    }

    // Option 1: push the next available value.
    if (pushIndex < (int)pushed.size()) {
        st.push(pushed[pushIndex]);

        if (validateBruteRec(pushed, popped, pushIndex + 1, popIndex, st)) {
            return true;
        }

        st.pop();
    }

    // Option 2: pop the stack top if it matches the next required value.
    if (!st.empty() && st.top() == popped[popIndex]) {
        int value = st.top();
        st.pop();

        if (validateBruteRec(pushed, popped, pushIndex, popIndex + 1, st)) {
            return true;
        }

        st.push(value);
    }

    return false;
}

bool validateBrute(const vector<int> &pushed, const vector<int> &popped) {
    if (pushed.size() != popped.size()) {
        return false;
    }

    stack<int> st;
    return validateBruteRec(pushed, popped, 0, 0, st);
}

bool validateStackSequences(const vector<int> &pushed,
                            const vector<int> &popped) {
    if (pushed.size() != popped.size()) {
        return false;
    }

    stack<int> st;
    int popIndex = 0;

    for (int value : pushed) {
        st.push(value);

        while (!st.empty() && popIndex < (int)popped.size() &&
               st.top() == popped[popIndex]) {
            st.pop();
            popIndex++;
        }
    }

    return popIndex == (int)popped.size() && st.empty();
}

void print(const vector<int> &pushed, const vector<int> &popped) {
    cout << (validateBrute(pushed, popped) ? "true" : "false") << " "
         << (validateStackSequences(pushed, popped) ? "true" : "false") << endl;
}

int main() {
    print({1, 2, 3, 4, 5}, {4, 5, 3, 2, 1});  // true true
    print({1, 2, 3, 4, 5}, {4, 3, 5, 1, 2});  // false false
    print({1, 2, 3}, {1, 2, 3});              // true true
    print({1, 2, 3}, {3, 2, 1});              // true true
    print({1, 2}, {2, 1});                    // true true
    print({1, 2}, {1, 3});                    // false false
    print({}, {});                            // true true
    print({1}, {});                           // false false

    return 0;
}

/**
 * Special cases: both arrays empty, different lengths, a popped value that is
 * never pushed, a valid sequence that requires pushing everything first, and
 * a sequence that needs interleaved pushes and pops.
 */
