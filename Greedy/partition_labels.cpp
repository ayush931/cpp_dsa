/**
 * Partition a byte string into as many pieces as possible such that each
 * distinct byte occurs in only one piece. Precompute last occurrences;
 * extend the current boundary to cover every encountered character's last
 * occurrence. Cut at the earliest boundary that closes all dependencies.
 * O(n) time, O(256) space for 8-bit bytes, plus output.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> partitionLabels(const string &text) {
    array<int, 256> last{};
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        last[static_cast<unsigned char>(text[i])] = i;
    }
    vector<int> lengths;
    int start = 0, boundary = 0;
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        boundary = max(boundary, last[static_cast<unsigned char>(text[i])]);
        if (i == boundary) {
            lengths.push_back(i - start + 1);
            start = i + 1;
        }
    }
    return lengths;
}

int main() {
    auto lengths = partitionLabels("ababcbacadefegdehijhklij");
    assert((lengths == vector<int>{9, 7, 8}));
    assert((partitionLabels("abc") == vector<int>{1, 1, 1}));
    assert((partitionLabels("aaa") == vector<int>{3}));
    assert(partitionLabels("").empty());
    for (int length : lengths) {
        cout << length << ' ';
    }
    cout << '\n';  // 9 7 8
}
