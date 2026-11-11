/**
 * Palindromic Tree (Eertree)
 *
 * An eertree stores every distinct palindromic substring of a string in O(n)
 * time and space. Each node represents one distinct palindrome and stores:
 *   - its length
 *   - a suffix link to the longest proper palindromic suffix
 *   - transitions for appending a character to both ends
 *   - the number of occurrences
 *
 * Two roots simplify the logic:
 *   - node 0 has length -1 (imaginary palindrome)
 *   - node 1 has length 0 (empty palindrome)
 *
 * Adding a character creates at most one new distinct palindrome, so the tree
 * has at most n + 2 nodes.
 *
 * A brute-force palindrome enumerator is included for verification.
 * Input is lowercase ASCII. Add all characters, then finalize occurrence
 * counts with propagateOccurrences(); repeated finalization is harmless.
 */

#include <array>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class PalindromicTree {
   private:
    struct Node {
        int length;
        int suffixLink;
        array<int, 26> next;
        long long occurrences;

        explicit Node(int len) : length(len), suffixLink(0), occurrences(0) {
            next.fill(-1);
        }
    };

    vector<Node> nodes;
    string text;
    int last;
    bool finalized = false;

   public:
    PalindromicTree() {
        nodes.push_back(Node(-1));
        nodes.push_back(Node(0));
        nodes[0].suffixLink = 0;
        nodes[1].suffixLink = 0;
        last = 1;
    }

    void add(char character) {
        if (character < 'a' || character > 'z') {
            throw invalid_argument("eertree expects lowercase ASCII letters");
        }
        if (finalized) {
            throw logic_error("add characters before propagating occurrences");
        }
        text.push_back(character);
        int position = static_cast<int>(text.size()) - 1;
        int current = last;

        while (true) {
            int currentLength = nodes[current].length;
            int mirror = position - 1 - currentLength;

            if (mirror >= 0 && text[mirror] == character) {
                break;
            }
            current = nodes[current].suffixLink;
        }

        int letter = character - 'a';

        if (nodes[current].next[letter] != -1) {
            last = nodes[current].next[letter];
            nodes[last].occurrences++;
            return;
        }

        int newLength = nodes[current].length + 2;
        nodes.push_back(Node(newLength));
        int created = static_cast<int>(nodes.size()) - 1;
        nodes[current].next[letter] = created;

        if (newLength == 1) {
            nodes[created].suffixLink = 1;
        } else {
            int linkCandidate = nodes[current].suffixLink;

            while (true) {
                int candidateLength = nodes[linkCandidate].length;
                int mirror = position - 1 - candidateLength;

                if (mirror >= 0 && text[mirror] == character) {
                    break;
                }
                linkCandidate = nodes[linkCandidate].suffixLink;
            }

            nodes[created].suffixLink = nodes[linkCandidate].next[letter];
        }

        last = created;
        nodes[last].occurrences = 1;
    }

    void propagateOccurrences() {
        if (finalized) {
            return;
        }
        for (int index = static_cast<int>(nodes.size()) - 1; index >= 2;
             index--) {
            nodes[nodes[index].suffixLink].occurrences +=
                nodes[index].occurrences;
        }
        finalized = true;
    }

    int distinctPalindromes() const {
        return static_cast<int>(nodes.size()) - 2;
    }

    long long totalPalindromicSubstrings() const {
        long long total = 0;

        for (int index = 2; index < static_cast<int>(nodes.size()); index++) {
            total += nodes[index].occurrences;
        }

        return total;
    }

    void printLengthsAndCounts() const {
        cout << "[";

        for (int index = 2; index < static_cast<int>(nodes.size()); index++) {
            if (index > 2) {
                cout << ", ";
            }
            cout << "len " << nodes[index].length << " x "
                 << nodes[index].occurrences;
        }

        cout << "]";
    }
};

bool isPalindrome(const string &value) {
    int left = 0;
    int right = static_cast<int>(value.size()) - 1;

    while (left < right) {
        if (value[left] != value[right]) {
            return false;
        }
        left++;
        right--;
    }

    return true;
}

pair<int, long long> bruteForce(const string &text) {
    set<string> distinct;
    long long total = 0;

    for (int start = 0; start < static_cast<int>(text.size()); start++) {
        for (int end = start; end < static_cast<int>(text.size()); end++) {
            string candidate = text.substr(start, end - start + 1);

            if (isPalindrome(candidate)) {
                distinct.insert(candidate);
                total++;
            }
        }
    }

    return {static_cast<int>(distinct.size()), total};
}

void runTest(const string &text) {
    PalindromicTree tree;

    for (char character : text) {
        tree.add(character);
    }

    tree.propagateOccurrences();

    pair<int, long long> expected = bruteForce(text);

    cout << "text: \"" << text << "\"" << endl;
    cout << "distinct palindromes: " << tree.distinctPalindromes()
         << " (brute force: " << expected.first << ")" << endl;
    cout << "total palindromic substrings: "
         << tree.totalPalindromicSubstrings()
         << " (brute force: " << expected.second << ")" << endl;
    cout << "lengths and counts: ";
    tree.printLengthsAndCounts();
    cout << endl;
    cout << "matches: "
         << (tree.distinctPalindromes() == expected.first &&
                     tree.totalPalindromicSubstrings() == expected.second
                 ? "yes"
                 : "no")
         << endl;
    cout << "---" << endl;
}

int main() {
    runTest("ababa");
    runTest("aaaa");
    runTest("abc");
    runTest("a");
    runTest("");

    mt19937 generator(20261111);
    uniform_int_distribution<int> distribution(0, 2);
    bool randomTestsPassed = true;

    for (int test = 0; test < 200; test++) {
        int length = generator() % 12;
        string text;

        for (int i = 0; i < length; i++) {
            text.push_back(static_cast<char>('a' + distribution(generator)));
        }

        PalindromicTree tree;
        for (char character : text) {
            tree.add(character);
        }
        tree.propagateOccurrences();

        pair<int, long long> expected = bruteForce(text);

        if (tree.distinctPalindromes() != expected.first ||
            tree.totalPalindromicSubstrings() != expected.second) {
            randomTestsPassed = false;
            cout << "failed on \"" << text << "\"" << endl;
            break;
        }
    }

    cout << "200 random comparisons: "
         << (randomTestsPassed ? "passed" : "failed") << endl;

    return 0;
}

// Expected output (abridged):
// text: "ababa"
// distinct palindromes: 5 (brute force: 5)
// total palindromic substrings: 9 (brute force: 9)
// lengths and counts: [len 1 x 3, len 1 x 2, len 3 x 2, len 3 x 1, len 5 x 1]
// matches: yes
// ---
// 200 random comparisons: passed
//
// Special cases: empty string, single character, all-equal string, no
// palindromes longer than one character, and overlapping palindromes.
