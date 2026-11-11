// next_permutation -> rearranges the range into the next lexicographically
// greater permutation prev_permutation -> rearranges the range into the
// previous lexicographically smaller permutation Both return false when no
// next/previous permutation exists Duplicate elements produce only unique
// permutations

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    string str = "abc";

    do {
        cout << str << endl;
    } while (next_permutation(str.begin(), str.end()));

    vector<int> v = {3, 1, 2};
    next_permutation(v.begin(), v.end());
    for (int el : v) {
        cout << el << " ";
    }
    cout << endl;

    vector<int> v2 = {3, 2, 1};
    prev_permutation(v2.begin(), v2.end());
    for (int el : v2) {
        cout << el << " ";
    }
    cout << endl;

    string duplicate = "aab";
    do {
        cout << duplicate << endl;
    } while (next_permutation(duplicate.begin(), duplicate.end()));

    return 0;
}

// Time complexity of one call -> O(n)
// Total permutations of n distinct elements -> n!
