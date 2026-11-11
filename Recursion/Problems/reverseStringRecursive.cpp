// Reverse a string recursively
//
// Two common recursive shapes:
//   1. Build a reversed copy: reverse the suffix, then append the first char.
//   2. Swap the two ends and recurse inward, which mutates the string in place.
//
// Example:
//   "hello"   -> "olleh"
//   "recursion" -> "noisrucer"
//   ""        -> ""

#include <iostream>
#include <string>
#include <utility>

// Approach 1: returns a new reversed string.
std::string reverseCopy(const std::string &text, int index) {
    // base case: past the last character
    if (index == (int)text.size()) {
        return "";
    }

    // reverse the rest first, then place the current character at the end
    return reverseCopy(text, index + 1) + text[index];
}

// Approach 2: in-place two-pointer recursion.
void reverseInPlace(std::string &text, int left, int right) {
    if (left >= right) {
        return;
    }

    std::swap(text[left], text[right]);
    reverseInPlace(text, left + 1, right - 1);
}

int main() {
    std::cout << reverseCopy("hello", 0) << "\n";      // olleh
    std::cout << reverseCopy("recursion", 0) << "\n";  // noisrucer
    std::cout << reverseCopy("", 0) << "\n";           // (empty line)

    std::string word = "abcdef";
    reverseInPlace(word, 0, (int)word.size() - 1);
    std::cout << word << "\n";  // fedcba

    std::string single = "x";
    reverseInPlace(single, 0, 0);
    std::cout << single << "\n";  // x

    return 0;
}

// Output:
// olleh
// noisrucer
//
// fedcba
// x
//
// Time complexity: O(n) for both approaches.
// Space complexity:
//   reverseCopy   -> O(n) for the returned string plus O(n) stack
//   reverseInPlace -> O(n) stack, O(1) extra data
//
// Edge cases: empty string, one character, and even/odd lengths (the
// left >= right base case handles both).
