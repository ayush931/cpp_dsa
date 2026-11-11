/**
 * Erase overlap intervals
 *
 * Given closed intervals, remove the minimum number of intervals so that the
 * remaining intervals do not overlap. Intervals that only touch are allowed:
 * [1, 2] and [2, 3] can both stay.
 *
 * Brute force:
 *   Try every subset and keep the largest subset whose intervals are pairwise
 *   compatible. With n intervals that is O(2^n * n^2).
 *
 * Optimized greedy:
 *   Sort by end time, then keep an interval whenever its start is at or after
 *   the end of the last kept interval. Choosing the earliest finishing
 *   interval leaves the most room for the rest.
 *
 * Time        -> O(n log n) optimized, O(2^n * n^2) brute force
 * Space       -> O(n) for the sorted copy and subset bookkeeping
 *
 * Edge cases: empty input, one interval, touching endpoints, duplicate
 * intervals, negative coordinates, full overlap, and no overlap.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Interval {
    int start;
    int end;
};

bool compatible(const vector<Interval> &chosen) {
    for (int i = 0; i < (int)chosen.size(); i++) {
        for (int j = i + 1; j < (int)chosen.size(); j++) {
            bool separate = chosen[i].end <= chosen[j].start ||
                            chosen[j].end <= chosen[i].start;

            if (!separate) {
                return false;
            }
        }
    }

    return true;
}

int eraseOverlapBrute(const vector<Interval> &intervals) {
    int n = intervals.size();
    int bestKept = 0;

    for (int mask = 0; mask < (1 << n); mask++) {
        vector<Interval> chosen;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                chosen.push_back(intervals[i]);
            }
        }

        if (compatible(chosen)) {
            bestKept = max(bestKept, (int)chosen.size());
        }
    }

    return n - bestKept;
}

int eraseOverlap(vector<Interval> intervals) {
    if (intervals.empty()) {
        return 0;
    }

    sort(intervals.begin(), intervals.end(),
         [](const Interval &a, const Interval &b) {
             if (a.end != b.end) {
                 return a.end < b.end;
             }
             return a.start < b.start;
         });

    int kept = 0;
    int lastEnd = intervals[0].start;

    for (const Interval &interval : intervals) {
        if (interval.start >= lastEnd) {
            kept++;
            lastEnd = interval.end;
        }
    }

    return (int)intervals.size() - kept;
}

void print(const vector<Interval> &intervals) {
    cout << "[";

    for (int i = 0; i < (int)intervals.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "[" << intervals[i].start << "," << intervals[i].end << "]";
    }

    cout << "]";
}

void runCase(const vector<Interval> &intervals) {
    int optimized = eraseOverlap(intervals);
    int brute = eraseOverlapBrute(intervals);

    cout << "removals: " << optimized << "  brute: " << brute
         << "  agree: " << (optimized == brute ? "yes" : "no") << endl;
}

int main() {
    runCase({});
    runCase({{1, 2}});
    runCase({{1, 2}, {2, 3}, {3, 4}});
    runCase({{1, 2}, {1, 2}, {1, 2}});
    runCase({{1, 2}, {2, 3}, {1, 3}});
    runCase({{-5, -1}, {-2, 3}, {4, 7}});
    runCase({{1, 10}, {2, 3}, {4, 5}});
    runCase({{1, 2}, {3, 4}});

    return 0;
}

// Expected output:
// removals: 0  brute: 0  agree: yes
// removals: 0  brute: 0  agree: yes
// removals: 0  brute: 0  agree: yes
// removals: 2  brute: 2  agree: yes
// removals: 1  brute: 1  agree: yes
// removals: 1  brute: 1  agree: yes
// removals: 2  brute: 2  agree: yes
// removals: 0  brute: 0  agree: yes
//
// Special cases: empty input, one interval, touching endpoints, duplicate
// intervals, negative coordinates, full overlap, and no overlap.
//
// Why earliest end first? Suppose the optimal solution keeps some interval
// that ends later. Replacing it with the earliest finishing compatible
// interval cannot hurt any later choice, so the greedy exchange is safe.
