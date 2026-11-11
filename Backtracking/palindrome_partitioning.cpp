/**
 * Palindrome Partitioning
 *
 * Given a string s, split it into substrings so that every substring is a
 * palindrome. Return all possible palindrome partitions.
 *
 * Example:
 * Input:  "aab"
 * Output: ["a","a","b"], ["aa","b"]
 *
 * Input:  "aba"
 * Output: ["a","b","a"], ["aba"]
 *
 * Input:  "ab"
 * Output: ["a","b"]
 *
 * Constraints: 1 <= s.length <= 16, s contains lowercase English letters.
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isPalindrome(const string &s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Brute force: generate every possible partition, then keep only the ones in
// which every part is a palindrome. There are 2^(n-1) partitions.
void generateAll(const string &s, int start, vector<string> &current,
                 vector<vector<string>> &all) {
    if (start == s.size()) {
        all.push_back(current);
        return;
    }

    for (int end = start; end < s.size(); end++) {
        current.push_back(s.substr(start, end - start + 1));
        generateAll(s, end + 1, current, all);
        current.pop_back();
    }
}

vector<vector<string>> partitionBrute(const string &s) {
    vector<vector<string>> all;
    vector<string> current;

    generateAll(s, 0, current, all);

    vector<vector<string>> result;

    for (const vector<string> &parts : all) {
        bool valid = true;

        for (const string &part : parts) {
            if (!isPalindrome(part, 0, part.size() - 1)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            result.push_back(parts);
        }
    }

    return result;
}

// Optimized: only recurse on prefixes that are already palindromes.
void generate(const string &s, int start, vector<string> &current,
              vector<vector<string>> &result) {
    if (start == s.size()) {
        result.push_back(current);
        return;
    }

    for (int end = start; end < s.size(); end++) {
        if (!isPalindrome(s, start, end)) {
            continue;
        }

        current.push_back(s.substr(start, end - start + 1));
        generate(s, end + 1, current, result);
        current.pop_back();
    }
}

vector<vector<string>> partition(const string &s) {
    vector<vector<string>> result;
    vector<string> current;

    generate(s, 0, current, result);

    return result;
}

void print(const vector<vector<string>> &partitions) {
    for (const vector<string> &parts : partitions) {
        cout << "[";
        for (int i = 0; i < parts.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << "\"" << parts[i] << "\"";
        }
        cout << "]\n";
    }
}

int main() {
    vector<vector<string>> brute = partitionBrute("aab");
    print(brute);
    // ["a","a","b"]
    // ["aa","b"]

    cout << "---\n";

    vector<vector<string>> optimized = partition("aab");
    print(optimized);
    // ["a","a","b"]
    // ["aa","b"]

    cout << "---\n";
    print(partition("aba"));
    // ["a","b","a"]
    // ["aba"]

    cout << "---\n";
    print(partition("ab"));
    // ["a","b"]

    cout << "---\n";
    print(partition("a"));
    // ["a"]

    return 0;
}

/**
 * Time complexity -> O(n * 2^n) in the worst case: 2^(n-1) partitions and
 * O(n) work to copy or check each one; palindrome pruning reduces this a lot.
 * Space complexity -> O(n) recursion depth plus the result.
 *
 * Special cases: one character, all characters equal (every partition works),
 * no repeated characters, and a string that is itself a palindrome.
 */
