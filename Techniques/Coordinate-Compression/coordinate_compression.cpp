/**
 * Coordinate compression: replace sparse values by ranks in sorted unique
 * order. Equal values get equal ranks. Preserves comparisons, NOT distances
 * or arithmetic: original 10 and 1000 may have consecutive ranks.
 * Offline O(n log n) time, O(n) space. Useful before Fenwick/segment trees.
 * An unseen query value uses lower_bound/upper_bound to count coordinates;
 * it does not automatically become an existing compressed coordinate.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

vector<int> compress(const vector<long long> &values,
                     vector<long long> &coordinates) {
    coordinates = values;
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()),
                      coordinates.end());
    vector<int> ranks;
    ranks.reserve(values.size());
    for (long long x : values) {
        ranks.push_back(static_cast<int>(
            lower_bound(coordinates.begin(), coordinates.end(), x) -
            coordinates.begin()));
    }
    return ranks;
}

int main() {
    vector<long long> coordinates;
    auto ranks = compress({1000000000LL, -5, 42, -5}, coordinates);
    assert((ranks == vector<int>{2, 0, 1, 0}));
    assert((coordinates == vector<long long>{-5, 42, 1000000000LL}));
    assert(lower_bound(coordinates.begin(), coordinates.end(), 40) -
               coordinates.begin() ==
           1);  // one distinct coordinate < 40
    assert(compress({}, coordinates).empty() && coordinates.empty());
    cout << "2 0 1 0\n";
}
