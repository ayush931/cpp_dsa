/**
 * Rabin Karp -> pattern matching using a rolling hash
 * Compare hash values first, then verify character by character
 * Average time -> O(n + m)
 * Worst time -> O(n * m) when many hash collisions occur
 * Space -> O(1)
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> rabinKarp(string text, string pattern, int prime = 101) {
    vector<int> result;

    int n = text.size();
    int m = pattern.size();

    if (m == 0 || m > n) {
        return result;
    }

    const int d = 256;  // number of characters in the alphabet

    int patternHash = 0;
    int textHash = 0;
    int highestPower = 1;

    for (int i = 0; i < m - 1; i++) {
        highestPower = (highestPower * d) % prime;
    }

    for (int i = 0; i < m; i++) {
        patternHash = (patternHash * d + pattern[i]) % prime;
        textHash = (textHash * d + text[i]) % prime;
    }

    for (int i = 0; i <= n - m; i++) {
        if (patternHash == textHash) {
            bool match = true;

            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                result.push_back(i);
            }
        }

        if (i < n - m) {
            textHash =
                (d * (textHash - text[i] * highestPower) + text[i + m]) % prime;

            if (textHash < 0) {
                textHash += prime;
            }
        }
    }

    return result;
};

int main() {
    string text = "ababcababcabc";
    string pattern = "abc";

    vector<int> positions = rabinKarp(text, pattern);

    for (int pos : positions) {
        cout << pos << " ";
    }
    cout << endl;

    return 0;
}
