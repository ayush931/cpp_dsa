/**
 * Decode string
 *
 * Decode a string written with repeat counts: k[encoded] means the encoded
 * part is repeated k times. Counts can have multiple digits and brackets can
 * be nested.
 *
 * Example:
 * Input:  "3[a]2[bc]"
 * Output: "aaabcbc"
 *
 * Input:  "3[a2[c]]"
 * Output: "accaccacc"
 *
 * Input:  "2[abc]3[cd]ef"
 * Output: "abcabccdcdcdef"
 *
 * Recursive parser -> O(n) time and O(depth) stack space
 * Stack parser     -> O(n) time and O(depth) space
 * The output itself can be much longer than the input; total work is
 * O(length of the decoded string).
 */

#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

string decodeRecursive(const string &s, int &position) {
    string result;

    while (position < (int)s.size() && s[position] != ']') {
        if (isdigit((unsigned char)s[position])) {
            int count = 0;

            while (position < (int)s.size() &&
                   isdigit((unsigned char)s[position])) {
                count = count * 10 + (s[position] - '0');
                position++;
            }

            position++;  // skip '['
            string inner = decodeRecursive(s, position);
            position++;  // skip ']'

            for (int i = 0; i < count; i++) {
                result += inner;
            }
        } else {
            result += s[position];
            position++;
        }
    }

    return result;
}

string decodeStringRecursive(const string &s) {
    int position = 0;
    return decodeRecursive(s, position);
}

string decodeString(const string &s) {
    stack<int> counts;
    stack<string> prefixes;
    string current;
    int count = 0;

    for (char ch : s) {
        if (isdigit((unsigned char)ch)) {
            count = count * 10 + (ch - '0');
        } else if (ch == '[') {
            counts.push(count);
            prefixes.push(current);
            current.clear();
            count = 0;
        } else if (ch == ']') {
            string repeated = current;

            current = prefixes.top();
            prefixes.pop();

            int times = counts.top();
            counts.pop();

            for (int i = 0; i < times; i++) {
                current += repeated;
            }
        } else {
            current += ch;
        }
    }

    return current;
}

int main() {
    vector<string> tests = {"3[a]2[bc]", "3[a2[c]]", "2[abc]3[cd]ef",
                            "10[a]",     "abc",      "2[ab3[c]]"};

    cout << decodeString(tests[0]) << endl;           // aaabcbc
    cout << decodeStringRecursive(tests[0]) << endl;  // aaabcbc

    cout << decodeString(tests[1]) << endl;           // accaccacc
    cout << decodeStringRecursive(tests[1]) << endl;  // accaccacc

    cout << decodeString(tests[2]) << endl;  // abcabccdcdcdef
    cout << decodeString(tests[3]) << endl;  // aaaaaaaaaa
    cout << decodeString(tests[4]) << endl;  // abc
    cout << decodeString(tests[5]) << endl;  // abcccabccc

    return 0;
}

/**
 * Special cases: no brackets, multiple-digit repeat counts, deeply nested
 * brackets, an empty encoded part ("2[]" -> ""), and an empty input string.
 * Both parsers assume a valid encoding.
 */
