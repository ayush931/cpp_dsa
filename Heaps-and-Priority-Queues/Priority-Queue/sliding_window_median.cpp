/**
 * Sliding window median: ordered lower/upper halves with exact deletion.
 * Multisets support deletion of an outgoing occurrence in O(log k); this
 * avoids the lazy-deletion bookkeeping needed by two priority queues.
 * Invariant: every lower value <= every upper value; lower has equal size or
 * one extra. O(n log k) time, O(k) working space plus O(n-k+1) output.
 * Read the STL multiset introduction before this ordered-container variant.
 */

#include <cassert>
#include <climits>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <vector>
using namespace std;

class MedianWindow {
    multiset<int> lower, upper;

    void balance() {
        while (lower.size() > upper.size() + 1) {
            auto largest = prev(lower.end());
            upper.insert(*largest);
            lower.erase(largest);
        }
        while (lower.size() < upper.size()) {
            auto smallest = upper.begin();
            lower.insert(*smallest);
            upper.erase(smallest);
        }
    }

   public:
    void add(int value) {
        if (lower.empty() || value <= *lower.rbegin()) {
            lower.insert(value);
        } else {
            upper.insert(value);
        }
        balance();
    }

    void remove(int value) {
        auto found = lower.find(value);
        if (found != lower.end()) {
            lower.erase(found);  // exactly one occurrence
        } else {
            found = upper.find(value);
            if (found == upper.end()) {
                throw logic_error("value not in window");
            }
            upper.erase(found);
        }
        balance();
    }

    double median() const {
        if (lower.empty()) {
            throw logic_error("empty window");
        }
        if (lower.size() > upper.size()) {
            return *lower.rbegin();
        }
        return (static_cast<long long>(*lower.rbegin()) + *upper.begin()) / 2.0;
    }
};

vector<double> slidingMedian(const vector<int> &numbers, int k) {
    if (k <= 0 || k > static_cast<int>(numbers.size())) {
        return {};
    }
    MedianWindow window;
    vector<double> result;
    for (int i = 0; i < static_cast<int>(numbers.size()); ++i) {
        window.add(numbers[i]);
        if (i >= k) {
            window.remove(numbers[i - k]);
        }
        if (i >= k - 1) {
            result.push_back(window.median());
        }
    }
    return result;
}

int main() {
    auto medians = slidingMedian({1, 3, -1, -3, 5, 3, 6, 7}, 3);
    assert((medians == vector<double>{1, -1, -1, 3, 5, 6}));
    assert((slidingMedian({INT_MAX, INT_MAX}, 2) == vector<double>{INT_MAX}));
    assert((slidingMedian({2, 2, 2}, 2) == vector<double>{2, 2}));
    assert(slidingMedian({}, 1).empty());
    for (double median : medians) {
        cout << median << ' ';
    }
    cout << '\n';  // 1 -1 -1 3 5 6
}
