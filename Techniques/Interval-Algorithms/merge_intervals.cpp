/**
 * Merge intervals
 *
 * Given a list of closed intervals [start, end], merge every pair that
 * overlaps or touches. Two intervals touch when one ends exactly where the
 * next begins, for example [1, 2] and [2, 3]; they become [1, 3].
 *
 * Optimized approach:
 *   1. sort by start (then by end)
 *   2. scan once, extending the last merged interval while the next start is
 *      <= the last end
 *
 * Brute force:
 *   Build the overlap graph. Every connected component of that graph is one
 *   merged interval, so a BFS over all pairs finds the same answer in O(n^2).
 *
 * Time        -> O(n log n) optimized, O(n^2) brute force
 * Space       -> O(n) for the result, O(n) for the BFS bookkeeping
 *
 * Edge cases: empty input, one interval, touching endpoints, duplicate
 * intervals, negative coordinates, full overlap, and no overlap.
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Interval {
    int start;
    int end;

    bool operator==(const Interval &other) const {
        return start == other.start && end == other.end;
    }
};

bool overlaps(const Interval &a, const Interval &b) {
    return max(a.start, b.start) <= min(a.end, b.end);
}

vector<Interval> mergeBrute(const vector<Interval> &intervals) {
    int n = intervals.size();
    vector<bool> visited(n, false);
    vector<Interval> result;

    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            continue;
        }

        visited[i] = true;
        queue<int> pending;
        pending.push(i);

        int low = intervals[i].start;
        int high = intervals[i].end;

        while (!pending.empty()) {
            int current = pending.front();
            pending.pop();

            for (int j = 0; j < n; j++) {
                if (visited[j]) {
                    continue;
                }

                if (overlaps(intervals[current], intervals[j])) {
                    visited[j] = true;
                    low = min(low, intervals[j].start);
                    high = max(high, intervals[j].end);
                    pending.push(j);
                }
            }
        }

        result.push_back({low, high});
    }

    sort(result.begin(), result.end(),
         [](const Interval &a, const Interval &b) {
             if (a.start != b.start) {
                 return a.start < b.start;
             }
             return a.end < b.end;
         });

    return result;
}

vector<Interval> mergeIntervals(vector<Interval> intervals) {
    if (intervals.empty()) {
        return {};
    }

    sort(intervals.begin(), intervals.end(),
         [](const Interval &a, const Interval &b) {
             if (a.start != b.start) {
                 return a.start < b.start;
             }
             return a.end < b.end;
         });

    vector<Interval> merged;
    merged.push_back(intervals[0]);

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i].start <= merged.back().end) {
            merged.back().end = max(merged.back().end, intervals[i].end);
        } else {
            merged.push_back(intervals[i]);
        }
    }

    return merged;
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
    vector<Interval> optimized = mergeIntervals(intervals);
    vector<Interval> brute = mergeBrute(intervals);

    print(optimized);
    cout << "  brute agrees: " << (optimized == brute ? "yes" : "no") << endl;
}

int main() {
    runCase({});
    runCase({{1, 3}});
    runCase({{1, 3}, {2, 6}, {8, 10}, {15, 18}});
    runCase({{1, 2}, {2, 3}});
    runCase({{1, 4}, {1, 4}});
    runCase({{-5, -1}, {-2, 3}, {4, 7}});
    runCase({{1, 10}, {2, 3}, {4, 5}});
    runCase({{1, 2}, {3, 4}});

    return 0;
}

// Expected output:
// []  brute agrees: yes
// [[1,3]]  brute agrees: yes
// [[1,6], [8,10], [15,18]]  brute agrees: yes
// [[1,3]]  brute agrees: yes
// [[1,4]]  brute agrees: yes
// [[-5,3], [4,7]]  brute agrees: yes
// [[1,10]]  brute agrees: yes
// [[1,2], [3,4]]  brute agrees: yes
//
// Special cases: empty input, one interval, touching endpoints, duplicate
// intervals, negative coordinates, full overlap, and no overlap.
//
// The condition uses <=, not <, because closed intervals that touch share the
// endpoint and therefore form one continuous interval.
