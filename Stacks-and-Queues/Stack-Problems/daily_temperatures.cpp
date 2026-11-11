/**
 * Daily Temperatures
 *
 * Given an array temperatures, return an array answer such that answer[i] is
 * the number of days until a warmer temperature. If no future day is warmer,
 * answer[i] = 0.
 *
 * Example:
 * Input:  [73, 74, 75, 71, 69, 72, 76, 73]
 * Output: [1, 1, 4, 2, 1, 1, 0, 0]
 *
 * Constraints: 1 <= temperatures.length <= 10^5,
 * 30 <= temperatures[i] <= 100.
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

// Brute force: scan forward for every day -> O(n^2) time, O(1) extra space
vector<int> dailyTemperaturesBrute(vector<int> &temperatures) {
    int n = temperatures.size();
    vector<int> answer(n, 0);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (temperatures[j] > temperatures[i]) {
                answer[i] = j - i;
                break;
            }
        }
    }

    return answer;
}

// Monotonic stack: indices with decreasing temperatures -> O(n)
vector<int> dailyTemperatures(vector<int> &temperatures) {
    int n = temperatures.size();
    vector<int> answer(n, 0);
    stack<int> pending;  // indices waiting for a warmer day

    for (int i = 0; i < n; i++) {
        while (!pending.empty() &&
               temperatures[i] > temperatures[pending.top()]) {
            int previous = pending.top();
            pending.pop();

            answer[previous] = i - previous;
        }

        pending.push(i);
    }

    // Indices left on the stack have no warmer day, so their answer stays 0.
    return answer;
}

void print(const vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> first = {73, 74, 75, 71, 69, 72, 76, 73};
    vector<int> second = {30, 40, 50, 60};
    vector<int> third = {30, 60, 90};
    vector<int> decreasing = {90, 80, 70, 60};
    vector<int> single = {50};

    vector<int> brute = dailyTemperaturesBrute(first);
    print(brute);  // 1 1 4 2 1 1 0 0

    vector<int> optimized = dailyTemperatures(first);
    print(optimized);  // 1 1 4 2 1 1 0 0

    print(dailyTemperatures(second));      // 1 1 1 0
    print(dailyTemperatures(third));       // 1 1 0
    print(dailyTemperatures(decreasing));  // 0 0 0 0
    print(dailyTemperatures(single));      // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   monotonic stack -> O(n) time, O(n) space
 *
 * Every index is pushed once and popped at most once, so the total work is
 * linear even though there is a nested while loop.
 *
 * Special cases: strictly decreasing temperatures (all zeros), strictly
 * increasing temperatures (all ones except the last), equal temperatures (not
 * warmer, so they stay on the stack), one day, and duplicate values.
 *
 * Variants: next greater element, stock span, and largest rectangle in a
 * histogram all use the same monotonic-stack pattern.
 */
