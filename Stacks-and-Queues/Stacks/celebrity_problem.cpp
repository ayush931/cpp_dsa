/**
 * Celebrity problem
 *
 * A celebrity is known by every other person but knows nobody. Given an n x n
 * matrix knows where knows[a][b] == 1 means person a knows person b, return
 * the celebrity's index, or -1 when no celebrity exists.
 *
 * Brute force: verify every person against all others -> O(n^2) time, O(1)
 * space Optimized: eliminate one candidate per comparison -> O(n) time, O(n)
 * space (the stack can be replaced by a single candidate variable, giving O(1)
 * space)
 *
 * Example:
 * knows = {{0, 1, 1},
 *          {0, 0, 0},
 *          {0, 1, 0}}
 * Output: 1
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int celebrityBrute(const vector<vector<int>> &knows) {
    int n = knows.size();

    for (int candidate = 0; candidate < n; candidate++) {
        bool valid = true;

        for (int person = 0; person < n; person++) {
            if (person == candidate) {
                continue;
            }

            // The candidate must not know anyone, and everyone must know them.
            if (knows[candidate][person] == 1 ||
                knows[person][candidate] == 0) {
                valid = false;
                break;
            }
        }

        if (valid) {
            return candidate;
        }
    }

    return -1;
}

int celebrity(const vector<vector<int>> &knows) {
    int n = knows.size();
    if (n == 0) {
        return -1;
    }

    stack<int> candidates;
    for (int person = 0; person < n; person++) {
        candidates.push(person);
    }

    // If a knows b, a cannot be the celebrity; otherwise b cannot be.
    while (candidates.size() > 1) {
        int a = candidates.top();
        candidates.pop();

        int b = candidates.top();
        candidates.pop();

        if (knows[a][b] == 1) {
            candidates.push(b);
        } else {
            candidates.push(a);
        }
    }

    int candidate = candidates.top();

    // The elimination only proves that everyone else is not a celebrity.
    for (int person = 0; person < n; person++) {
        if (person == candidate) {
            continue;
        }

        if (knows[candidate][person] == 1 || knows[person][candidate] == 0) {
            return -1;
        }
    }

    return candidate;
}

void print(const vector<vector<int>> &knows) {
    cout << celebrityBrute(knows) << " " << celebrity(knows) << endl;
}

int main() {
    vector<vector<int>> first = {{0, 1, 1}, {0, 0, 0}, {0, 1, 0}};
    print(first);  // 1 1

    vector<vector<int>> second = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    print(second);  // -1 -1

    vector<vector<int>> third = {{0, 1, 1}, {1, 0, 1}, {0, 0, 0}};
    print(third);  // 2 2

    vector<vector<int>> single = {{0}};
    print(single);  // 0 0

    vector<vector<int>> empty;
    print(empty);  // -1 -1

    return 0;
}

/**
 * The elimination loop removes one person from consideration per comparison,
 * so exactly n - 1 comparisons leave one candidate. A final O(n) pass checks
 * that the candidate really knows nobody and is known by everyone.
 *
 * Special cases: no people, one person, no celebrity, several people who know
 * nobody (then nobody is known by everyone), and a celebrity who is the first
 * or last person in the matrix.
 */
