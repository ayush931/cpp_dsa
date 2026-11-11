/**
 * Print all permutations of a string in lexicographically sorted order.
 *
 * Input:  PQR
 * Output: PQR PRQ QPR QRP RPQ RQP
 *
 * Duplicate characters are handled too: the result contains each distinct
 * permutation only once.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void generate(string &str, vector<bool> &used, string &current) {
    if (current.size() == str.size()) {
        cout << current << "\n";
        return;
    }

    for (int i = 0; i < str.size(); i++) {
        if (used[i]) {
            continue;
        }

        // Skip a duplicate character at the same recursion level.
        if (i > 0 && str[i] == str[i - 1] && !used[i - 1]) {
            continue;
        }

        used[i] = true;
        current.push_back(str[i]);

        generate(str, used, current);

        current.pop_back();
        used[i] = false;
    }
}

void permutation(string &str) {
    sort(str.begin(), str.end());

    vector<bool> used(str.size(), false);
    string current;

    generate(str, used, current);
}

int main() {
    string first = "PQR";
    permutation(first);
    // PQR PRQ QPR QRP RPQ RQP

    cout << "---\n";

    string second = "aab";
    permutation(second);
    // aab aba baa

    return 0;
}

/**
 * Time complexity -> O(n * n!) to print every permutation, O(n log n) to sort
 * Space complexity -> O(n) for the used array, current string and recursion
 *
 * Special cases: empty string (prints one empty line), one character, all
 * characters equal (one permutation), duplicate characters, uppercase and
 * lowercase characters (sorted by their ASCII values).
 */
