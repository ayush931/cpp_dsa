/**
 * A first STL toolkit for reading DSA solutions. C++17.
 * Prerequisites: functions and vectors. Later modules implement these
 * containers from scratch and study their guarantees in greater detail.
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

int main() {
    array<int, 3> fixed{3, 1, 2};  // fixed size; owns its elements
    vector<int> values(fixed.begin(), fixed.end());
    sort(values.begin(), values.end());  // O(n log n), ascending
    // Ranges are half-open: [begin, end); end itself must never be
    // dereferenced.
    auto it = lower_bound(values.begin(), values.end(), 2);  // sorted input
    assert(it != values.end() && *it == 2);
    cout << accumulate(values.begin(), values.end(), 0LL) << '\n';  // 6

    unordered_map<int, int> frequency;  // expected O(1) lookup, O(n) worst
    for (int x : {1, 2, 2}) {
        ++frequency[x];  // [] inserts a missing key
    }
    assert(frequency.find(9) == frequency.end());  // find does not insert
    unordered_set<int> seen{1, 2, 2};
    assert(seen.size() == 2 && frequency.at(2) == 2);
    multiset<int> ordered{3, 1, 3};  // sorted; keeps duplicate occurrences
    ordered.erase(
        ordered.find(3));  // erase one occurrence; erase(3) removes all
    assert(ordered.count(3) == 1 && *ordered.begin() == 1);

    stack<int> lifo;
    queue<int> fifo;
    lifo.push(4);
    fifo.push(4);  // check empty() before top/front/pop
    assert(lifo.top() == 4 && fifo.front() == 4);
    lifo.pop();
    fifo.pop();
    priority_queue<int, vector<int>, greater<int>> minHeap;
    minHeap.push(5);
    minHeap.push(2);  // push/pop O(log n); top O(1)
    assert(minHeap.top() == 2);

    vector<pair<int, int>> jobs{{2, 5}, {1, 4}, {2, 3}};
    sort(jobs.begin(), jobs.end(), [](const auto &a, const auto &b) {
        return a.second < b.second;  // strict ordering: never use <=
    });
    assert(jobs.front().second == 3);
    cout << "STL toolkit checks passed\n";
}
