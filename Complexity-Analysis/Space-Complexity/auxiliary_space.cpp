// Auxiliary space
//
// Auxiliary space is the extra memory an algorithm uses beyond the input and
// the output. It includes:
//   - temporary arrays and hash tables
//   - recursion stack frames
//   - counters, pointers, and loop variables (O(1))
//
// "In-place" usually means O(1) auxiliary space, not zero space. A few
// variables are still needed.
//
// Examples:
//   - reverse an array with two pointers -> O(1) auxiliary
//   - reverse by copying into a new array -> O(n) auxiliary
//   - merge sort -> O(n) auxiliary for the merge buffer
//   - heap sort -> O(1) auxiliary (it sorts in place)
//   - palindrome check with two pointers -> O(1) auxiliary
//   - palindrome check by building a reversed copy -> O(n) auxiliary

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// O(1) auxiliary: swap the ends and move inward.
void reverseInPlace(vector<int> &values) {
    int left = 0;
    int right = values.size() - 1;

    while (left < right) {
        swap(values[left], values[right]);
        left++;
        right--;
    }
}

// O(n) auxiliary: build a new array.
vector<int> reverseWithCopy(vector<int> &values) {
    vector<int> result(values.size());

    for (int i = 0; i < values.size(); i++) {
        result[i] = values[values.size() - 1 - i];
    }

    return result;
}

// O(1) auxiliary: compare characters from both ends.
bool isPalindromeInPlace(const string &text) {
    int left = 0;
    int right = text.size() - 1;

    while (left < right) {
        if (text[left] != text[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

// O(n) auxiliary: build the reversed string.
bool isPalindromeWithCopy(const string &text) {
    string reversed(text.rbegin(), text.rend());
    return text == reversed;
}

void print(vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> first = {1, 2, 3, 4, 5};
    vector<int> second = first;

    reverseInPlace(first);
    print(first);  // 5 4 3 2 1

    vector<int> copied = reverseWithCopy(second);
    print(copied);  // 5 4 3 2 1

    cout << isPalindromeInPlace("racecar") << endl;   // 1
    cout << isPalindromeWithCopy("racecar") << endl;  // 1
    cout << isPalindromeInPlace("hello") << endl;     // 0

    // Merge sort needs a temporary array of size n, so it is not in-place.
    // Heap sort swaps inside the input array, so it is in-place.
    return 0;
}

// Space complexity:
//   reverseInPlace -> O(1) auxiliary
//   reverseWithCopy -> O(n) auxiliary
//   isPalindromeInPlace -> O(1) auxiliary
//   isPalindromeWithCopy -> O(n) auxiliary
//
// Special cases: an empty container, one element, an odd length (the middle
// element needs no swap), a string with spaces or punctuation, and input that
// must not be modified (then a copy is necessary).
//
// Total space = input + auxiliary + output. When people say "O(1) space" they
// normally mean auxiliary space, excluding the input.
