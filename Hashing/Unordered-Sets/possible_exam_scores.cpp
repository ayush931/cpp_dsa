/**
 * Given the number of questions as n, and marks for the correct answer as p and
 * q marks for the incorrect answer. One can either attempt to solve the
 * question in a examination and get either p marks if the answer is right, or q
 * marks if the answer is wrong, or leave the question unattended and get 0
 * marks. The task is to find the count of all the different possible marks that
 * one can score in the examination.
 *
 * Input 1:- n = 2, p = 1, q = -1
 * Output 1: 5
 * Explanation:- The different possible marks are: -2, -1, 0, 1, 2
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, p, q;
    cin >> n >> p >> q;

    unordered_set<int> s;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            int correct = i;
            int incorrect = j;
            int unattended = n - (i + j);

            if (unattended >= 0) {
                int score = correct * p + incorrect * q;
                s.insert(score);
            } else {
                break;
            }
        }
    }

    // Copy and sort so the printed order does not depend on the hash table.
    vector<int> scores(s.begin(), s.end());
    sort(scores.begin(), scores.end());

    for (auto score : scores) {
        cout << score << " ";
    }
    cout << endl;

    cout << "Ans: " << s.size() << endl;  // 5

    return 0;
}