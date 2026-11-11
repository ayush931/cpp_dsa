/**
 * Infer a valid alphabet order from lexicographically sorted lowercase words.
 * Adjacent words give ONE edge at their first differing letter. Detect an
 * invalid prefix ("abc" before "ab") and directed cycles. Include letters
 * with no edges; deduplicate edges before computing indegrees.
 * Kahn's algorithm with a min-heap returns the lexicographically smallest
 * valid order. O(total characters + A^2 + A log A) time, O(A^2) space, A=26.
 * nullopt means invalid; an empty word list has a valid empty alphabet.
 */

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

optional<string> alienOrder(const vector<string> &words) {
    array<bool, 26> present{};
    array<int, 26> indegree{};
    array<array<bool, 26>, 26> edge{};
    for (const auto &word : words) {
        for (char letter : word) {
            if (letter < 'a' || letter > 'z') {
                throw invalid_argument("lowercase words required");
            }
            present[letter - 'a'] = true;
        }
    }
    for (size_t i = 1; i < words.size(); ++i) {
        const auto &before = words[i - 1];
        const auto &after = words[i];
        size_t common = 0;
        while (common < before.size() && common < after.size() &&
               before[common] == after[common]) {
            ++common;
        }
        if (common == after.size() && before.size() > after.size()) {
            return nullopt;
        }
        if (common == before.size() || common == after.size()) {
            continue;
        }
        int from = before[common] - 'a', to = after[common] - 'a';
        if (!edge[from][to]) {
            edge[from][to] = true;
            ++indegree[to];
        }
    }
    priority_queue<int, vector<int>, greater<int>> ready;
    int letters = 0;
    for (int i = 0; i < 26; ++i) {
        if (!present[i]) {
            continue;
        }
        ++letters;
        if (indegree[i] == 0) {
            ready.push(i);
        }
    }
    string order;
    while (!ready.empty()) {
        int from = ready.top();
        ready.pop();
        order.push_back(static_cast<char>('a' + from));
        for (int to = 0; to < 26; ++to) {
            if (edge[from][to] && --indegree[to] == 0) {
                ready.push(to);
            }
        }
    }
    if (static_cast<int>(order.size()) != letters) {
        return nullopt;
    }
    return order;
}

int main() {
    assert(alienOrder({"wrt", "wrf", "er", "ett", "rftt"}) == "wertf");
    assert(!alienOrder({"abc", "ab"}));
    assert(!alienOrder({"z", "x", "z"}));
    assert(alienOrder({"abc"}) == "abc");
    assert(alienOrder({}) == "");
    cout << *alienOrder({"wrt", "wrf", "er", "ett", "rftt"}) << '\n';
}
