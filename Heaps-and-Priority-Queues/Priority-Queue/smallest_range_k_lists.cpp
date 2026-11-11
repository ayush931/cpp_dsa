/**
 * Smallest closed range containing >=1 number from each sorted list.
 * Keep one current candidate per list in a min-heap and track their maximum.
 * Only advancing the minimum can improve the range. Stop when its list ends.
 * Tie: smaller left endpoint wins. O(N log k) time, O(k) space for N total
 * values, k lists. Any empty list (or no lists) has no feasible range.
 */

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

optional<pair<int, int>> smallestRange(const vector<vector<int>> &lists) {
    if (lists.empty()) {
        return nullopt;
    }
    using Entry = tuple<int, size_t, size_t>;  // value, list, index
    priority_queue<Entry, vector<Entry>, greater<Entry>> heap;
    int maximum = 0;
    for (size_t row = 0; row < lists.size(); ++row) {
        if (lists[row].empty()) {
            return nullopt;
        }
        heap.emplace(lists[row][0], row, 0);
        maximum = row == 0 ? lists[row][0] : max(maximum, lists[row][0]);
    }
    pair<int, int> best{get<0>(heap.top()), maximum};
    while (true) {
        auto [minimum, row, index] = heap.top();
        heap.pop();
        long long width = static_cast<long long>(maximum) - minimum;
        long long bestWidth = static_cast<long long>(best.second) - best.first;
        if (width < bestWidth || (width == bestWidth && minimum < best.first)) {
            best = {minimum, maximum};
        }
        if (++index == lists[row].size()) {
            break;
        }
        int next = lists[row][index];
        maximum = max(maximum, next);
        heap.emplace(next, row, index);
    }
    return best;
}

int main() {
    auto range =
        smallestRange({{4, 10, 15, 24, 26}, {0, 9, 12, 20}, {5, 18, 22, 30}});
    assert((range == make_pair(20, 24)));
    assert((smallestRange({{1, 2}, {1, 3}}) == make_pair(1, 1)));
    assert(!smallestRange({{}, {2}}));
    assert(!smallestRange({}));
    cout << range->first << ' ' << range->second << '\n';  // 20 24
}
