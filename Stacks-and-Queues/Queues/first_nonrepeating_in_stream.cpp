/**
 * First Non-Repeating Character in a Stream
 *
 * Given a stream of lowercase letters, after each character print the first
 * character that has appeared exactly once so far. If no such character
 * exists, print '#'.
 *
 * Example:
 * Input:  "aabcbd"
 * Output: a # b b c c
 */

#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

// Brute force: recount the whole prefix after every character -> O(n^2)
vector<char> firstNonRepeatingBrute(string stream) {
    vector<char> result;

    for (int end = 0; end < stream.size(); end++) {
        int frequency[26] = {0};

        for (int i = 0; i <= end; i++) {
            frequency[stream[i] - 'a']++;
        }

        char answer = '#';

        for (int i = 0; i <= end; i++) {
            if (frequency[stream[i] - 'a'] == 1) {
                answer = stream[i];
                break;
            }
        }

        result.push_back(answer);
    }

    return result;
}

// Optimized: queue of candidates + frequency array -> O(n)
vector<char> firstNonRepeating(string stream) {
    vector<char> result;
    queue<char> candidates;
    int frequency[26] = {0};

    for (char ch : stream) {
        frequency[ch - 'a']++;
        candidates.push(ch);

        // Remove every character that is no longer unique.
        while (!candidates.empty() && frequency[candidates.front() - 'a'] > 1) {
            candidates.pop();
        }

        result.push_back(candidates.empty() ? '#' : candidates.front());
    }

    return result;
}

void print(const vector<char> &values) {
    for (char value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    string first = "aabcbd";
    string second = "abcd";
    string third = "aaaa";
    string fourth = "abacabad";

    print(firstNonRepeatingBrute(first));  // a # b b c c
    print(firstNonRepeating(first));       // a # b b c c

    print(firstNonRepeating(second));  // a b c d
    print(firstNonRepeating(third));   // a # # #
    print(firstNonRepeating(fourth));  // a a b b b c c c

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   queue + frequency -> O(n) time, O(1) space (26 letters)
 *
 * Special cases: every character repeats immediately, all characters are
 * unique, a character becomes unique again (it cannot: once repeated, its
 * count only grows), empty stream, a long stream with a small alphabet.
 */
