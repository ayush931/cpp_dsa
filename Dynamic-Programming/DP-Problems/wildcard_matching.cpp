/**
 * Wildcard Matching
 *
 * Match the ENTIRE text against a pattern: '?' matches exactly one byte,
 * '*' matches any sequence (including empty), other bytes match literally.
 * Unlike regex matching, '*' is independent of the previous pattern symbol.
 *
 * Prefix DP: star either matches nothing (current row, previous column),
 * or consumes the last text byte (previous row, same column). A literal or
 * '?' comes from the diagonal. Preserve the old diagonal before overwriting.
 * Time: O(text length * pattern length). Space: O(pattern length).
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool wildcardMatch(const string &text, const string &pattern) {
    vector<bool> matches(pattern.size() + 1);
    matches[0] = true;
    for (size_t column = 1; column <= pattern.size(); ++column) {
        matches[column] = matches[column - 1] && pattern[column - 1] == '*';
    }

    for (char character : text) {
        bool diagonal = matches[0];
        matches[0] = false;  // a non-empty text cannot match an empty pattern
        for (size_t column = 1; column <= pattern.size(); ++column) {
            bool oldTop = matches[column];
            char token = pattern[column - 1];
            if (token == '*') {
                matches[column] = matches[column - 1] || oldTop;
            } else {
                matches[column] =
                    diagonal && (token == '?' || token == character);
            }
            diagonal = oldTop;
        }
    }
    return matches.back();
}

int main() {
    assert(wildcardMatch("adceb", "*a*b"));
    assert(!wildcardMatch("acdcb", "a*c?b"));
    assert(wildcardMatch("", "**"));
    assert(wildcardMatch("", ""));
    assert(!wildcardMatch("a", ""));
    assert(wildcardMatch("abc", "a?c"));
    cout << boolalpha << wildcardMatch("adceb", "*a*b") << '\n';  // true
}
